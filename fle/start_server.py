"""Start the FLE Factorio server in free mode (open_world scenario).

Uses factoriotools/factorio:2.0.77 (matches the Steam client; Steam offers no 2.0.73): FLE targets Factorio 2.0 and breaks on 2.1
(recipe.category, defines.inventory, map-settings...). The Steam client must be on a
2.0.x branch to join. Override with FLE_IMAGE if needed.

    python start_server.py          # start
    python start_server.py stop     # stop
"""

import os
import sys
from pathlib import Path

from fle.cluster.run_envs import ClusterManager, ComposeGenerator

ComposeGenerator.image = os.environ.get("FLE_IMAGE", "factoriotools/factorio:2.0.77")

SCENARIO = "open_world"
# FLE's bundled config targets Factorio 2.0; ./config has map-settings.json adapted to 2.1.
CONFIG_DIR = Path(__file__).parent / "config"

if __name__ == "__main__":
    manager = ClusterManager()
    manager.pkg_config_dir = CONFIG_DIR
    if len(sys.argv) > 1 and sys.argv[1] == "stop":
        manager.stop()
    else:
        manager.start(num_instances=1, scenario=SCENARIO)
