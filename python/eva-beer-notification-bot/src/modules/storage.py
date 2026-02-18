from pathlib import Path
import os
import json

from configs import bot_config


def load_users() -> dict[str, int]:
    path: Path = Path(bot_config.USERS_PATH)
    if not path.exists(): return {}

    if not path.is_file():
        raise ValueError(f"Path exists but is not a file: {path}")

    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError("Invalid JSON structure: expected dict")

    return {k: v for k, v in data.items()}


def save_users(users: dict[str, int]) -> None:
    path: Path = Path(bot_config.USERS_PATH)
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and not path.is_file():
        raise ValueError(f"Path exists but is not a file: {path}")

    tmp_path: Path = path.with_suffix(path.suffix + ".tmp")
    text: str = json.dumps({k: v for k, v in users.items()}, ensure_ascii=False, indent=2)
    tmp_path.write_text(text, encoding="utf-8")

    os.replace(tmp_path, path)
