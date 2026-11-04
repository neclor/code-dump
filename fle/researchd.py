"""Keeps research running: whenever nothing is being researched, queue the best available tech.

Separate process with its own RCON connection, so it does not touch agentd's FactorioInstance.
Picks techs whose prerequisites are done and whose packs are all in PACKS, key rocket-path techs first.
    python researchd.py [pack,pack,...]     default packs: automation,logistic
"""

import sys
import time

import factorio_rcon

PACKS = (sys.argv[1] if len(sys.argv) > 1 else "automation-science-pack,logistic-science-pack").split(",")
PRIORITY = [
    "electric-mining-drill", "automation-2", "fluid-handling", "oil-gathering", "oil-processing", "plastics", "advanced-circuit",
    "sulfur-processing", "engine", "automobilism", "stack-inserter", "advanced-material-processing",
    "solar-energy", "research-speed-1", "research-speed-2", "electric-energy-distribution-1",
    "battery", "flammables", "chemical-science-pack", "low-density-structure", "rocket-fuel",
    "rocket-control-unit", "rocket-silo", "space-science-pack", "military", "military-2",
    "stone-wall", "gates", "heavy-armor", "steel-axe", "toolbelt", "railway", "automation-2", "logistics-2",
]

LUA = """/sc local f=game.forces.player
if f.current_research then rcon.print('busy:'..f.current_research.name) return end
local allowed = {%s}
local prio = {%s}
local best, best_score
for name, t in pairs(f.technologies) do
  if t.enabled and not t.researched and not t.prototype.hidden then
    local ok = true
    for _, p in pairs(t.prerequisites) do if not p.researched then ok = false break end end
    if ok then
      for _, ing in pairs(t.research_unit_ingredients) do if not allowed[ing.name] then ok = false break end end
    end
    if ok then
      local score = (prio[name] or 1000) * 1000000 + t.research_unit_count
      if not best_score or score < best_score then best, best_score = name, score end
    end
  end
end
if best then f.add_research(best) rcon.print('queued:'..best) else rcon.print('none available') end"""


def lua_for():
    allowed = ",".join(f"['{p}']=true" for p in PACKS)
    prio = ",".join(f"['{n}']={i}" for i, n in enumerate(PRIORITY))
    return LUA % (allowed, prio)


def main():
    code = lua_for()
    client = factorio_rcon.RCONClient("127.0.0.1", 27000, "factorio")
    last = None
    while True:
        try:
            out = client.send_command(code.replace("\n", " "))
            if out != last and not out.startswith("busy"):
                print(time.strftime("%H:%M:%S"), out, flush=True)
            last = out
        except Exception as e:
            print("researchd error:", e, flush=True)
            time.sleep(5)
            client = factorio_rcon.RCONClient("127.0.0.1", 27000, "factorio")
        time.sleep(10)


if __name__ == "__main__":
    main()
