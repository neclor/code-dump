from dataclasses import dataclass
from datetime import date as Date
from decimal import Decimal


@dataclass(frozen=True, slots=True)
class EventDashboard:
    id: int
    name: str
    date: Date
    status: str
    nb_request: int
    cost: Decimal
