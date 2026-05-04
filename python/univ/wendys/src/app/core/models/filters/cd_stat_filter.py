from dataclasses import dataclass
from typing import Literal

from app.core.models.filters.sort_order import SortOrder


CdStatColumn = Literal[
    "cd_number",
    "total_duration",
    "min_duration",
    "max_duration",
    "average_duration",
    "playlist_count",
    "genres"
]


@dataclass(slots=True)
class CdStatFilter:
    sort_column: CdStatColumn = "cd_number"
    order: SortOrder = "asc"
