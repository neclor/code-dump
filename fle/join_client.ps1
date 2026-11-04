# Launch the Steam Factorio client and connect to the local FLE server,
# so the game is rendered while the agent plays.
# The server runs base game only, so the client uses its own mod dir with the DLC disabled
# (this leaves your regular %APPDATA%\Factorio\mods\mod-list.json untouched).
$factorio = "E:\SteamLibrary\steamapps\common\Factorio\bin\x64\factorio.exe"
$modDir = Join-Path $PSScriptRoot "client_mods"
Start-Process -FilePath $factorio -ArgumentList "--mod-directory", $modDir, "--mp-connect", "localhost:34197"
# agentd's watcher turns the joined human into a spectator (zoom + camera at the agent) automatically.
