# One-command setup: Docker Factorio server -> (client joins) -> agentd (FLE session) -> researchd.
#   .\up.ps1 -Client    RECOMMENDED: also launches the Steam client and waits for it to join
#                       BEFORE the agent starts (see "why" below)
#   .\up.ps1            server + agent only, nobody can join the game afterwards
#   .\up.ps1 -Fresh     recreate the server (brand new world); combine with -Client
#
# Why the client must join first: in real-conditions mode FLE registers script events (on_tick,
# on_nth_tick) at runtime. A client joining LATER loads the map, finds events that the scenario's
# control.lua does not register and refuses with "script-event-mismatch". Clients that are already
# connected keep working. agentd also keeps `storage` free of Lua functions so that a join attempt
# cannot crash the server ("Cannot serialise lua functions").
param([switch]$Client, [switch]$Fresh)

Set-Location $PSScriptRoot
$env:PYTHONIOENCODING = "utf-8"

function Stop-Daemons {
    Get-CimInstance Win32_Process -Filter "Name='python.exe'" |
        Where-Object { $_.CommandLine -match 'agentd.py|researchd.py' } |
        ForEach-Object { Stop-Process -Id $_.ProcessId -Force }
}

function Wait-Until($what, [scriptblock]$check, $seconds = 120) {
    $deadline = (Get-Date).AddSeconds($seconds)
    while (-not (& $check)) {
        if ((Get-Date) -gt $deadline) { throw "Timed out waiting for $what" }
        Start-Sleep -Seconds 2
    }
    Write-Host "ok: $what"
}

# 1. server
$running = docker ps --filter "name=factorio_" --format "{{.Names}}"
if ($Fresh -and $running) {
    Stop-Daemons
    python start_server.py stop
    $running = $null
}
if (-not $running) {
    # whitelist is read at server start; only the human may join, and only when asked for
    $wl = if ($Client) { '["necloor"]' } else { '[]' }
    Set-Content -Path "$PSScriptRoot\config\server-whitelist.json" -Value $wl
    python start_server.py
}
Wait-Until "RCON on 27000" { (Test-NetConnection 127.0.0.1 -Port 27000 -WarningAction SilentlyContinue).TcpTestSucceeded }

# 2. client joins first
if ($Client) {
    Get-Process factorio -ErrorAction SilentlyContinue | Stop-Process
    Start-Sleep -Seconds 2
    & "$PSScriptRoot\join_client.ps1"
    Wait-Until "client joined the game" { (docker logs fle-factorio_0-1 2>&1 | Out-String) -match 'necloor joined the game' } 240
}

# 3. agentd (a new FactorioInstance clears entities, so it is only restarted here)
Stop-Daemons
Remove-Item agentd.log, agentd.err -ErrorAction SilentlyContinue
Start-Process -FilePath python -ArgumentList "agentd.py" -WorkingDirectory $PSScriptRoot `
    -RedirectStandardOutput agentd.log -RedirectStandardError agentd.err -WindowStyle Hidden
Wait-Until "agentd listening" { (Test-Path agentd.log) -and ((Get-Content agentd.log -Raw) -match 'listening on') } 180

# 4. researchd (auto-queues the next research)
Start-Process -FilePath python -ArgumentList "researchd.py" -WorkingDirectory $PSScriptRoot `
    -RedirectStandardOutput researchd.log -RedirectStandardError researchd.err -WindowStyle Hidden

Write-Host "Ready. Send code with: python fp.py -c `"print(player_location)`""
