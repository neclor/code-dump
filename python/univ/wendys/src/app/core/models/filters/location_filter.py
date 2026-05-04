from dataclasses import dataclass
from datetime import date as Date
from decimal import Decimal
from typing import Literal

from app.core.models.filters.sort_order import SortOrder


LocationColumn = Literal[
    "id",
    "street",
    "city",
    "postal_code",
    "country",
    "comment"
]


@dataclass(slots=True)
class LocationFilter:
    id: int | None = None
    street: str | None = None
    city: str | None= None
    postal_code: str | None = None
    country: str | None = None
    comment: str | None = None
    sort_column: LocationColumn = "id"
    order: SortOrder = "asc"
