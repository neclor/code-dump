"""Vector3i - port of Godot 4's Vector3i. Immutable integer 3D vector.

Python cannot copy objects on assignment the way GDScript copies structs, so
this type is immutable: every operation returns a new Vector3i. Use with_x() /
with_y() / with_z() to get a changed copy.
"""

from __future__ import annotations

import math

from typing import TYPE_CHECKING

from core.math.utils import *


if TYPE_CHECKING:
    from collections.abc import Iterator

    from core.math.vectors.vector3 import Vector3


class Vector3i:
    __slots__ = ("x", "y", "z")

    x: int
    y: int
    z: int

    AXIS_X: int = 0
    AXIS_Y: int = 1
    AXIS_Z: int = 2

    ZERO: Vector3i
    ONE: Vector3i
    MIN: Vector3i
    MAX: Vector3i
    LEFT: Vector3i
    RIGHT: Vector3i
    UP: Vector3i
    DOWN: Vector3i
    FORWARD: Vector3i
    BACK: Vector3i

    def __init__(
        self,
        x: Vector3i | Vector3 | tuple[int, int, int] | list[int] | int = 0,
        y: int = 0,
        z: int = 0,
    ) -> None:
        if isinstance(x, (int, float)):
            ix, iy, iz = int(x), int(y), int(z)
        elif isinstance(x, (tuple, list)):
            ix, iy, iz = int(x[0]), int(x[1]), int(x[2])
        else:
            ix, iy, iz = int(x.x), int(x.y), int(x.z)
        object.__setattr__(self, "x", ix)
        object.__setattr__(self, "y", iy)
        object.__setattr__(self, "z", iz)

    def __setattr__(self, name: str, value: object) -> None:
        raise AttributeError("Vector3i is immutable; use with_x()/with_y()/with_z()")

    def __delattr__(self, name: str) -> None:
        raise AttributeError("Vector3i is immutable")

    # -- copy helpers --------------------------------------------------------

    def with_x(self, x: int) -> Vector3i: return Vector3i(x, self.y, self.z)
    def with_y(self, y: int) -> Vector3i: return Vector3i(self.x, y, self.z)
    def with_z(self, z: int) -> Vector3i: return Vector3i(self.x, self.y, z)
    def copy(self) -> Vector3i: return Vector3i(self.x, self.y, self.z)

    # -- Godot methods -----------------------------------------------------

    def abs(self) -> Vector3i:
        return Vector3i(abs(self.x), abs(self.y), abs(self.z))

    def clamp(self, min: Vector3i, max: Vector3i) -> Vector3i:
        return Vector3i(clampi(self.x, min.x, max.x),
                        clampi(self.y, min.y, max.y),
                        clampi(self.z, min.z, max.z))

    def clampi(self, min: int, max: int) -> Vector3i:
        return Vector3i(clampi(self.x, min, max),
                        clampi(self.y, min, max),
                        clampi(self.z, min, max))

    def distance_squared_to(self, to: Vector3i) -> int:
        dx = to.x - self.x
        dy = to.y - self.y
        dz = to.z - self.z
        return dx * dx + dy * dy + dz * dz

    def distance_to(self, to: Vector3i) -> float:
        return math.sqrt(self.distance_squared_to(to))

    def length(self) -> float:
        return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)

    def length_squared(self) -> int:
        return self.x * self.x + self.y * self.y + self.z * self.z

    def max(self, w: Vector3i) -> Vector3i:
        return Vector3i(max(self.x, w.x), max(self.y, w.y), max(self.z, w.z))

    def maxi(self, w: int) -> Vector3i:
        return Vector3i(max(self.x, w), max(self.y, w), max(self.z, w))

    def max_axis_index(self) -> int:
        if self.x < self.y:
            return Vector3i.AXIS_Z if self.y < self.z else Vector3i.AXIS_Y
        return Vector3i.AXIS_Z if self.x < self.z else Vector3i.AXIS_X

    def min(self, w: Vector3i) -> Vector3i:
        return Vector3i(min(self.x, w.x), min(self.y, w.y), min(self.z, w.z))

    def mini(self, w: int) -> Vector3i:
        return Vector3i(min(self.x, w), min(self.y, w), min(self.z, w))

    def min_axis_index(self) -> int:
        if self.x < self.y:
            return Vector3i.AXIS_X if self.x < self.z else Vector3i.AXIS_Z
        return Vector3i.AXIS_Y if self.y < self.z else Vector3i.AXIS_Z

    def sign(self) -> Vector3i:
        return Vector3i(signi(self.x), signi(self.y), signi(self.z))

    def snapped(self, step: Vector3i) -> Vector3i:
        return Vector3i(snappedi(self.x, step.x), snappedi(self.y, step.y), snappedi(self.z, step.z))

    def snappedi(self, step: int) -> Vector3i:
        return Vector3i(snappedi(self.x, step), snappedi(self.y, step), snappedi(self.z, step))

    # -- operators -------------------------------------------------------------

    def __add__(self, o: Vector3i) -> Vector3i:
        return Vector3i(self.x + o.x, self.y + o.y, self.z + o.z)

    def __sub__(self, o: Vector3i) -> Vector3i:
        return Vector3i(self.x - o.x, self.y - o.y, self.z - o.z)

    def __mul__(self, o: Vector3i | int) -> Vector3i:
        if isinstance(o, Vector3i):
            return Vector3i(self.x * o.x, self.y * o.y, self.z * o.z)
        return Vector3i(self.x * o, self.y * o, self.z * o)

    def __rmul__(self, o: int) -> Vector3i:
        return Vector3i(self.x * o, self.y * o, self.z * o)

    def __truediv__(self, o: Vector3i | int) -> Vector3i:
        if isinstance(o, Vector3i):
            return Vector3i(idiv(self.x, o.x), idiv(self.y, o.y), idiv(self.z, o.z))
        return Vector3i(idiv(self.x, o), idiv(self.y, o), idiv(self.z, o))

    __floordiv__ = __truediv__

    def __mod__(self, o: Vector3i | int) -> Vector3i:
        if isinstance(o, Vector3i):
            return Vector3i(imod(self.x, o.x), imod(self.y, o.y), imod(self.z, o.z))
        return Vector3i(imod(self.x, o), imod(self.y, o), imod(self.z, o))

    def __neg__(self) -> Vector3i: return Vector3i(-self.x, -self.y, -self.z)
    def __pos__(self) -> Vector3i: return Vector3i(self.x, self.y, self.z)
    def __abs__(self) -> Vector3i: return Vector3i(abs(self.x), abs(self.y), abs(self.z))

    def __eq__(self, o: object) -> bool:
        return (isinstance(o, Vector3i)
                and self.x == o.x and self.y == o.y and self.z == o.z)

    def __ne__(self, o: object) -> bool:
        return not (isinstance(o, Vector3i)
                    and self.x == o.x and self.y == o.y and self.z == o.z)

    def __hash__(self) -> int:
        return hash((self.x, self.y, self.z))

    def __lt__(self, o: Vector3i) -> bool:
        if self.x == o.x:
            return self.z < o.z if self.y == o.y else self.y < o.y
        return self.x < o.x

    def __le__(self, o: Vector3i) -> bool:
        if self.x == o.x:
            if self.y == o.y:
                return self.z <= o.z
            return self.y < o.y
        return self.x < o.x

    def __gt__(self, o: Vector3i) -> bool:
        if self.x == o.x:
            return self.z > o.z if self.y == o.y else self.y > o.y
        return self.x > o.x

    def __ge__(self, o: Vector3i) -> bool:
        if self.x == o.x:
            if self.y == o.y:
                return self.z >= o.z
            return self.y > o.y
        return self.x > o.x

    def __getitem__(self, i: int) -> int:
        if i == 0:
            return self.x
        if i == 1:
            return self.y
        if i == 2:
            return self.z
        raise IndexError("Vector3i index out of range")

    def __len__(self) -> int:
        return 3

    def __iter__(self) -> Iterator[int]:
        yield self.x
        yield self.y
        yield self.z

    def __bool__(self) -> bool:
        return self.x != 0 or self.y != 0 or self.z != 0

    def __repr__(self) -> str:
        return "Vector3i({}, {}, {})".format(self.x, self.y, self.z)

    def __str__(self) -> str:
        return "({}, {}, {})".format(self.x, self.y, self.z)


Vector3i.ZERO = Vector3i(0, 0, 0)
Vector3i.ONE = Vector3i(1, 1, 1)
Vector3i.MIN = Vector3i(INT_MIN, INT_MIN, INT_MIN)
Vector3i.MAX = Vector3i(INT_MAX, INT_MAX, INT_MAX)
Vector3i.LEFT = Vector3i(-1, 0, 0)
Vector3i.RIGHT = Vector3i(1, 0, 0)
Vector3i.UP = Vector3i(0, 1, 0)
Vector3i.DOWN = Vector3i(0, -1, 0)
Vector3i.FORWARD = Vector3i(0, 0, -1)
Vector3i.BACK = Vector3i(0, 0, 1)
