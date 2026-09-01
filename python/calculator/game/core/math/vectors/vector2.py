import math
from core.math.utils import *

from configs.system import *
if TYPIFICATOPN:
    from typing import TYPE_CHECKING
    if TYPE_CHECKING:
        from core.math.vectors.vector2i import Vector2i


class Vector2:
    __slots__ = ("x", "y")

    x: float
    y: float

    AXIS_X: int = 0
    AXIS_Y: int = 1

    ZERO: "Vector2"
    ONE: "Vector2"
    INF: "Vector2"
    LEFT: "Vector2"
    RIGHT: "Vector2"
    UP: "Vector2"
    DOWN: "Vector2"

    def __init__( self, x: "Vector2 | Vector2i | tuple[float, float] | list[float] | float" = 0.0, y: float = 0.0) -> None:
        if isinstance(x, (int, float)):
            fx, fy = float(x), float(y)
        elif isinstance(x, (tuple, list)):
            fx, fy = float(x[0]), float(x[1])
        else:
            fx, fy = float(x.x), float(x.y)
        object.__setattr__(self, "x", fx)
        object.__setattr__(self, "y", fy)

    def __setattr__(self, name: str, value: object) -> None:
        raise AttributeError("Vector2 is immutable; use with_x()/with_y()")

    def __delattr__(self, name: str) -> None:
        raise AttributeError("Vector2 is immutable")

    # -- copy helpers (not in Godot, convenient in Python) ------------------

    def with_x(self, x: float) -> Vector2: return Vector2(x, self.y)
    def with_y(self, y: float) -> Vector2: return Vector2(self.x, y)
    def copy(self) -> Vector2: return Vector2(self.x, self.y)

    # -- Godot methods -----------------------------------------------------

    def abs(self) -> Vector2:
        return Vector2(abs(self.x), abs(self.y))

    def angle(self) -> float:
        return math.atan2(self.y, self.x)

    def angle_to(self, to: Vector2) -> float:
        return math.atan2(self.cross(to), self.dot(to))

    def angle_to_point(self, to: Vector2) -> float:
        return math.atan2(to.y - self.y, to.x - self.x)

    def aspect(self) -> float:
        if self.y == 0.0:
            return float("inf") if self.x > 0.0 else (float("-inf") if self.x < 0.0 else float("nan"))
        return self.x / self.y

    def bezier_derivative(self, control_1: Vector2, control_2: Vector2, end: Vector2, t: float) -> Vector2:
        omt = 1.0 - t
        omt2 = omt * omt
        t2 = t * t
        return (self * (-3.0 * omt2)
                + control_1 * (3.0 * omt2 - 6.0 * omt * t)
                + control_2 * (6.0 * omt * t - 3.0 * t2)
                + end * (3.0 * t2))

    def bezier_interpolate(self, control_1: Vector2, control_2: Vector2, end: Vector2, t: float) -> Vector2:
        omt = 1.0 - t
        omt2 = omt * omt
        omt3 = omt2 * omt
        t2 = t * t
        t3 = t2 * t
        return (self * omt3
                + control_1 * (omt2 * t * 3.0)
                + control_2 * (omt * t2 * 3.0)
                + end * t3)

    def bounce(self, n: Vector2) -> Vector2:
        return -self.reflect(n)

    def ceil(self) -> Vector2:
        return Vector2(math.ceil(self.x), math.ceil(self.y))

    def clamp(self, min: Vector2, max: Vector2) -> Vector2:
        return Vector2(clampf(self.x, min.x, max.x), clampf(self.y, min.y, max.y))

    def clampf(self, min: float, max: float) -> Vector2:
        return Vector2(clampf(self.x, min, max), clampf(self.y, min, max))

    def cross(self, with_: Vector2) -> float:
        return self.x * with_.y - self.y * with_.x

    def cubic_interpolate(self, b: Vector2, pre_a: Vector2, post_b: Vector2, weight: float) -> Vector2:
        return Vector2(cubic_interpolate(self.x, b.x, pre_a.x, post_b.x, weight),
                       cubic_interpolate(self.y, b.y, pre_a.y, post_b.y, weight))

    def cubic_interpolate_in_time(
        self,
        b: Vector2,
        pre_a: Vector2,
        post_b: Vector2,
        weight: float,
        b_t: float,
        pre_a_t: float,
        post_b_t: float,
    ) -> Vector2:
        return Vector2(
            cubic_interpolate_in_time(self.x, b.x, pre_a.x, post_b.x, weight, b_t, pre_a_t, post_b_t),
            cubic_interpolate_in_time(self.y, b.y, pre_a.y, post_b.y, weight, b_t, pre_a_t, post_b_t))

    def direction_to(self, to: Vector2) -> Vector2:
        return Vector2(to.x - self.x, to.y - self.y).normalized()

    def distance_squared_to(self, to: Vector2) -> float:
        dx = to.x - self.x
        dy = to.y - self.y
        return dx * dx + dy * dy

    def distance_to(self, to: Vector2) -> float:
        return math.sqrt(self.distance_squared_to(to))

    def dot(self, with_: Vector2) -> float:
        return self.x * with_.x + self.y * with_.y

    def floor(self) -> Vector2:
        return Vector2(math.floor(self.x), math.floor(self.y))

    @staticmethod
    def from_angle(angle: float) -> Vector2:
        return Vector2(math.cos(angle), math.sin(angle))

    def is_equal_approx(self, to: Vector2) -> bool:
        return is_equal_approx(self.x, to.x) and is_equal_approx(self.y, to.y)

    def is_finite(self) -> bool:
        return math.isfinite(self.x) and math.isfinite(self.y)

    def is_normalized(self) -> bool:
        return abs(self.length_squared() - 1.0) < UNIT_EPSILON

    def is_zero_approx(self) -> bool:
        return is_zero_approx(self.x) and is_zero_approx(self.y)

    def length(self) -> float:
        return math.sqrt(self.x * self.x + self.y * self.y)

    def length_squared(self) -> float:
        return self.x * self.x + self.y * self.y

    def lerp(self, to: Vector2, weight: float) -> Vector2:
        return Vector2(lerp(self.x, to.x, weight), lerp(self.y, to.y, weight))

    def limit_length(self, length: float = 1.0) -> Vector2:
        l = self.length()
        v: Vector2 = self
        if l > 0.0 and length < l:
            v = v / l * length
        return Vector2(v.x, v.y)

    def max(self, with_: Vector2) -> Vector2:
        return Vector2(max(self.x, with_.x), max(self.y, with_.y))

    def maxf(self, with_: float) -> Vector2:
        return Vector2(max(self.x, with_), max(self.y, with_))

    def max_axis_index(self) -> int:
        return Vector2.AXIS_Y if self.x < self.y else Vector2.AXIS_X

    def min(self, with_: Vector2) -> Vector2:
        return Vector2(min(self.x, with_.x), min(self.y, with_.y))

    def minf(self, with_: float) -> Vector2:
        return Vector2(min(self.x, with_), min(self.y, with_))

    def min_axis_index(self) -> int:
        return Vector2.AXIS_X if self.x < self.y else Vector2.AXIS_Y

    def move_toward(self, to: Vector2, delta: float) -> Vector2:
        diff = Vector2(to.x - self.x, to.y - self.y)
        length = diff.length()
        if length <= delta or length < CMP_EPSILON:
            return Vector2(to.x, to.y)
        return self + diff / length * delta

    def normalized(self) -> Vector2:
        l = self.length()
        if l == 0.0:
            return Vector2(0.0, 0.0)
        return Vector2(self.x / l, self.y / l)

    def orthogonal(self) -> Vector2:
        return Vector2(self.y, -self.x)

    def posmod(self, mod: float) -> Vector2:
        return Vector2(fposmod(self.x, mod), fposmod(self.y, mod))

    def posmodv(self, modv: Vector2) -> Vector2:
        return Vector2(fposmod(self.x, modv.x), fposmod(self.y, modv.y))

    def project(self, b: Vector2) -> Vector2:
        return b * (self.dot(b) / b.length_squared())

    def reflect(self, line: Vector2) -> Vector2:
        return 2.0 * line * self.dot(line) - self

    def rotated(self, angle: float) -> Vector2:
        c = math.cos(angle)
        s = math.sin(angle)
        return Vector2(self.x * c - self.y * s, self.x * s + self.y * c)

    def round(self) -> Vector2:
        return Vector2(round_half_away(self.x), round_half_away(self.y))

    def sign(self) -> Vector2:
        return Vector2(signf(self.x), signf(self.y))

    def slerp(self, to: Vector2, weight: float) -> Vector2:
        start_length_sq = self.length_squared()
        end_length_sq = to.length_squared()
        if start_length_sq == 0.0 or end_length_sq == 0.0:
            return self.lerp(to, weight)
        start_length = math.sqrt(start_length_sq)
        result_length = lerp(start_length, math.sqrt(end_length_sq), weight)
        angle = self.angle_to(to)
        return self.rotated(angle * weight) * (result_length / start_length)

    def slide(self, n: Vector2) -> Vector2:
        return self - n * self.dot(n)

    def snapped(self, step: Vector2) -> Vector2:
        return Vector2(snapped(self.x, step.x), snapped(self.y, step.y))

    def snappedf(self, step: float) -> Vector2:
        return Vector2(snapped(self.x, step), snapped(self.y, step))

    # -- operators -------------------------------------------------------------

    def __add__(self, o: Vector2) -> Vector2:
        return Vector2(self.x + o.x, self.y + o.y)

    def __sub__(self, o: Vector2) -> Vector2:
        return Vector2(self.x - o.x, self.y - o.y)

    def __mul__(self, o: Vector2 | float) -> Vector2:
        if isinstance(o, Vector2):
            return Vector2(self.x * o.x, self.y * o.y)
        return Vector2(self.x * o, self.y * o)

    def __rmul__(self, o: float) -> Vector2:
        return Vector2(self.x * o, self.y * o)

    def __truediv__(self, o: Vector2 | float) -> Vector2:
        if isinstance(o, Vector2):
            return Vector2(self.x / o.x, self.y / o.y)
        return Vector2(self.x / o, self.y / o)

    def __neg__(self) -> Vector2: return Vector2(-self.x, -self.y)
    def __pos__(self) -> Vector2: return Vector2(self.x, self.y)
    def __abs__(self) -> Vector2: return Vector2(abs(self.x), abs(self.y))
    def __round__(self) -> Vector2: return self.round()
    def __floor__(self) -> Vector2: return self.floor()
    def __ceil__(self) -> Vector2: return self.ceil()

    def __eq__(self, o: object) -> bool:
        return isinstance(o, Vector2) and self.x == o.x and self.y == o.y

    def __ne__(self, o: object) -> bool:
        return not (isinstance(o, Vector2) and self.x == o.x and self.y == o.y)

    def __hash__(self) -> int:
        return hash((self.x, self.y))

    def __lt__(self, o: Vector2) -> bool:
        return self.y < o.y if self.x == o.x else self.x < o.x

    def __le__(self, o: Vector2) -> bool:
        return self.y <= o.y if self.x == o.x else self.x < o.x

    def __gt__(self, o: Vector2) -> bool:
        return self.y > o.y if self.x == o.x else self.x > o.x

    def __ge__(self, o: Vector2) -> bool:
        return self.y >= o.y if self.x == o.x else self.x > o.x

    def __getitem__(self, i: int) -> float:
        if i == 0:
            return self.x
        if i == 1:
            return self.y
        raise IndexError("Vector2 index out of range")

    def __len__(self) -> int:
        return 2

    def __iter__(self) -> Iterator[float]:
        yield self.x
        yield self.y

    def __bool__(self) -> bool:
        return self.x != 0.0 or self.y != 0.0

    def __repr__(self) -> str:
        return "Vector2({}, {})".format(self.x, self.y)

    def __str__(self) -> str:
        return "({}, {})".format(self.x, self.y)


Vector2.ZERO = Vector2(0.0, 0.0)
Vector2.ONE = Vector2(1.0, 1.0)
Vector2.INF = Vector2(float("inf"), float("inf"))
Vector2.LEFT = Vector2(-1.0, 0.0)
Vector2.RIGHT = Vector2(1.0, 0.0)
Vector2.UP = Vector2(0.0, -1.0)
Vector2.DOWN = Vector2(0.0, 1.0)
