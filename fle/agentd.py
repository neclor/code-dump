"""Long-lived FLE session so game state and agent variables persist between my commands.

FactorioInstance wipes entities on every construction, so it is created once here.
Run once (background):  python agentd.py
Send code with:         python fp.py script.py   (or: python fp.py -c "print(...)")

Protocol: one JSON line in {"code": str, "timeout": int}, one JSON line out
{"reward": float, "duration": float, "output": str}.
"""

import json
import os
import socketserver
import sys
import threading
import time
import traceback
from pathlib import Path

from fle.env import FactorioInstance

HOST, PORT = "127.0.0.1", int(os.environ.get("FLE_AGENT_PORT", "8765"))
RCON_PORT = int(os.environ.get("FLE_RCON_PORT", "27000"))
SPEED = float(os.environ.get("FLE_SPEED", "2"))
FAST = os.environ.get("FLE_FAST", "0") == "1"
DOCS = Path(__file__).parent / "docs"

lock = threading.Lock()  # one snippet at a time: the game state is shared
instance = None


def start_instance():
    global instance
    instance = FactorioInstance(
        address="localhost",
        tcp_port=RCON_PORT,
        num_agents=1,
        # fast=True executes actions instantly and only books the time they would take.
        # Real-conditions mode (default): the character really walks, mines and crafts in game time.
        fast=FAST,
        cache_scripts=True,
        inventory={},
        all_technologies_researched=False,
        peaceful=True,
        reset_speed=SPEED,
    )
    instance.set_speed_and_unpause(SPEED)
    # The server boots with tick_paused=true while FLE believes it is running, so its own
    # unpause() is a no-op. Without ticks, path requests never finish and move_to times out.
    instance.rcon_client.send_command("/sc game.tick_paused = false")
    DOCS.mkdir(exist_ok=True)
    (DOCS / "api.md").write_text(instance.get_system_prompt(0), encoding="utf-8")
    print(f"FLE instance ready, speed {SPEED}; API docs in {DOCS / 'api.md'}", flush=True)


WATCH_LUA = (
    "/sc for _, p in pairs(game.connected_players) do "
    "if p.controller_type ~= defines.controllers.god then "
    # god = free camera without a character; unlike spectator it keeps the map view (M)
    "p.set_controller{type=defines.controllers.god} "
    "p.zoom = 0.7 "  # a fresh spectator/god camera can start at zoom 1/256 -> black screen
    "local a = storage.agent_characters and storage.agent_characters[1] "
    "if a and a.valid then p.teleport(a.position, a.surface) end end end "
    "local ch = storage.agent_characters and storage.agent_characters[1] "
    "rcon.print((ch and ch.valid) and 'ok' or 'no_agent')"
)


# FLE keeps ~80 Lua functions inside `storage`. Factorio cannot serialise functions, so ANY map
# save (a client joining, autosave) kills the server with "Cannot serialise lua functions".
# Move the functions into a plain global table and leave metatable proxies in `storage`
# (unregistered metatables are simply not saved). Re-run periodically: scripts may re-add functions.
SANITIZE_LUA = (
    "/sc FLE_FUNCS = FLE_FUNCS or {} "
    "local function fix(t, id, depth) "
    "  local funcs = FLE_FUNCS[id] or {} FLE_FUNCS[id] = funcs "
    "  local keys = {} "
    "  for k, v in pairs(t) do keys[#keys + 1] = k end "
    "  for _, k in ipairs(keys) do "
    "    local v = rawget(t, k) "
    "    if type(v) == 'function' then funcs[k] = v rawset(t, k, nil) "
    "    elseif type(v) == 'table' and depth < 3 and type(k) == 'string' and not getmetatable(v) then "
    "      fix(v, id .. '.' .. k, depth + 1) "
    "    elseif type(v) == 'table' and depth < 3 and type(k) == 'string' then fix(v, id .. '.' .. k, depth + 1) end "
    "  end "
    "  if not getmetatable(t) then "
    "    local ok = pcall(setmetatable, t, {__index = funcs, __newindex = function(tt, kk, vv) "
    "      if type(vv) == 'function' then funcs[kk] = vv else rawset(tt, kk, vv) end end}) "
    "    if not ok then return false end "
    "  end "
    "  return true "
    "end "
    "rcon.print(tostring(fix(storage, 'root', 0)))"
)


def watcher():
    """A human joining as a normal player makes FLE lose its agent character (nearest() then
    finds nothing). Keep humans as god-mode cameras and recreate the agent character if it is gone."""
    while True:
        time.sleep(5)
        try:
            with lock:
                instance.ensure_connected()
                instance.rcon_client.send_command(SANITIZE_LUA)
                state = instance.rcon_client.send_command(WATCH_LUA)
                if state and "no_agent" in state:
                    instance.first_namespace._create_agent_characters(1)
                    print("agent character recreated", flush=True)
        except Exception:
            print("watcher error:", traceback.format_exc(), flush=True)


class Handler(socketserver.StreamRequestHandler):
    def handle(self):
        try:
            req = json.loads(self.rfile.readline().decode("utf-8"))
            if "speed" in req:  # change game speed without restarting (a restart wipes the world)
                with lock:
                    instance.set_speed_and_unpause(float(req["speed"]))
                resp = {"reward": 0, "duration": 0, "output": f"game speed {req['speed']}"}
            else:
                with lock:
                    instance.ensure_connected()  # RCON can drop during long non-fast actions
                    reward, duration, output = instance.eval(
                        req["code"], timeout=req.get("timeout", 120)
                    )
                    try:
                        tick = instance.rcon_client.send_command("/sc rcon.print(game.tick)")
                    except Exception:
                        instance.reconnect()
                        tick = instance.rcon_client.send_command("/sc rcon.print(game.tick)")
                resp = {
                    "reward": reward,
                    "duration": duration,
                    "output": str(output),
                    "tick": int(tick) if tick and tick.strip().isdigit() else None,
                }
        except Exception:
            resp = {"reward": -1, "duration": 0, "output": traceback.format_exc()}
        self.wfile.write((json.dumps(resp) + "\n").encode("utf-8"))


class Server(socketserver.ThreadingTCPServer):
    allow_reuse_address = True
    daemon_threads = True


if __name__ == "__main__":
    sys.stdout.reconfigure(encoding="utf-8")
    start_instance()
    threading.Thread(target=watcher, daemon=True).start()
    with Server((HOST, PORT), Handler) as srv:
        print(f"listening on {HOST}:{PORT}", flush=True)
        srv.serve_forever()
