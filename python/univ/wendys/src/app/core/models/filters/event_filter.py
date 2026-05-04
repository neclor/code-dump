from dataclasses import dataclass
from datetime import date as Date
from decimal import Decimal
from typing import Literal

from app.core.models.filters.sort_order import SortOrder


EventColumn = Literal[
    "id",
    "name",
    "date",
    "description",
    "client",
    "manager",
    "event_planner",
    "dj",
    "theme",
    "type",
    "location",
    "rental_fee",
    "playlist"
]


@dataclass(slots=True)
class EventFilter:
    id: int | None = None
    name: str | None = None
    date: Date | None = None
    description: str | None = None
    client: int | None = None
    manager: int | None = None
    event_planner: int | None = None
    dj: int | None = None
    theme: str | None = None
    type: str | None = None
    location: int | None = None
    rental_fee: Decimal | None = None
    playlist: str | None = None
    sort_column: EventColumn = "date"
    order: SortOrder = "desc"
