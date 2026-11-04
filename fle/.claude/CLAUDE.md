# FLE (Factorio Learning Environment) — local setup notes

**Playing the game: read `.claude/GAME_PROMPT.md` first** (goal, real-conditions rules, strategy, pitfalls).

Free mode (open_world / `open_play`) on Windows, Docker server + Steam client for rendering.
**Server is Factorio 2.0.77** (FLE breaks on 2.1; FLE pins 2.0.73 but Steam only offers 2.0.77 to the user, and client/server versions must match. 2.0.73 also works with FLE: `FLE_IMAGE=factoriotools/factorio:2.0.73`).
Steam client is on 2.0.77 (verified with `factorio.exe --version`, build 84539).
Everything below about 2.1 patches is historical; the patches are harmless on 2.0.

## Quick start

```powershell
cd C:\Git\code-dumb\fle
.\up.ps1 -Fresh -Client   # RECOMMENDED: server -> Steam client joins FIRST -> agentd -> researchd
.\up.ps1                  # server + agent, nobody can join (whitelist empty)
# Why client first: real-conditions (fast=False) FLE registers on_tick/on_nth_tick at runtime; a client
# joining later fails with script-event-mismatch. agentd's SANITIZE_LUA also moves ~80 functions out of
# `storage` (Factorio cannot serialise them; any map save = "Cannot serialise lua functions" = server crash).
# Real-conditions: agentd FLE_FAST=0 (default) -> character really walks/mines/crafts; each call logs tick.
.\down.ps1          # stop agentd + server
python fp.py -c "print(player_location)"   # play: send code to the session
```
(`start_server.py`, `join_client.ps1`, `agentd.py`, `play.py` are the building blocks; `up.ps1` calls them.
`up.ps1` always restarts agentd, and a new FactorioInstance clears entities, so re-running it wipes the factory.)

**Human viewer = god controller.** agentd's watcher thread (every 5 s) switches any connected human to `defines.controllers.god`
(camera zoom 0.7, teleported to the agent) and recreates the agent character if it is missing.
Why: a human joining as a normal player makes FLE lose its agent character (`nearest()` finds nothing, `storage.agent_characters` nil).
Spectator mode fixes that but has no map view (M) and starts at zoom 1/256 (black screen); god mode keeps the map.

- Container name: `fle-factorio_0-1` (image `factoriotools/factorio:2.0.77`, override with env `FLE_IMAGE`).
- `config/map-settings.json` is the original 2.0 one again (the 2.1 `enemy_expansion` variant was reverted).
- Map layout changed with the version: re-run `nearest(Resource.X)` instead of trusting old coordinates.
- Ports: game UDP 34197, RCON TCP 27000 (password `factorio`).
- Logs: `docker logs fle-factorio_0-1`. Restart: `docker restart fle-factorio_0-1`.
- `fle cluster ...` is NOT used: it runs run-envs.sh and pins the 2.0.73 image. `start_server.py` wraps `fle.cluster.run_envs.ClusterManager` instead.

## Files in this folder

- `start_server.py` — sets `ComposeGenerator.image`, uses local `config/` as `pkg_config_dir`, scenario `open_world`.
- `join_client.ps1` — runs `E:\SteamLibrary\steamapps\common\Factorio\bin\x64\factorio.exe --mod-directory client_mods --mp-connect localhost:34197`.
- `client_mods/mod-list.json` — base only, DLC (space-age, quality, elevated-rails) disabled. Keeps the user's real `%APPDATA%\Factorio\mods` untouched.
- `config/` — copy of FLE's `fle/cluster/config` with `map-settings.json` converted to the 2.1 `enemy_expansion` format; `server-whitelist.json` = `["necloor"]`, `server-adminlist.json` = `["client_master","necloor"]`.
- `play.py` — `gym.make("open_play", run_idx=0)`, sets speed 1, runs `nearest(Resource.IronOre)`.
- `.fle/` — sprites from `fle sprites` (~343 MB, 9496 files). Used only by FLE's own Python renderer (`map_image` in observations), not by the Steam client.

## Gotchas (already hit)

- **pip temp dir**: `TEMP` points to `X:\Temp`, which breaks sdist builds (`WinError 1`). Run pip with `$env:TEMP=$env:TMP="$env:LOCALAPPDATA\Temp\pip-tmp"`.
- **a2a-sdk**: must be `<1` (installed 0.3.26). 1.x breaks `from a2a.types import TextPart`.
- **Env ids**: registered as plain `open_play` (not `Factorio-open_play-v0`), need `run_idx=0`, and `import fle.env.gym_env.registry` first.
- **Server removes DLC data** (run_envs.py), so the client must have the DLC disabled, otherwise `Refusing connection ... ModsMismatch`. Always join via `join_client.ps1`.
- **Whitelist**: server runs with `--use-server-whitelist`; the player name (`necloor`) must be in `config/server-whitelist.json`.
- **Script cache**: FLE caches Lua checksums inside the running game. After editing any FLE Lua file, `docker restart fle-factorio_0-1` before retesting, or the old code keeps running.
- A Lua error in `on_tick` (alerts.lua) crashes the server; it auto-restarts (`restart: unless-stopped`). Find the cause with `docker logs fle-factorio_0-1 | Select-String 'Error while running event' -Context 0,1`.
- Debug game state via RCON: `factorio_rcon.RCONClient("127.0.0.1", 27000, "factorio").send_command("/silent-command ... rcon.print(...)")`.

## Patches to the installed FLE (global Python 3.13 site-packages, lost on reinstall)

Base path: `C:\Users\neclor\AppData\Local\Programs\Python\Python313\Lib\site-packages\fle\env\`

1. `tools/admin/score/server.lua` and `tools/agent/score/server.lua` (two copies, both loaded): `entity.fixed_recipe` is a LuaRecipePrototype in 2.1, not a string; use `.name` when not a string.
2. `mods/initialise.lua`: alias block at the top mapping removed 2.0 inventory defines to 2.1 `crafter_input/output/modules` and `rocket_silo_rocket` (`assembling_machine_*`, `furnace_*`, `rocket_silo_*`, `rocket`).
3. `mods/lualib_util.lua`: `util.product_amount` uses `(product.probability or 1)`.

If more 2.1 incompatibilities show up, fix them the same way. Fallback: image `factoriotools/factorio:2.0.73` (FLE's pinned version) plus rolling the Steam client back to 2.0.x.

## Playing the game myself (agent = Claude via terminal)

The user decided I am the agent: I play by sending FLE Python code to a persistent session.
Goal: advance as far as possible, build the biggest factory, launch a rocket.

```powershell
cd C:\Git\code-dumb\fle
python agentd.py            # once, in background (Start-Process, logs agentd.log/.err); holds FactorioInstance
python fp.py -c "print(inspect_inventory())"     # run a snippet
python fp.py -t 300 scripts\NN_name.py           # run a script file (timeout in s)
```

- Why a daemon: `FactorioInstance()` wipes entities on every construction, so never create it twice; variables persist in its namespace between calls.
- `docs/api.md` (generated by agentd.py) is the full tool manual (3500 lines). Read parts with offset/Grep, not whole.
- Start of world: empty inventory, spawn (0,0). Coal (40.5,-3.5), iron (47.5,10.5), stone (47.5,34.5), water (-14,34), wood (-9,47), copper (15.5,-72.5), crude oil far at (-280,-313).
- Server boots with `game.tick_paused=true` while FLE thinks it is unpaused -> path requests time out. agentd.py now sends `/sc game.tick_paused = false`. If `move_to` says "Path request timed out", check pause first.
- Scripts live in `scripts/`; movement/harvest/mining verified working. Opening script: `scripts/01_bootstrap.py`.

### (Obsolete after downgrade to 2.0.77) 2.1 incompat that was open

Recipes have `categories` (array) instead of `category` in 2.1. Fixed in `tools/agent/craft_item/server.lua` (inline pcall).
STILL TO FIX (the edit tool was denied permission for package edits, ask the user): `tools/agent/insert_item/server.lua:123` (`recipe.category == "smelting"`, hit when inserting ore into a furnace) and `mods/serialize.lua:606` (`category = '"' .. recipe.category .. '"'`).
Fix pattern: `local ok, c = pcall(function() return recipe.category end); category = ok and c or recipe.categories[1]`; for "is smelting" check any entry of `recipe.categories`.
After editing any Lua: stop agentd, `docker restart fle-factorio_0-1`, start agentd again (fresh world!).

## Game progress log (current world; wiped by `up.ps1`/agentd restart)

- Speed 2x (user asked). Helpers defined in session by scripts/04_helpers_coal.py: `inv`, `collect_plates`, `refuel`, `gather`; `place_drills` in scripts/05_expand_iron.py. Re-run those files after an agentd restart.
- Iron patch bbox (-30,-79)..(3,-49); coal near (40,-82); stone (55,-63); water (-14,34); wood (28,-83).
- 7 burner drills on iron with stone furnaces on the output (one drill sits on mixed ore and yields copper). Collect with `collect_plates()`, keep fueled with `refuel()`.
- Steam power plant at the water: pump (-12.5,34.5), boiler (-12.5,30), engine (-12.5,24.5), facing UP (land is north of the pump). Lab at (-6.5,24.5), powered through small poles.
- Research: Automation started (10 red packs). API lessons: `place_entity_next_to(..., spacing=N)` needs spacing for pipes (spacing 3 -> 9 pipes); craft gears first if craft_item says missing gears.

- Science: lab chain at y=24.5, x=-6.5,-2.5,1.5,5.5 (inserters between them in the 1-tile gaps pass packs along). Red science assembler chain above the first lab (gear asm (-6.5,16.5) -> science asm (-6.5,20.5) -> lab). Hand-fed pack chest (iron chest) at (-6.5,27.5) with inserter into lab 1.
- Boiler fed by chest (-9.5,30.5) + inserter; `feed_boiler_chest()` tops it up. 11 coal drills with wooden chests, 11+ iron drills, 5 copper drills.
- Session helpers: scripts 11 (research_next, collect_coal, feed_science), 15 (tend() = whole maintenance round). Log of every call: `logs/actions.log` / `actions.jsonl` (written by fp.py).
- Power 2: second plant at x=-22.5 (pump y~35, boiler (-22.5,32), engines (-22.5,26.5),(-22.5,21.5)), coal chest (-19.5,32.5). 4 engines total. 16 electric iron drills + 11 burner on coal, 12 labs in a row (y=24.5, x=-6.5..+49).
- Agent inventory FILLS UP (extract_item then fails with "Could not extract."). Dump surplus with `stash(proto, keep)` (scripts/24_storage.py, chests at (10.5+2k,28.5)); copper also sits in the pack chest. Electric mining drill needs tech `electric-mining-drill` which is missing from the Technology enum: queue it via RCON `game.forces.player.add_research('electric-mining-drill')`.
- BUGS to avoid: `get_entity/get_entities` fail on labs that hold packs (serializer expects dict, gets list) -> use coordinates/inserter chains; `place_entity(Lab)` raises "[pack]" but DOES place the lab; `craft_item` of composite items fails with "missing iron-gear-wheel": pre-craft gears/circuits first.

## REAL-CONDITIONS run (current; started after user's restart request, FLE fast=False)

- World wiped by every `up.ps1`. At game_time ~22 min: ~7 burner drills on iron, 2 coal drills with wooden chests at (39,-83),(42,-83), smelter bank of stone furnaces next to iron patch, steam plant at water (pump (-13.5,34.5), boiler (-13.5,30), engine (-13.5,24.5)), 1 lab (-8.5,24.5) fed by pack chest (-8.5,27.5)+inserter.
- Run helpers in this order after a daemon restart: scripts/39_sync_location.py LAST (it defines go/gather/near/collect_plates; 38_tend.py redefines `near` with the old buggy version, so re-run 39 after 38). `tend()` = collect_coal + collect_plates + refuel (scripts/38).
- **player_location drift**: FLE's move_to builds paths from a CACHED location refreshed only by successful move_to; after connect_entities/other walking it goes stale and every move_to fails with "Path not found". Use `go(pos)` (syncs from the real character through `move_to.connection.rcon_client` and `move_to.game_state.player_location`).
- Hand mining is ~4x faster than vanilla in FLE (20 ore ~ 585 ticks); walking/crafting are real. Mention when judging time.
- Client MUST join before agentd starts (script-event-mismatch otherwise); a map save with functions in `storage` crashes the server -> agentd's SANITIZE_LUA proxies them. RCON can drop; agentd calls ensure_connected().
- README.md in the project root is the human-facing guide (Russian).

## Verified so far

Server up, client joined, FLE connected over RCON, tools loaded, `nearest(Resource.IronOre)` -> `Position(x=47.5, y=10.5)`.
Not verified: complex agent tools (crafting, belts, fluids) on 2.1, and that `map_image` is non-empty after a reset.
