from dataclasses import dataclass


@dataclass(frozen=True, slots=True)
class Client:
    client_number: int
    first_name: str
    last_name: str
    email_address: str
    phone_number: str | None = None


    def __post_init__(self):
        if self.client_number <= 0: raise ValueError("client_number cannot be zero or negative")
        if not self.first_name.strip(): raise ValueError("first_name cannot be empty")
        if not self.last_name.strip(): raise ValueError("last_name cannot be empty")
        if "@" not in self.email_address or "." not in self.email_address.split("@")[-1]: raise ValueError(f"Invalid email address: {self.email_address}")

        if self.phone_number is not None and not self.phone_number.strip(): raise ValueError("phone_number cannot be empty")
