from datetime import date as Date
from typing import get_args
import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class LocationRepository(Repository):

    def location_exists(self, location_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM location WHERE id = %s {'FOR UPDATE' if lock else ''}", [location_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def is_location_available(self, location_id: int, date: Date, exclude_event_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"""
                SELECT COUNT(*) AS n FROM location
                WHERE id = %s
                AND (SELECT COUNT(*) FROM event WHERE location = %s AND date = %s AND id != %s {'FOR UPDATE' if lock else ''}) = 0
                {'FOR UPDATE' if lock else ''}
            """, [location_id, location_id, date, exclude_event_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def get_all(self, filter: LocationFilter = LocationFilter(), lock: bool = False) -> list[Location]:
        conditions: list[str] = []
        params: list[object] = []

        if filter.id is not None: conditions.append("id = %s");             params.append(filter.id)
        if filter.street:         conditions.append("street LIKE %s");      params.append(f"%{filter.street}%")
        if filter.city:           conditions.append("city LIKE %s");        params.append(f"%{filter.city}%")
        if filter.postal_code:    conditions.append("postal_code LIKE %s"); params.append(f"%{filter.postal_code}%")
        if filter.country:        conditions.append("country LIKE %s");     params.append(f"%{filter.country}%")
        if filter.comment:        conditions.append("comment LIKE %s");     params.append(f"%{filter.comment}%")

        where: str = f"WHERE {' AND '.join(conditions)}" if conditions else ""
        column: LocationColumn = filter.sort_column if filter.sort_column in get_args(LocationColumn) else "id"
        order_by: str = f"ORDER BY {column} {'DESC' if filter.order == 'desc' else 'ASC'}"

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM location {where} {order_by} {'FOR UPDATE' if lock else ''}", params)
            rows: tuple[dict, ...] = cursor.fetchall()

        return [Location(**row) for row in rows]
