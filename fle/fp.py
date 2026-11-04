"""Send a snippet of FLE agent code to the running agentd.py and print the result.

    python fp.py script.py
    python fp.py -c "print(nearest(Resource.Coal))"
    python fp.py -t 300 script.py        # custom timeout in seconds
"""

import argparse
import json
import os
import socket
import sys
from datetime import datetime
from pathlib import Path

sys.stdout.reconfigure(encoding="utf-8")

p = argparse.ArgumentParser()
p.add_argument("file", nargs="?")
p.add_argument("-c", dest="code")
p.add_argument("-t", dest="timeout", type=int, default=120)
p.add_argument("--speed", type=float, help="set game speed (e.g. 5) and exit")
args = p.parse_args()

if args.speed is not None:
    request = {"speed": args.speed}
else:
    code = args.code if args.code is not None else open(args.file, encoding="utf-8").read()
    request = {"code": code, "timeout": args.timeout}
port = int(os.environ.get("FLE_AGENT_PORT", "8765"))

with socket.create_connection(("127.0.0.1", port), timeout=args.timeout + 30) as s:
    s.sendall((json.dumps(request) + "\n").encode("utf-8"))
    resp = json.loads(s.makefile("rb").readline().decode("utf-8"))

print(resp["output"])
tick = resp.get("tick")
game_time = f" game_time={tick / 3600:.1f}min" if tick is not None else ""
print(f"[reward={resp['reward']} duration={resp['duration']:.1f}s tick={tick}{game_time}]")

# Persistent action log: one JSON line per call (+ a readable text copy), appended across sessions.
log_dir = Path(__file__).parent / "logs"
log_dir.mkdir(exist_ok=True)
now = datetime.now()
entry = {
    "time": now.isoformat(timespec="seconds"),
    "request": request,
    "source": args.file or "-c",
    "response": resp,
}
with open(log_dir / "actions.jsonl", "a", encoding="utf-8") as f:
    f.write(json.dumps(entry, ensure_ascii=False) + "\n")
with open(log_dir / "actions.log", "a", encoding="utf-8") as f:
    code_text = request.get("code", f"<speed {request.get('speed')}>")
    f.write(f"=== {entry['time']} [{entry['source']}] reward={resp['reward']}\n{code_text}\n--- output\n{resp['output']}\n\n")
