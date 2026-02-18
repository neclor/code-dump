import logging
import subprocess


logger: logging.Logger = logging.getLogger(__name__)


def git_pull() -> None:
    result: subprocess.CompletedProcess[str] = subprocess.run(["git", "pull"], capture_output=True, text=True)
    if result.returncode != 0: raise Exception(result.stderr)
