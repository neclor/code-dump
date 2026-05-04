from datetime import date as Date
import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class EmployeeRepository(Repository):

    def dj_exists(self, dj_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM dj WHERE id = %s {'FOR UPDATE' if lock else ''}", [dj_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def is_dj_available(self, dj_id: int, date: Date, exclude_event_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"""
                SELECT COUNT(*) AS n FROM dj
                WHERE id = %s
                AND (SELECT COUNT(*) FROM event WHERE dj = %s AND date = %s AND id != %s {'FOR UPDATE' if lock else ''}) = 0
                {'FOR UPDATE' if lock else ''}
            """, [dj_id, dj_id, date, exclude_event_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def planner_exists(self, planner_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM eventplanner WHERE id = %s {'FOR UPDATE' if lock else ''}", [planner_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def is_planner_available(self, planner_id: int, date: Date, exclude_event_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"""
                SELECT COUNT(*) AS n FROM eventplanner
                WHERE id = %s
                AND (SELECT COUNT(*) FROM event WHERE event_planner = %s AND date = %s AND id != %s {'FOR UPDATE' if lock else ''}) = 0
                {'FOR UPDATE' if lock else ''}
            """, [planner_id, planner_id, date, exclude_event_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def manager_exists(self, manager_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM manager WHERE id = %s {'FOR UPDATE' if lock else ''}", [manager_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False
