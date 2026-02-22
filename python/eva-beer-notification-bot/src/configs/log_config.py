from pathlib import Path
import logging


LOG_LEVEL: int = logging.WARNING
LOG_FORMAT: str = "[%(levelname)s %(asctime)s] %(name)s: %(message)s"
DATE_FORMAT: str = "%d-%m-%Y %H:%M"
LOG_PATH: str = "logs/bot.log"


path: Path = Path(LOG_PATH)
path.parent.mkdir(parents=True, exist_ok=True)
if not path.exists():
    path.touch()

logging.basicConfig(
    level=LOG_LEVEL,
    format=LOG_FORMAT,
    datefmt=DATE_FORMAT,
    handlers=[logging.FileHandler(LOG_PATH), logging.StreamHandler()]
)
