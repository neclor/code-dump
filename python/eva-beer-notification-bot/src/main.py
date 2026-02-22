
import asyncio
import logging

from bot.bot import Bot

from configs import log_config


logger: logging.Logger = logging.getLogger(__name__)


def main() -> None:
    logger.info("Start")

    bot: Bot = Bot()
    bot.start_bot()
    asyncio.get_event_loop().run_forever()

    logger.info("Exit")


if __name__ == "__main__": main()
