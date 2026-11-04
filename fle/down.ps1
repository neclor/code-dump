# Stop agentd and the Factorio server container.
Set-Location $PSScriptRoot
Get-CimInstance Win32_Process -Filter "Name='python.exe'" |
    Where-Object { $_.CommandLine -match 'agentd.py|researchd.py' } |
    ForEach-Object { Stop-Process -Id $_.ProcessId -Force; Write-Host "stopped agentd" }
python start_server.py stop
