from typing import get_args
import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class EventRepository(Repository):

    def event_exists(self, event_id: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM event WHERE id = %s {'FOR UPDATE' if lock else ''}", [event_id])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def get_by_id(self, event_id: int, lock: bool = False) -> Event | None:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM event WHERE id = %s {'FOR UPDATE' if lock else ''}", [event_id])
            row: dict | None = cursor.fetchone()

        return Event(**row) if row is not None else None


    def save_event(self, event: Event) -> int:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            if event.id is None:
                cursor.execute("""
                    INSERT INTO event (name, date, description, theme, type, client,
                        manager, event_planner, dj, location, rental_fee, playlist)
                    VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)
                """, [
                    event.name, event.date, event.description, event.theme, event.type, event.client,
                    event.manager, event.event_planner, event.dj, event.location,
                    event.rental_fee, event.playlist
                ])
                return cursor.lastrowid
            else:
                cursor.execute("""
                    UPDATE event
                    SET name=%s, date=%s, description=%s, theme=%s, type=%s,
                        manager=%s, event_planner=%s, dj=%s, location=%s,
                        rental_fee=%s, playlist=%s
                    WHERE id=%s
                """, [
                    event.name, event.date, event.description, event.theme, event.type,
                    event.manager, event.event_planner, event.dj, event.location,
                    event.rental_fee, event.playlist,
                    event.id
                ])
                return event.id


    def delete_event(self, event_id: int) -> None:
        with self._db.cursor() as cursor:
            cursor.execute("DELETE FROM event WHERE id = %s", [event_id])


    def get_dashboard(self, filter: EventDashboardFilter = EventDashboardFilter()) -> list[EventDashboard]:
        column: EventDashboardColumn = filter.sort_column if filter.sort_column in get_args(EventDashboardColumn) else "date"
        order_by: str = f"ORDER BY {column} {'DESC' if filter.order == 'desc' else 'ASC'}"

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM event_dashboard {order_by}")
            rows: tuple[dict, ...] = cursor.fetchall()

        return [EventDashboard(**row) for row in rows]


    def get_all(self, filter: EventFilter = EventFilter(), lock: bool = False) -> list[Event]:
        conditions: list[str] = []
        params: list[object] = []

        if filter.id is not None:            conditions.append("id = %s");             params.append(filter.id)
        if filter.name:                      conditions.append("name LIKE %s");        params.append(f"%{filter.name}%")
        if filter.date is not None:          conditions.append("date = %s");           params.append(filter.date)
        if filter.description:               conditions.append("description LIKE %s"); params.append(f"%{filter.description}%")
        if filter.client is not None:        conditions.append("client = %s");         params.append(filter.client)
        if filter.manager is not None:       conditions.append("manager = %s");        params.append(filter.manager)
        if filter.event_planner is not None: conditions.append("event_planner = %s");  params.append(filter.event_planner)
        if filter.dj is not None:            conditions.append("dj = %s");             params.append(filter.dj)
        if filter.theme:                     conditions.append("theme LIKE %s");       params.append(f"%{filter.theme}%")
        if filter.type:                      conditions.append("type LIKE %s");        params.append(f"%{filter.type}%")
        if filter.location is not None:      conditions.append("location = %s");       params.append(filter.location)
        if filter.rental_fee is not None:    conditions.append("rental_fee = %s");     params.append(filter.rental_fee)
        if filter.playlist:                  conditions.append("playlist LIKE %s");    params.append(f"%{filter.playlist}%")

        where: str = f"WHERE {' AND '.join(conditions)}" if conditions else ""
        column: EventColumn = filter.sort_column if filter.sort_column in get_args(EventColumn) else "date"
        order_by: str = f"ORDER BY {column} {'DESC' if filter.order == 'desc' else 'ASC'}"

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM event {where} {order_by} {'FOR UPDATE' if lock else ''}", params)
            rows: tuple[dict, ...] = cursor.fetchall()

        return [Event(**row) for row in rows]
