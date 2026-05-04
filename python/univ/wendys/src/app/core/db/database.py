import logging
import time
import pymysql


logger: logging.Logger = logging.getLogger(__name__)


_RETRIES: int = 10
_WAIT_TIME: int = 5


def connect_db(user: str, password: str, host: str, database: str) -> pymysql.connections.Connection:
    for _ in range(_RETRIES):
        try:
            db: pymysql.connections.Connection = pymysql.connections.Connection(
                user=user,
                password=password,
                host=host,
                database=database,
                cursorclass=pymysql.cursors.DictCursor,
                autocommit=True
            )
            logger.info("Connected to database: %s@%s/%s", user, host, database)
            return db
        except pymysql.err.OperationalError:
            logger.info("Database not ready. Retrying in %d seconds...", _WAIT_TIME)
            time.sleep(_WAIT_TIME)

    raise RuntimeError(f"Could not connect to the database {database}.")

