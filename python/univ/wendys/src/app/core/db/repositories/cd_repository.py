from datetime import date as Date
from typing import get_args
import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class CdRepository(Repository):

    def playlist_exists(self, playlist: str, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM playlist WHERE name = %s {'FOR UPDATE' if lock else ''}", [playlist])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def is_playlist_available(self, playlist: str, date: Date, exclude_event_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"""
                SELECT (
                    SELECT COUNT(*) FROM playlist WHERE name = %s {'FOR UPDATE' if lock else ''}
                ) = 1
                AND (
                    SELECT COUNT(*) = 0
                    FROM contains c JOIN cd USING (cd_number)
                    WHERE c.playlist = %s
                    AND (
                        SELECT COUNT(*)
                        FROM event e
                        JOIN contains c2 ON c2.playlist = e.playlist AND c2.cd_number = c.cd_number
                        WHERE e.date = %s AND e.id != %s
                        {'FOR UPDATE' if lock else ''}
                    ) >= cd.copies
                    {'FOR UPDATE' if lock else ''}
                ) AS is_available
            """, [playlist, playlist, date, exclude_event_id])
            row: dict | None = cursor.fetchone()
        return bool(row["is_available"]) if row else False


    def cd_exists(self, cd_number: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM cd WHERE cd_number = %s {'FOR UPDATE' if lock else ''}", [cd_number])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def get_cd_by_number(self, cd_number: int, lock: bool = False) -> Cd | None:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM cd WHERE cd_number = %s {'FOR UPDATE' if lock else ''}", [cd_number])
            row: dict | None = cursor.fetchone()
        return Cd(**row) if row else None


    def save_cd(self, cd: Cd) -> int:
        with self._db.cursor() as cursor:
            if cd.cd_number is None:
                cursor.execute("""
                    INSERT INTO cd (title, producer, year, copies)
                    VALUES (%s, %s, %s, %s)
                """, [cd.title, cd.producer, cd.year, cd.copies])
                return cursor.lastrowid
            else:
                cursor.execute("""
                    UPDATE cd SET title=%s, producer=%s, year=%s, copies=%s
                    WHERE cd_number = %s
                """, [cd.title, cd.producer, cd.year, cd.copies, cd.cd_number])
                return cd.cd_number


    def delete_cd(self, cd_number: int) -> None:
        with self._db.cursor() as cursor:
            cursor.execute("DELETE FROM cd WHERE cd_number = %s", [cd_number])


    def get_cd_statistics(self, filter: CdStatFilter = CdStatFilter()) -> list[CdStat]:
        column: CdStatColumn = filter.sort_column if filter.sort_column in get_args(CdStatColumn) else "cd_number"
        order_by: str = f"ORDER BY {column} {'DESC' if filter.order == 'desc' else 'ASC'}"

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM cd_statistics {order_by}")
            rows: tuple[dict, ...] = cursor.fetchall()

        return [CdStat(**row) for row in rows]


    def get_cd_dashboard(self, filter: CdDashboardFilter = CdDashboardFilter()) -> list[CdDashboard]:
        column: CdDashboardColumn = filter.sort_column if filter.sort_column in get_args(CdDashboardColumn) else "date"
        order_by: str = f"ORDER BY {column} {'DESC' if filter.order == 'desc' else 'ASC'}"

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM cd_dashboard {order_by}")
            rows: tuple[dict, ...] = cursor.fetchall()

        return [CdDashboard(**row) for row in rows]


    def song_exists(self, cd_number: int, track_number: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM song WHERE cd_number = %s AND track_number = %s {'FOR UPDATE' if lock else ''}", [cd_number, track_number])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def get_song(self, cd_number: int, track_number: int, lock: bool = False) -> Song | None:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM song WHERE cd_number = %s AND track_number = %s {'FOR UPDATE' if lock else ''}", [cd_number, track_number])
            row: dict | None = cursor.fetchone()
        return Song(**row) if row else None


    def save_song(self, song: Song) -> None:
        with self._db.cursor() as cursor:
            cursor.execute("""
                INSERT INTO song (cd_number, track_number, title, artist, duration, genre)
                VALUES (%s, %s, %s, %s, %s, %s)
                ON DUPLICATE KEY UPDATE title=%s, artist=%s, duration=%s, genre=%s
            """, [
                song.cd_number, song.track_number, song.title, song.artist, song.duration, song.genre,
                song.title, song.artist, song.duration, song.genre
            ])


    def delete_song(self, cd_number: int, track_number: int) -> None:
        with self._db.cursor() as cursor:
            cursor.execute("DELETE FROM song WHERE cd_number = %s AND track_number = %s", [cd_number, track_number])


    def get_songs_by_cd(self, cd_number: int, lock: bool = False) -> list[Song]:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM song WHERE cd_number = %s ORDER BY track_number {'FOR UPDATE' if lock else ''}", [cd_number])
            rows: tuple[dict, ...] = cursor.fetchall()
        return [Song(**row) for row in rows]
