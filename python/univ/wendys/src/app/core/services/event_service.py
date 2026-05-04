import logging
import pymysql

from datetime import date as Date
from app.core.services.transaction import Transaction
from app.core.models import *
from app.core.db.repositories import *


logger: logging.Logger = logging.getLogger(__name__)


class EventService:
    def __init__(self,
        db: pymysql.connections.Connection,
        events: EventRepository,
        employees: EmployeeRepository,
        cd: CdRepository,
        clients: ClientRepository,
        location: LocationRepository,
        themes: ThemeRepository
    ) -> None:
        self._db = db
        self._events = events
        self._employees = employees
        self._cd = cd
        self._clients = clients
        self._location = location
        self._themes = themes


    def get_by_id(self, event_id: int) -> Event | None:
        return self._events.get_by_id(event_id)


    def save(self, event: Event) -> int:
        with Transaction(self._db):
            exclude_id: int = event.id if event.id is not None else -1

            if event.id is not None and not self._events.event_exists(event.id, lock=True): raise ValueError("Event not found.")
            if event.date <= Date.today(): raise ValueError("Event date must be in the future.")

            if not self._clients.client_exists(event.client, lock=True): raise ValueError("Client does not exist.")
            if not self._themes.theme_exists(event.theme, lock=True): raise ValueError("Theme does not exist.")

            if event.manager is not None and not self._employees.manager_exists(event.manager, lock=True): raise ValueError("Manager does not exist.")
            if event.event_planner is not None and not self._employees.is_planner_available(event.event_planner, event.date, exclude_id, lock=True): raise ValueError("Event planner does not exist or is already booked on this date.")
            if event.dj is not None and not self._employees.is_dj_available(event.dj, event.date, exclude_id, lock=True): raise ValueError("DJ does not exist or is already booked on this date.")
            if event.location is not None and not self._location.is_location_available(event.location, event.date, exclude_id, lock=True): raise ValueError("Location does not exist or is already booked on this date.")
            if event.playlist is not None and not self._cd.is_playlist_available(event.playlist, event.date, exclude_id, lock=True): raise ValueError("Some CDs in the playlist do not have enough copies available.")

            new_id: int = self._events.save_event(event)

            if event.id is None: logger.info("Event created: id=%d name=%r date=%s", new_id, event.name, event.date)
            else: logger.info("Event updated: id=%d name=%r date=%s", event.id, event.name, event.date)

            return new_id


    def delete(self, event_id: int) -> None:
        with Transaction(self._db):
            event: Event | None = self._events.get_by_id(event_id, lock=True)
            if event is None: return
            if event.date <= Date.today(): raise ValueError("Cannot delete a past or ongoing event.")
            self._events.delete_event(event_id)
            logger.info("Event deleted: id=%d", event_id)


    def get_dashboard(self, filter: EventDashboardFilter = EventDashboardFilter()) -> list[EventDashboard]:
        return self._events.get_dashboard(filter)


    def get_all(self, filter: EventFilter = EventFilter()) -> list[Event]:
        return self._events.get_all(filter)
