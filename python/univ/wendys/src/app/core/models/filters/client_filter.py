from dataclasses import dataclass
from typing import Literal

from app.core.models.filters.sort_order import SortOrder


ClientColumn = Literal[
    "client_number",
    "first_name",
    "last_name",
    "email_address",
    "phone_number"
]


@dataclass(slots=True)
class ClientFilter:
    client_number: int | None = None
    first_name: str | None = None
    last_name: str | None = None
    email_address: str | None = None
    phone_number: str | None = None
    sort_column: ClientColumn = "client_number"
    order: SortOrder = "asc"
