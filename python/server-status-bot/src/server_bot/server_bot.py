import asyncio
import logging
from telethon import TelegramClient

from configs import api_keys
from modules import server_manager as sm
from server_bot import commands as cmds


logger: logging.Logger = logging.getLogger(__name__)


class ServerBot:
    RETRY_DEALY: int = 30
    STATUS_UPDATE_DELAY: int = 10 * 60


    def __init__(self) -> None:
        self._client: TelegramClient = TelegramClient(api_keys.MY_SESSION_NAME, api_keys.API_ID, api_keys.API_HASH)
        self._status_message_ids: dict[int | str, list[int]] = {}

        cmds.setup_client(self._client)


    def start(self) -> None:
        loop: asyncio.AbstractEventLoop = asyncio.get_event_loop()
        loop.create_task(self._connect())
        loop.create_task(self._update_status())


    async def _connect(self) -> None:
        while True:
            try:
                async with self._client:
                    logger.info("Connected successfully")
                    await self._client.disconnected
            except Exception: pass
            logger.warning(f"Connection error")
            await asyncio.sleep(self.RETRY_DEALY)


    async def _update_status(self) -> None:
        while True:
            if not self._client.is_connected():
                await asyncio.sleep(1)
                continue

            for user in api_keys.USERS:
                try:
                    status_message = await self._client.send_message(user, sm.get_status())

                    if user in self._status_message_ids:
                        try:
                            await self._client.delete_messages(user, self._status_message_ids[user])
                            self._status_message_ids[user] = []
                        except Exception as e:
                            logger.warning(f"Deleting message error: {e}")

                    self._status_message_ids.setdefault(user, []).append(status_message.id)
                except Exception as e:
                    logger.warning(f"Error updating status: {e}")

            await asyncio.sleep(self.STATUS_UPDATE_DELAY)
