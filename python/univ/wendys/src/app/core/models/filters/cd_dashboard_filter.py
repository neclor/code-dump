from dataclasses import dataclass
from typing import Literal

from app.core.models.filters.sort_order import SortOrder


CdDashboardColumn = Literal[
    "date",
    "cd_number",
    "copies",
    "copies_used",
]


@dataclass(slots=True)
class CdDashboardFilter:
    sort_column: CdDashboardColumn = "date"
    order: SortOrder = "desc"
