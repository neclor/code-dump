"""Vector2i - port of Godot 4's Vector2i. Immutable integer 2D vector.

Python cannot copy objects on assignment the way GDScript copies structs, so
this type is immutable: every operation returns a new Vector2i. Use with_x() /
with_y() to get a changed copy.
"""

from __future__ import annotations

import math



from core.math.utils import *

from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from core.math.vectors.vector2 import Vector2


class Vector2i:
    __slots__ = ("x", "y")

    x: int
    y: int

    AXIS_X: int = 0
    AXIS_Y: int = 1

    ZERO: Vector2i
    ONE: Vector2i
    MIN: Vector2i
    MAX: Vector2i
    LEFT: Vector2i
    RIGHT: Vector2i
    UP: Vector2i
    DOWN: Vector2i

    def __init__(
        self,
        x: Vector2i | Vector2 | tuple[int, int] | list[int] | int = 0,
        y: int = 0,
    ) -> None:
        if isinstance(x, (int, float)):
            ix, iy = int(x), int(y)
        elif isinstance(x, (tuple, list)):
            ix, iy = int(x[0]), int(x[1])
        else:
            ix, iy = int(x.x), int(x.y)
        object.__setattr__(self, "x", ix)
        object.__setattr__(self, "y", iy)

    def __setattr__(self, name: str, value: object) -> None:
        raise AttributeError("Vector2i is immutable; use with_x()/with_y()")

    def __delattr__(self, name: str) -> None:
        raise AttributeError("Vector2i is immutable")

    # -- copy helpers (not in Godot, convenient in Python) ------------------

    def with_x(self, x: int) -> Vector2i: return Vector2i(x, self.y)
    def with_y(self, y: int) -> Vector2i: return Vector2i(self.x, y)
    def copy(self) -> Vector2i: return Vector2i(self.x, self.y)

    # -- Godot methods -----------------------------------------------------

    def abs(self) -> Vector2i:
        return Vector2i(abs(self.x), abs(self.y))

    def aspect(self) -> float:
        if self.y == 0:
            return float("inf") if self.x > 0 else (float("-inf") if self.x < 0 else float("nan"))
        return self.x / self.y

    def clamp(self, min: Vector2i, max: Vector2i) -> Vector2i:
        return Vector2i(clampi(self.x, min.x, max.x), clampi(self.y, min.y, max.y))

    def clampi(self, min: int, max: int) -> Vector2i:
        return Vector2i(clampi(self.x, min, max), clampi(self.y, min, max))

    def distance_squared_to(self, to: Vector2i) -> int:
        dx = to.x - self.x
        dy = to.y - self.y
        return dx * dx + dy * dy

    def distance_to(self, to: Vector2i) -> float:
        return math.sqrt(self.distance_squared_to(to))

    def length(self) -> float:
        return math.sqrt(self.x * self.x + self.y * self.y)

    def length_squared(self) -> int:
        return self.x * self.x + self.y * self.y

    def max(self, with_: Vector2i) -> Vector2i:
        return Vector2i(max(self.x, with_.x), max(self.y, with_.y))

    def maxi(self, with_: int) -> Vector2i:
        return Vector2i(max(self.x, with_), max(self.y, with_))

    def max_axis_index(self) -> int:
        return Vector2i.AXIS_Y if self.x < self.y else Vector2i.AXIS_X

    def min(self, with_: Vector2i) -> Vector2i:
        return Vector2i(min(self.x, with_.x), min(self.y, with_.y))

    def mini(self, with_: int) -> Vector2i:
        return Vector2i(min(self.x, with_), min(self.y, with_))

    def min_axis_index(self) -> int:
        return Vector2i.AXIS_X if self.x < self.y else Vector2i.AXIS_Y

    def sign(self) -> Vector2i:
        return Vector2i(signi(self.x), signi(self.y))

    def snapped(self, step: Vector2i) -> Vector2i:
        return Vector2i(snappedi(self.x, step.x), snappedi(self.y, step.y))

    def snappedi(self, step: int) -> Vector2i:
        return Vector2i(snappedi(self.x, step), snappedi(self.y, step))

    # -- operators -------------------------------------------------------------

    def __add__(self, o: Vector2i) -> Vector2i:
        return Vector2i(self.x + o.x, self.y + o.y)

    def __sub__(self, o: Vector2i) -> Vector2i:
        return Vector2i(self.x - o.x, self.y - o.y)

    def __mul__(self, o: Vector2i | int) -> Vector2i:
        if isinstance(o, Vector2i):
            return Vector2i(self.x * o.x, self.y * o.y)
        return Vector2i(self.x * o, self.y * o)

    def __rmul__(self, o: int) -> Vector2i:
        return Vector2i(self.x * o, self.y * o)

    def __truediv__(self, o: Vector2i | int) -> Vector2i:
        if isinstance(o, Vector2i):
            return Vector2i(idiv(self.x, o.x), idiv(self.y, o.y))
        return Vector2i(idiv(self.x, o), idiv(self.y, o))

    __floordiv__ = __truediv__

    def __mod__(self, o: Vector2i | int) -> Vector2i:
        if isinstance(o, Vector2i):
            return Vector2i(imod(self.x, o.x), imod(self.y, o.y))
        return Vector2i(imod(self.x, o), imod(self.y, o))

    def __neg__(self) -> Vector2i: return Vector2i(-self.x, -self.y)
    def __pos__(self) -> Vector2i: return Vector2i(self.x, self.y)
    def __abs__(self) -> Vector2i: return Vector2i(abs(self.x), abs(self.y))

    def __eq__(self, o: object) -> bool:
        return isinstance(o, Vector2i) and self.x == o.x and self.y == o.y

    def __ne__(self, o: object) -> bool:
        return not (isinstance(o, Vector2i) and self.x == o.x and self.y == o.y)

    def __hash__(self) -> int:
        return hash((self.x, self.y))

    def __lt__(self, o: Vector2i) -> bool:
        return self.y < o.y if self.x == o.x else self.x < o.x

    def __le__(self, o: Vector2i) -> bool:
        return self.y <= o.y if self.x == o.x else self.x < o.x

    def __gt__(self, o: Vector2i) -> bool:
        return self.y > o.y if self.x == o.x else self.x > o.x

    def __ge__(self, o: Vector2i) -> bool:
        return self.y >= o.y if self.x == o.x else self.x > o.x

    def __getitem__(self, i: int) -> int:
        if i == 0:
            return self.x
        if i == 1:
            return self.y
        raise IndexError("Vector2i index out of range")

    def __len__(self) -> int:
        return 2

    def __iter__(self) -> Iterator[int]:
        yield self.x
        yield self.y

    def __bool__(self) -> bool:
        return self.x != 0 or self.y != 0

    def __repr__(self) -> str:
        return "Vector2i({}, {})".format(self.x, self.y)

    def __str__(self) -> str:
        return "({}, {})".format(self.x, self.y)


Vector2i.ZERO = Vector2i(0, 0)
Vector2i.ONE = Vector2i(1, 1)
Vector2i.MIN = Vector2i(INT_MIN, INT_MIN)
Vector2i.MAX = Vector2i(INT_MAX, INT_MAX)
Vector2i.LEFT = Vector2i(-1, 0)
Vector2i.RIGHT = Vector2i(1, 0)
Vector2i.UP = Vector2i(0, -1)
Vector2i.DOWN = Vector2i(0, 1)
