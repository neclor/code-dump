from dataclasses import dataclass


@dataclass(frozen=True, slots=True)
class GenreDashboard:
    genre: str
    subgenres: str | None


    def __post_init__(self):
        if not self.genre.strip(): raise ValueError("genre cannot be empty")
        if self.subgenres is not None and not self.subgenres.strip(): raise ValueError("subgenres cannot be empty")
