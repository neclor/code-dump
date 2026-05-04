from dataclasses import dataclass
from datetime import time as Time


@dataclass(frozen=True, slots=True)
class Song:
    cd_number: int
    track_number: int
    title: str
    artist: str
    duration: Time
    genre: str


    def __post_init__(self):
        if self.cd_number <= 0: raise ValueError("cd_number cannot be zero or negative")
        if self.track_number <= 0: raise ValueError("track_number cannot be zero or negative")
        if not self.title.strip(): raise ValueError("title cannot be empty")
        if not self.artist.strip(): raise ValueError("artist cannot be empty")
        if not self.duration: raise ValueError("duration must be greater than 0")
        if not self.genre.strip(): raise ValueError("genre cannot be empty")
