from dataclasses import dataclass


@dataclass(frozen=True, slots=True)
class Genre:
    genre: str
    subgenre: str | None


    def __post_init__(self):
        if not self.genre.strip(): raise ValueError("genre cannot be empty")
        if self.subgenre is not None and not self.subgenre.strip(): raise ValueError("subgenre cannot be empty")
