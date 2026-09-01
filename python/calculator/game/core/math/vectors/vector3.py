"""Vector3 - port of Godot 4's Vector3 (float). Immutable value type.

Python cannot copy objects on assignment the way GDScript copies structs, so
this type is immutable: every operation returns a new Vector3. Use with_x() /
with_y() / with_z() to get a changed copy.

`outer()` returns a 3-tuple of Vector3 rows instead of a Basis (not ported).
"""

from __future__ import annotations

import math

from typing import TYPE_CHECKING

from core.math.utils import *


if TYPE_CHECKING:
    from collections.abc import Iterator

    from core.math.vectors.vector2 import Vector2
    from core.math.vectors.vector3i import Vector3i


class Vector3:
    __slots__ = ("x", "y", "z")

    x: float
    y: float
    z: float

    AXIS_X: int = 0
    AXIS_Y: int = 1
    AXIS_Z: int = 2

    ZERO: Vector3
    ONE: Vector3
    INF: Vector3
    LEFT: Vector3
    RIGHT: Vector3
    UP: Vector3
    DOWN: Vector3
    FORWARD: Vector3
    BACK: Vector3
    MODEL_LEFT: Vector3
    MODEL_RIGHT: Vector3
    MODEL_TOP: Vector3
    MODEL_BOTTOM: Vector3
    MODEL_FRONT: Vector3
    MODEL_REAR: Vector3

    def __init__(
        self,
        x: Vector3 | Vector3i | tuple[float, float, float] | list[float] | float = 0.0,
        y: float = 0.0,
        z: float = 0.0,
    ) -> None:
        if isinstance(x, (int, float)):
            fx, fy, fz = float(x), float(y), float(z)
        elif isinstance(x, (tuple, list)):
            fx, fy, fz = float(x[0]), float(x[1]), float(x[2])
        else:
            fx, fy, fz = float(x.x), float(x.y), float(x.z)
        object.__setattr__(self, "x", fx)
        object.__setattr__(self, "y", fy)
        object.__setattr__(self, "z", fz)

    def __setattr__(self, name: str, value: object) -> None:
        raise AttributeError("Vector3 is immutable; use with_x()/with_y()/with_z()")

    def __delattr__(self, name: str) -> None:
        raise AttributeError("Vector3 is immutable")

    # -- copy helpers ----------------------------------------------------------

    def with_x(self, x: float) -> Vector3: return Vector3(x, self.y, self.z)
    def with_y(self, y: float) -> Vector3: return Vector3(self.x, y, self.z)
    def with_z(self, z: float) -> Vector3: return Vector3(self.x, self.y, z)
    def copy(self) -> Vector3: return Vector3(self.x, self.y, self.z)

    # -- Godot methods -----------------------------------------------------

    def abs(self) -> Vector3:
        return Vector3(abs(self.x), abs(self.y), abs(self.z))

    def angle_to(self, to: Vector3) -> float:
        return math.atan2(self.cross(to).length(), self.dot(to))

    def bezier_derivative(self, control_1: Vector3, control_2: Vector3, end: Vector3, t: float) -> Vector3:
        omt = 1.0 - t
        omt2 = omt * omt
        t2 = t * t
        return (self * (-3.0 * omt2)
                + control_1 * (3.0 * omt2 - 6.0 * omt * t)
                + control_2 * (6.0 * omt * t - 3.0 * t2)
                + end * (3.0 * t2))

    def bezier_interpolate(self, control_1: Vector3, control_2: Vector3, end: Vector3, t: float) -> Vector3:
        omt = 1.0 - t
        omt2 = omt * omt
        omt3 = omt2 * omt
        t2 = t * t
        t3 = t2 * t
        return (self * omt3
                + control_1 * (omt2 * t * 3.0)
                + control_2 * (omt * t2 * 3.0)
                + end * t3)

    def bounce(self, n: Vector3) -> Vector3:
        return -self.reflect(n)

    def ceil(self) -> Vector3:
        return Vector3(math.ceil(self.x), math.ceil(self.y), math.ceil(self.z))

    def clamp(self, min: Vector3, max: Vector3) -> Vector3:
        return Vector3(clampf(self.x, min.x, max.x),
                       clampf(self.y, min.y, max.y),
                       clampf(self.z, min.z, max.z))

    def clampf(self, min: float, max: float) -> Vector3:
        return Vector3(clampf(self.x, min, max), clampf(self.y, min, max), clampf(self.z, min, max))

    def cross(self, w: Vector3) -> Vector3:
        return Vector3(self.y * w.z - self.z * w.y,
                       self.z * w.x - self.x * w.z,
                       self.x * w.y - self.y * w.x)

    def cubic_interpolate(self, b: Vector3, pre_a: Vector3, post_b: Vector3, weight: float) -> Vector3:
        return Vector3(cubic_interpolate(self.x, b.x, pre_a.x, post_b.x, weight),
                       cubic_interpolate(self.y, b.y, pre_a.y, post_b.y, weight),
                       cubic_interpolate(self.z, b.z, pre_a.z, post_b.z, weight))

    def cubic_interpolate_in_time(
        self,
        b: Vector3,
        pre_a: Vector3,
        post_b: Vector3,
        weight: float,
        b_t: float,
        pre_a_t: float,
        post_b_t: float,
    ) -> Vector3:
        return Vector3(
            cubic_interpolate_in_time(self.x, b.x, pre_a.x, post_b.x, weight, b_t, pre_a_t, post_b_t),
            cubic_interpolate_in_time(self.y, b.y, pre_a.y, post_b.y, weight, b_t, pre_a_t, post_b_t),
            cubic_interpolate_in_time(self.z, b.z, pre_a.z, post_b.z, weight, b_t, pre_a_t, post_b_t))

    def direction_to(self, to: Vector3) -> Vector3:
        return Vector3(to.x - self.x, to.y - self.y, to.z - self.z).normalized()

    def distance_squared_to(self, to: Vector3) -> float:
        dx = to.x - self.x
        dy = to.y - self.y
        dz = to.z - self.z
        return dx * dx + dy * dy + dz * dz

    def distance_to(self, to: Vector3) -> float:
        return math.sqrt(self.distance_squared_to(to))

    def dot(self, w: Vector3) -> float:
        return self.x * w.x + self.y * w.y + self.z * w.z

    def floor(self) -> Vector3:
        return Vector3(math.floor(self.x), math.floor(self.y), math.floor(self.z))

    def inverse(self) -> Vector3:
        return Vector3(inverse(self.x), inverse(self.y), inverse(self.z))

    def is_equal_approx(self, to: Vector3) -> bool:
        return (is_equal_approx(self.x, to.x)
                and is_equal_approx(self.y, to.y)
                and is_equal_approx(self.z, to.z))

    def is_finite(self) -> bool:
        return math.isfinite(self.x) and math.isfinite(self.y) and math.isfinite(self.z)

    def is_normalized(self) -> bool:
        return abs(self.length_squared() - 1.0) < UNIT_EPSILON

    def is_zero_approx(self) -> bool:
        return is_zero_approx(self.x) and is_zero_approx(self.y) and is_zero_approx(self.z)

    def length(self) -> float:
        return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)

    def length_squared(self) -> float:
        return self.x * self.x + self.y * self.y + self.z * self.z

    def lerp(self, to: Vector3, weight: float) -> Vector3:
        return Vector3(lerp(self.x, to.x, weight),
                       lerp(self.y, to.y, weight),
                       lerp(self.z, to.z, weight))

    def limit_length(self, length: float = 1.0) -> Vector3:
        l = self.length()
        v: Vector3 = self
        if l > 0.0 and length < l:
            v = v / l * length
        return Vector3(v.x, v.y, v.z)

    def max(self, w: Vector3) -> Vector3:
        return Vector3(max(self.x, w.x), max(self.y, w.y), max(self.z, w.z))

    def maxf(self, w: float) -> Vector3:
        return Vector3(max(self.x, w), max(self.y, w), max(self.z, w))

    def max_axis_index(self) -> int:
        if self.x < self.y:
            return Vector3.AXIS_Z if self.y < self.z else Vector3.AXIS_Y
        return Vector3.AXIS_Z if self.x < self.z else Vector3.AXIS_X

    def min(self, w: Vector3) -> Vector3:
        return Vector3(min(self.x, w.x), min(self.y, w.y), min(self.z, w.z))

    def minf(self, w: float) -> Vector3:
        return Vector3(min(self.x, w), min(self.y, w), min(self.z, w))

    def min_axis_index(self) -> int:
        if self.x < self.y:
            return Vector3.AXIS_X if self.x < self.z else Vector3.AXIS_Z
        return Vector3.AXIS_Y if self.y < self.z else Vector3.AXIS_Z

    def move_toward(self, to: Vector3, delta: float) -> Vector3:
        diff = Vector3(to.x - self.x, to.y - self.y, to.z - self.z)
        length = diff.length()
        if length <= delta or length < CMP_EPSILON:
            return Vector3(to.x, to.y, to.z)
        return self + diff / length * delta

    def normalized(self) -> Vector3:
        l = self.length()
        if l == 0.0:
            return Vector3(0.0, 0.0, 0.0)
        return Vector3(self.x / l, self.y / l, self.z / l)

    def octahedron_encode(self) -> Vector2:
        from core.math.vectors.vector2 import Vector2

        n = self / (abs(self.x) + abs(self.y) + abs(self.z))
        if n.z >= 0.0:
            ox, oy = n.x, n.y
        else:
            ox = (1.0 - abs(n.y)) * (1.0 if n.x >= 0.0 else -1.0)
            oy = (1.0 - abs(n.x)) * (1.0 if n.y >= 0.0 else -1.0)
        return Vector2(ox * 0.5 + 0.5, oy * 0.5 + 0.5)

    @staticmethod
    def octahedron_decode(uv: Vector2) -> Vector3:
        fx = uv.x * 2.0 - 1.0
        fy = uv.y * 2.0 - 1.0
        n = Vector3(fx, fy, 1.0 - abs(fx) - abs(fy))
        t = clampf(-n.z, 0.0, 1.0)
        n = Vector3(n.x + (-t if n.x >= 0.0 else t),
                    n.y + (-t if n.y >= 0.0 else t),
                    n.z)
        return n.normalized()

    def outer(self, w: Vector3) -> tuple[Vector3, Vector3, Vector3]:
        return (Vector3(self.x * w.x, self.x * w.y, self.x * w.z),
                Vector3(self.y * w.x, self.y * w.y, self.y * w.z),
                Vector3(self.z * w.x, self.z * w.y, self.z * w.z))

    def posmod(self, mod: float) -> Vector3:
        return Vector3(fposmod(self.x, mod), fposmod(self.y, mod), fposmod(self.z, mod))

    def posmodv(self, modv: Vector3) -> Vector3:
        return Vector3(fposmod(self.x, modv.x), fposmod(self.y, modv.y), fposmod(self.z, modv.z))

    def project(self, b: Vector3) -> Vector3:
        return b * (self.dot(b) / b.length_squared())

    def reflect(self, n: Vector3) -> Vector3:
        return 2.0 * n * self.dot(n) - self

    def rotated(self, axis: Vector3, angle: float) -> Vector3:
        c = math.cos(angle)
        s = math.sin(angle)
        return (self * c
                + axis.cross(self) * s
                + axis * (axis.dot(self) * (1.0 - c)))

    def round(self) -> Vector3:
        return Vector3(round_half_away(self.x), round_half_away(self.y), round_half_away(self.z))

    def sign(self) -> Vector3:
        return Vector3(signf(self.x), signf(self.y), signf(self.z))

    def signed_angle_to(self, to: Vector3, axis: Vector3) -> float:
        cross_to = self.cross(to)
        unsigned_angle = math.atan2(cross_to.length(), self.dot(to))
        return -unsigned_angle if cross_to.dot(axis) < 0.0 else unsigned_angle

    def slerp(self, to: Vector3, weight: float) -> Vector3:
        start_length_sq = self.length_squared()
        end_length_sq = to.length_squared()
        if start_length_sq == 0.0 or end_length_sq == 0.0:
            return self.lerp(to, weight)
        axis = self.cross(to)
        axis_length_sq = axis.length_squared()
        if axis_length_sq == 0.0:
            return self.lerp(to, weight)
        axis = axis / math.sqrt(axis_length_sq)
        start_length = math.sqrt(start_length_sq)
        result_length = lerp(start_length, math.sqrt(end_length_sq), weight)
        angle = self.angle_to(to)
        return self.rotated(axis, angle * weight) * (result_length / start_length)

    def slide(self, n: Vector3) -> Vector3:
        return self - n * self.dot(n)

    def snapped(self, step: Vector3) -> Vector3:
        return Vector3(snapped(self.x, step.x), snapped(self.y, step.y), snapped(self.z, step.z))

    def snappedf(self, step: float) -> Vector3:
        return Vector3(snapped(self.x, step), snapped(self.y, step), snapped(self.z, step))

    # -- operators -------------------------------------------------------------

    def __add__(self, o: Vector3) -> Vector3:
        return Vector3(self.x + o.x, self.y + o.y, self.z + o.z)

    def __sub__(self, o: Vector3) -> Vector3:
        return Vector3(self.x - o.x, self.y - o.y, self.z - o.z)

    def __mul__(self, o: Vector3 | float) -> Vector3:
        if isinstance(o, Vector3):
            return Vector3(self.x * o.x, self.y * o.y, self.z * o.z)
        return Vector3(self.x * o, self.y * o, self.z * o)

    def __rmul__(self, o: float) -> Vector3:
        return Vector3(self.x * o, self.y * o, self.z * o)

    def __truediv__(self, o: Vector3 | float) -> Vector3:
        if isinstance(o, Vector3):
            return Vector3(self.x / o.x, self.y / o.y, self.z / o.z)
        return Vector3(self.x / o, self.y / o, self.z / o)

    def __neg__(self) -> Vector3: return Vector3(-self.x, -self.y, -self.z)
    def __pos__(self) -> Vector3: return Vector3(self.x, self.y, self.z)
    def __abs__(self) -> Vector3: return Vector3(abs(self.x), abs(self.y), abs(self.z))
    def __round__(self) -> Vector3: return self.round()
    def __floor__(self) -> Vector3: return self.floor()
    def __ceil__(self) -> Vector3: return self.ceil()

    def __eq__(self, o: object) -> bool:
        return (isinstance(o, Vector3)
                and self.x == o.x and self.y == o.y and self.z == o.z)

    def __ne__(self, o: object) -> bool:
        return not (isinstance(o, Vector3)
                    and self.x == o.x and self.y == o.y and self.z == o.z)

    def __hash__(self) -> int:
        return hash((self.x, self.y, self.z))

    def __lt__(self, o: Vector3) -> bool:
        if self.x == o.x:
            return self.z < o.z if self.y == o.y else self.y < o.y
        return self.x < o.x

    def __le__(self, o: Vector3) -> bool:
        if self.x == o.x:
            if self.y == o.y:
                return self.z <= o.z
            return self.y < o.y
        return self.x < o.x

    def __gt__(self, o: Vector3) -> bool:
        if self.x == o.x:
            return self.z > o.z if self.y == o.y else self.y > o.y
        return self.x > o.x

    def __ge__(self, o: Vector3) -> bool:
        if self.x == o.x:
            if self.y == o.y:
                return self.z >= o.z
            return self.y > o.y
        return self.x > o.x

    def __getitem__(self, i: int) -> float:
        if i == 0:
            return self.x
        if i == 1:
            return self.y
        if i == 2:
            return self.z
        raise IndexError("Vector3 index out of range")

    def __len__(self) -> int:
        return 3

    def __iter__(self) -> Iterator[float]:
        yield self.x
        yield self.y
        yield self.z

    def __bool__(self) -> bool:
        return self.x != 0.0 or self.y != 0.0 or self.z != 0.0

    def __repr__(self) -> str:
        return "Vector3({}, {}, {})".format(self.x, self.y, self.z)

    def __str__(self) -> str:
        return "({}, {}, {})".format(self.x, self.y, self.z)


Vector3.ZERO = Vector3(0.0, 0.0, 0.0)
Vector3.ONE = Vector3(1.0, 1.0, 1.0)
Vector3.INF = Vector3(float("inf"), float("inf"), float("inf"))
Vector3.LEFT = Vector3(-1.0, 0.0, 0.0)
Vector3.RIGHT = Vector3(1.0, 0.0, 0.0)
Vector3.UP = Vector3(0.0, 1.0, 0.0)
Vector3.DOWN = Vector3(0.0, -1.0, 0.0)
Vector3.FORWARD = Vector3(0.0, 0.0, -1.0)
Vector3.BACK = Vector3(0.0, 0.0, 1.0)
Vector3.MODEL_LEFT = Vector3(1.0, 0.0, 0.0)
Vector3.MODEL_RIGHT = Vector3(-1.0, 0.0, 0.0)
Vector3.MODEL_TOP = Vector3(0.0, 1.0, 0.0)
Vector3.MODEL_BOTTOM = Vector3(0.0, -1.0, 0.0)
Vector3.MODEL_FRONT = Vector3(0.0, 0.0, 1.0)
Vector3.MODEL_REAR = Vector3(0.0, 0.0, -1.0)
