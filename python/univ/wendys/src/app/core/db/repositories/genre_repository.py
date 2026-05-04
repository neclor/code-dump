from typing import get_args
import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class GenreRepository(Repository):

    def genre_exists(self, name: str, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM genre WHERE name = %s {'FOR UPDATE' if lock else ''}", [name])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def genre_create_cycle(self, genre: Genre) -> bool:
        if genre.subgenre is None: return False

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute("SELECT genre_create_cycle(%s, %s) AS result", [genre.genre, genre.subgenre])
            row: dict | None = cursor.fetchone()

        return bool(row["result"]) if row else False


    def insert_ignore_genres(self, *names: str) -> None:
        if not names: return

        with self._db.cursor() as cursor:
            cursor.execute(f"INSERT IGNORE INTO genre (name) VALUES {', '.join(['(%s)'] * len(names))}", names)


    def insert_ignore_specializes(self, *genres: Genre) -> None:
        pairs: list[Genre] = [genre for genre in genres if genre.subgenre is not None]
        if not pairs: return

        with self._db.cursor() as cursor:
            cursor.execute(f"""
                INSERT IGNORE INTO specializes (genre, subgenre)
                VALUES {', '.join(['(%s, %s)'] * len(pairs))}
            """,
            [g for genre in pairs for g in (genre.genre, genre.subgenre)]
            )


    def get_dashboard(self) -> list[GenreDashboard]:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute("SELECT * FROM genre_with_subgenres ORDER BY genre")
            rows: tuple[dict, ...] = cursor.fetchall()

        return [GenreDashboard(**row) for row in rows]
