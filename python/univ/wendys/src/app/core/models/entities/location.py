from dataclasses import dataclass


@dataclass(frozen=True, slots=True)
class Location:
    id: int
    street: str
    city: str
    country: str
    postal_code: str | None = None
    comment: str | None = None


    def __post_init__(self):
        if self.id <= 0: raise ValueError("id cannot be zero or negative")
        if not self.street.strip(): raise ValueError("street cannot be empty")
        if not self.city.strip(): raise ValueError("city cannot be empty")
        if not self.country.strip(): raise ValueError("country cannot be empty")

        if self.postal_code is not None and not self.postal_code.strip().isdigit(): raise ValueError("postal code must be a number")
        if self.comment is not None and not self.comment.strip(): raise ValueError("comment cannot be empty")
