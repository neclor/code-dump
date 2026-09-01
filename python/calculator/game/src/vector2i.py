"""Integer 2D vector for grid / pixel math (CPython + MicroPython / casioplot)."""

import math


class Vector2i:
    __slots__ = ("x", "y")

    ZERO: "Vector2i"
    ONE: "Vector2i"
    LEFT: "Vector2i"
    RIGHT: "Vector2i"
    UP: "Vector2i"
    DOWN: "Vector2i"

    def __init__(self, x: int = 0, y: int = 0) -> None:
        self.x: int = int(x)
        self.y: int = int(y)

    @classmethod
    def from_tuple(cls, tuple: "tuple[int, int]") -> "Vector2i": return cls(tuple[0], tuple[1])

    @classmethod
    def from_angle(cls, radians: float, length: float = 1.0) -> "Vector2i":
        return cls(round(math.cos(radians) * length), round(math.sin(radians) * length))

    def copy(self) -> "Vector2i": return Vector2i(self.x, self.y)

    def to_tuple(self) -> "tuple[int, int]": return (self.x, self.y)

    def __iter__(self):
        yield self.x
        yield self.y

    def __len__(self) -> int:
        return 2

    def __getitem__(self, i: int) -> int:
        if i == 0: return self.x
        if i == 1: return self.y
        raise IndexError("Vector2i index out of range")

    def __repr__(self) -> str:
        return "Vector2i({}, {})".format(self.x, self.y)

    def __str__(self) -> str:
        return "({}, {})".format(self.x, self.y)

    def __eq__(self, other: object) -> bool:
        if isinstance(other, Vector2i): return self.x == other.x and self.y == other.y
        return NotImplemented

    def __ne__(self, other: object) -> bool:
        result = self.__eq__(other)
        if result is NotImplemented: return result
        return not result

    def __hash__(self) -> int: return hash((self.x, self.y))

    def __bool__(self) -> bool: return self.x != 0 or self.y != 0

    def __add__(self, other: "Vector2i") -> "Vector2i": return Vector2i(self.x + other.x, self.y + other.y)

    __radd__ = __add__

    def __sub__(self, other: "Vector2i") -> "Vector2i":  return Vector2i(self.x - other.x, self.y - other.y)

    def __rsub__(self, other: "Vector2i") -> "Vector2i": return other - self

    def __neg__(self) -> "Vector2i": return Vector2i(-self.x, -self.y)

    def __pos__(self) -> "Vector2i": return Vector2i(self.x, self.y)

    def __abs__(self) -> "Vector2i": return Vector2i(abs(self.x), abs(self.y))

    def __mul__(self, other: "int | Vector2i") -> "Vector2i":
        if isinstance(other, Vector2i):  return Vector2i(self.x * other.x, self.y * other.y)
        return Vector2i(self.x * other, self.y * other)

    __rmul__ = __mul__

    def __floordiv__(self, other: "int | Vector2i") -> "Vector2i":
        if isinstance(other, Vector2i): return Vector2i(self.x // other.x, self.y // other.y)
        return Vector2i(self.x // other, self.y // other)

    __truediv__ = __floordiv__

    def __mod__(self, other: "int | Vector2i") -> "Vector2i":
        if isinstance(other, Vector2i): return Vector2i(self.x % other.x, self.y % other.y)
        return Vector2i(self.x % other, self.y % other)

    def dot(self, other: "Vector2i") -> int: return self.x * other.x + self.y * other.y

    def cross(self, other: "Vector2i") -> int: return self.x * other.y - self.y * other.x

    def length_squared(self) -> int: return self.x * self.x + self.y * self.y

    def length(self) -> float: return math.sqrt(self.x * self.x + self.y * self.y)

    def distance_squared_to(self, other: "Vector2i") -> int:
        dx = self.x - other.x
        dy = self.y - other.y
        return dx * dx + dy * dy

    def distance_to(self, other: "Vector2i") -> float: return math.sqrt(self.distance_squared_to(other))

    def sign(self) -> "Vector2i": return Vector2i(_sign(self.x), _sign(self.y))

    def angle(self) -> float: return math.atan2(self.y, self.x)

    def angle_to(self, other: "Vector2i") -> float: return math.atan2(self.cross(other), self.dot(other))

    def angle_to_point(self, other: "Vector2i") -> float: return math.atan2(other.y - self.y, other.x - self.x)

    def rotated_90(self) -> "Vector2i": return Vector2i(-self.y, self.x)

    def rotated_90_cw(self) -> "Vector2i": return Vector2i(self.y, -self.x)

    def rotated_180(self) -> "Vector2i": return Vector2i(-self.x, -self.y)

    def perpendicular(self) -> "Vector2i": return Vector2i(-self.y, self.x)

    def rotated(self, radians: float) -> "Vector2i":
        c = math.cos(radians)
        s = math.sin(radians)
        return Vector2i(round(self.x * c - self.y * s), round(self.x * s + self.y * c))

    def scalar_projection(self, onto: "Vector2i") -> float:
        n = onto.length()
        if n == 0.0: return 0.0
        return self.dot(onto) / n

    def project_onto(self, onto: "Vector2i") -> "Vector2i":
        """Component of self along `onto`, rounded back to integers."""
        d = onto.length_squared()
        if d == 0:
            return Vector2i(0, 0)
        k = self.dot(onto) / d
        return Vector2i(round(onto.x * k), round(onto.y * k))

    def reject_from(self, onto: "Vector2i") -> "Vector2i":
        """Component of self perpendicular to `onto`."""
        return self - self.project_onto(onto)

    def reflected(self, normal: "Vector2i") -> "Vector2i":
        """Reflect self across the line with the given normal."""
        d = normal.length_squared()
        if d == 0:
            return self.copy()
        k = 2 * self.dot(normal) / d
        return Vector2i(round(self.x - k * normal.x), round(self.y - k * normal.y))

    def bounce(self, normal: "Vector2i") -> "Vector2i":
        return -self.reflected(normal)

    # -- component-wise helpers -------------------------------------------------

    def min(self, other: "Vector2i") -> "Vector2i":
        return Vector2i(min(self.x, other.x), min(self.y, other.y))

    def max(self, other: "Vector2i") -> "Vector2i":
        return Vector2i(max(self.x, other.x), max(self.y, other.y))

    def clamped(self, lo: "Vector2i", hi: "Vector2i") -> "Vector2i":
        return Vector2i(_clamp(self.x, lo.x, hi.x), _clamp(self.y, lo.y, hi.y))

    def lerp(self, other: "Vector2i", t: float) -> "Vector2i":
        return Vector2i(round(self.x + (other.x - self.x) * t),
                        round(self.y + (other.y - self.y) * t))

    def midpoint(self, other: "Vector2i") -> "Vector2i":
        return Vector2i((self.x + other.x) // 2, (self.y + other.y) // 2)

    def aspect(self) -> float:
        return self.x / self.y


# -- module helpers -------------------------------------------------------------

def _sign(v: int) -> int:
    if v > 0:
        return 1
    if v < 0:
        return -1
    return 0


def _clamp(v: int, lo: int, hi: int) -> int:
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v


# -- constants (screen convention: Y grows downward) --------------------------

Vector2i.ZERO = Vector2i(0, 0)
Vector2i.ONE = Vector2i(1, 1)
Vector2i.LEFT = Vector2i(-1, 0)
Vector2i.RIGHT = Vector2i(1, 0)
Vector2i.UP = Vector2i(0, -1)
Vector2i.DOWN = Vector2i(0, 1)
