from dataclasses import dataclass
from datetime import date as Date


@dataclass(frozen=True, slots=True)
class CdDashboard:
    date: Date
    cd_number: int | None
    copies: int | None
    copies_used: int | None
