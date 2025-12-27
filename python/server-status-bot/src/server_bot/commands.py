import logging
from telethon.sync import TelegramClient, events

from configs import log_config, api_keys
from modules import server_manager as sm


logger: logging.Logger = logging.getLogger(__name__)


TELEGRAM_MESSAGE_LIMIT: int = 512


def setup_client(client: TelegramClient) -> None:
    for pattern, function in COMMANDS.items():
        client.on(events.NewMessage(chats=api_keys.USERS, from_users=api_keys.USERS, pattern=pattern))(function)


async def status(event) -> None:
    await event.respond(sm.get_status())


async def logs(event) -> None:
    logs: str = ""
    try:
        with open(log_config.LOG_PATH, "r") as log_file:
            logs = log_file.read()
            if len(logs) > TELEGRAM_MESSAGE_LIMIT:
                logs = logs[-TELEGRAM_MESSAGE_LIMIT:]
    except Exception as e:
        logger.error(f"Logs reading error: {e}")
        await event.respond(f"Logs reading error: {e}")
        return

    try:
        await event.respond(log_config.LOG_PATH + ":\n" + logs)
    except Exception as e:
        logger.error(f"Logs sending error: {e}")
        await event.respond(f"Logs sending error: {e}")
        logger.warning(logs)


async def restart(event) -> None:
    try:
        sm.terminate_service()
    except Exception as e:
        logger.error(f"Service termination error: {e}")
        await event.respond(f"Service termination error: {e}")


async def help(event) -> None:
    message: str = """```
Commands:
    status     Show status
    logs       Show logs
    restart    Restart bot
    help       Show help
    version    Show version
```"""
    await event.respond(message, parse_mode="markdown")


async def version(event) -> None:
    message: str = "Server Status Bot v2.0.0 neclor"
    await event.respond(message)


COMMANDS: dict = {
    r"(?i)s(t|ta|tat|tatu|tatus)?": status,
    r"(?i)l(o|og|ogs)?": logs,
    r"(?i)r(e|es|est|esta|estar|estart)?": restart,
    r"(?i)h(e|el|elp)?": help,
    r"(?i)v(e|er|ers|ersi|ersio|ersion)?": version,
}
