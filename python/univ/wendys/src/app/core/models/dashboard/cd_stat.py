from dataclasses import dataclass
from datetime import timedelta as TimeDelta


@dataclass(frozen=True, slots=True)
class CdStat:
    cd_number: int
    total_duration: TimeDelta
    min_duration: TimeDelta
    max_duration: TimeDelta
    average_duration: TimeDelta
    playlist_count: int
    genres: str | None

