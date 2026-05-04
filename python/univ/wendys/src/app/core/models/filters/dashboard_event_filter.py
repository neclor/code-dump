from dataclasses import dataclass
from typing import Literal

from app.core.models.filters.sort_order import SortOrder


EventDashboardColumn = Literal[
    "id",
    "name",
    "date",
    "status",
    "nb_request",
    "cost",
]


@dataclass(slots=True)
class EventDashboardFilter:
    sort_column: EventDashboardColumn = "date"
    order: SortOrder = "desc"
