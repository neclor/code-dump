import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class ThemeRepository(Repository):

    def theme_exists(self, name: str, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM theme WHERE name = %s {'FOR UPDATE' if lock else ''}", [name])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False
