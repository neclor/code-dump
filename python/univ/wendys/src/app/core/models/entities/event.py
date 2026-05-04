from dataclasses import dataclass
from datetime import date as Date
from decimal import Decimal


@dataclass(frozen=True, slots=True)
class Event:
    id: int | None
    name: str
    date: Date
    client: int
    theme: str
    type: str
    description: str | None = None
    manager: int | None = None
    event_planner: int | None = None
    dj: int | None = None
    location: int | None = None
    rental_fee: Decimal | None = None
    playlist: str | None = None


    def __post_init__(self):
        if self.id is not None and self.id <= 0: raise ValueError("Id cannot be zero or negative")
        if not self.name.strip(): raise ValueError("name cannot be empty")
        if self.client <= 0: raise ValueError("Client number cannot be zero or negative")
        if not self.theme.strip(): raise ValueError("theme cannot be empty")
        if not self.type.strip(): raise ValueError("type cannot be empty")

        if self.description is not None and not self.description.strip(): raise ValueError("description should be None or not empty")

        if self.manager and self.manager <= 0: raise ValueError("Manager id cannot be zero or negative")
        if self.event_planner and self.event_planner <= 0: raise ValueError("Event planner id cannot be zero or negative")
        if self.dj and self.dj <= 0: raise ValueError("Dj id cannot be zero or negative")
        if self.location and self.location <= 0: raise ValueError("Location id cannot be zero or negative")
        if self.rental_fee is not None and self.rental_fee < 0: raise ValueError("Rental fee cannot be negative")
        
        if self.event_planner is not None and self.manager is None: raise ValueError("Cannot assign event_planner without a manager")
        if self.dj is not None and self.manager is None: raise ValueError("Cannot assign dj without a manager")
        if self.location is not None and self.event_planner is None: raise ValueError("Cannot assign location without an event_planner")
        if self.rental_fee is not None and self.location is None: raise ValueError("Cannot assign rental_fee without a location")
