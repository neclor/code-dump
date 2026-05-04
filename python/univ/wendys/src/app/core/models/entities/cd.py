from dataclasses import dataclass


@dataclass(frozen=True, slots=True)
class Cd:
    cd_number: int | None 
    title: str
    producer: str
    year: int
    copies: int


    def __post_init__(self):
        if self.cd_number is not None and self.cd_number <= 0: raise ValueError("cd_number cannot be zero or negative")
        if not self.title.strip(): raise ValueError("title cannot be empty")
        if not self.producer.strip(): raise ValueError("producer cannot be empty")
        if self.year < 1982: raise ValueError("year must be >= 1982")
        if self.copies < 0: raise ValueError("copies cannot be negative")
