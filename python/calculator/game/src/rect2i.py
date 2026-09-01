"""Integer 2D axis-aligned rectangle for grid / pixel math.

Mirrors Godot's Rect2i. Convention: `size` is normally non-negative and `end`
(bottom-right) is exclusive, so a rect covers columns `x .. x + width - 1`.
Call `abs()` first on any rect that may have a negative size.
"""

from vector2i import Vector2i


class Rect2i:
    __slots__ = ("position", "size")

    # -- construction -------------------------------------------------------

    def __init__(self, *args) -> None:
        """Accepts any of:
        Rect2i()                       -> zero rect
        Rect2i(other_rect)             -> copy
        Rect2i(position, size)         -> two Vector2i / tuples
        Rect2i(x, y, width, height)    -> four ints
        """
        if len(args) == 0:
            px = py = sx = sy = 0
        elif len(args) == 1 and isinstance(args[0], Rect2i):
            px, py = args[0].position.x, args[0].position.y
            sx, sy = args[0].size.x, args[0].size.y
        elif len(args) == 2:
            px, py = args[0][0], args[0][1]
            sx, sy = args[1][0], args[1][1]
        elif len(args) == 4:
            px, py, sx, sy = args
        else:
            raise TypeError("Rect2i(): expected 0, 1, 2 or 4 arguments")
        self.position: Vector2i = Vector2i(px, py)
        self.size: Vector2i = Vector2i(sx, sy)

    @classmethod
    def from_corners(cls, a: Vector2i, b: Vector2i) -> "Rect2i":
        """Rectangle spanning two opposite corners (order-independent)."""
        x0, x1 = (a[0], b[0]) if a[0] <= b[0] else (b[0], a[0])
        y0, y1 = (a[1], b[1]) if a[1] <= b[1] else (b[1], a[1])
        return cls(x0, y0, x1 - x0, y1 - y0)

    @classmethod
    def from_points(cls, points) -> "Rect2i":
        """Smallest rectangle containing every point in the iterable."""
        it = iter(points)
        try:
            first = next(it)
        except StopIteration:
            return cls()
        min_x = max_x = first[0]
        min_y = max_y = first[1]
        for p in it:
            if p[0] < min_x:
                min_x = p[0]
            if p[0] > max_x:
                max_x = p[0]
            if p[1] < min_y:
                min_y = p[1]
            if p[1] > max_y:
                max_y = p[1]
        return cls(min_x, min_y, max_x - min_x, max_y - min_y)

    def copy(self) -> "Rect2i":
        return Rect2i(self.position.x, self.position.y, self.size.x, self.size.y)

    def to_tuple(self) -> "tuple[int, int, int, int]":
        return (self.position.x, self.position.y, self.size.x, self.size.y)

    # -- scalar accessors ----------------------------------------------------

    @property
    def x(self) -> int:
        return self.position.x

    @x.setter
    def x(self, value: int) -> None:
        self.position.x = int(value)

    @property
    def y(self) -> int:
        return self.position.y

    @y.setter
    def y(self, value: int) -> None:
        self.position.y = int(value)

    @property
    def width(self) -> int:
        return self.size.x

    @width.setter
    def width(self, value: int) -> None:
        self.size.x = int(value)

    @property
    def height(self) -> int:
        return self.size.y

    @height.setter
    def height(self, value: int) -> None:
        self.size.y = int(value)

    @property
    def left(self) -> int:
        return self.position.x

    @property
    def top(self) -> int:
        return self.position.y

    @property
    def right(self) -> int:
        return self.position.x + self.size.x

    @property
    def bottom(self) -> int:
        return self.position.y + self.size.y

    @property
    def end(self) -> Vector2i:
        """Bottom-right corner (exclusive), = position + size."""
        return Vector2i(self.position.x + self.size.x, self.position.y + self.size.y)

    @end.setter
    def end(self, value: Vector2i) -> None:
        self.size = Vector2i(value[0] - self.position.x, value[1] - self.position.y)

    @property
    def center(self) -> Vector2i:
        return Vector2i(self.position.x + self.size.x // 2,
                        self.position.y + self.size.y // 2)

    @property
    def area(self) -> int:
        return self.size.x * self.size.y

    # Godot-style method aliases
    def get_area(self) -> int:
        return self.size.x * self.size.y

    def get_center(self) -> Vector2i:
        return self.center

    # -- representation ----------------------------------------------------

    def __repr__(self) -> str:
        return "Rect2i({}, {}, {}, {})".format(
            self.position.x, self.position.y, self.size.x, self.size.y)

    def __str__(self) -> str:
        return "[P{} S{}]".format(self.position, self.size)

    # -- comparison / hashing ---------------------------------------------

    def __eq__(self, other: object) -> bool:
        if isinstance(other, Rect2i):
            return self.position == other.position and self.size == other.size
        return NotImplemented

    def __ne__(self, other: object) -> bool:
        result = self.__eq__(other)
        if result is NotImplemented:
            return result
        return not result

    def __hash__(self) -> int:
        return hash((self.position.x, self.position.y, self.size.x, self.size.y))

    def __bool__(self) -> bool:
        return self.has_area()

    def __iter__(self):
        yield self.position.x
        yield self.position.y
        yield self.size.x
        yield self.size.y

    def __contains__(self, item: "Vector2i | Rect2i") -> bool:
        if isinstance(item, Rect2i):
            return self.encloses(item)
        return self.has_point(item)

    # -- translation ------------------------------------------------------

    def __add__(self, offset: Vector2i) -> "Rect2i":
        return Rect2i(self.position.x + offset[0], self.position.y + offset[1],
                      self.size.x, self.size.y)

    __radd__ = __add__

    def __sub__(self, offset: Vector2i) -> "Rect2i":
        return Rect2i(self.position.x - offset[0], self.position.y - offset[1],
                      self.size.x, self.size.y)

    def moved(self, offset: Vector2i) -> "Rect2i":
        return self.__add__(offset)

    def with_position(self, position: Vector2i) -> "Rect2i":
        return Rect2i(position[0], position[1], self.size.x, self.size.y)

    def with_size(self, size: Vector2i) -> "Rect2i":
        return Rect2i(self.position.x, self.position.y, size[0], size[1])

    # -- normalization --------------------------------------------------------

    def abs(self) -> "Rect2i":
        """Equivalent rect with non-negative size and top-left position."""
        px = self.position.x + (self.size.x if self.size.x < 0 else 0)
        py = self.position.y + (self.size.y if self.size.y < 0 else 0)
        return Rect2i(px, py, abs(self.size.x), abs(self.size.y))

    __abs__ = abs

    # -- queries ---------------------------------------------------------------

    def has_area(self) -> bool:
        return self.size.x > 0 and self.size.y > 0

    def has_point(self, point: Vector2i) -> bool:
        """True if `point` lies inside; right/bottom edges are exclusive."""
        return (self.position.x <= point[0] < self.position.x + self.size.x and
                self.position.y <= point[1] < self.position.y + self.size.y)

    def encloses(self, other: "Rect2i") -> bool:
        """True if `other` lies entirely within this rect."""
        return (other.position.x >= self.position.x and
                other.position.y >= self.position.y and
                other.position.x + other.size.x <= self.position.x + self.size.x and
                other.position.y + other.size.y <= self.position.y + self.size.y)

    def intersects(self, other: "Rect2i", include_borders: bool = False) -> bool:
        """True if the two rects overlap. Call abs() first on negative-size rects."""
        if include_borders:
            return (self.position.x <= other.position.x + other.size.x and
                    self.position.x + self.size.x >= other.position.x and
                    self.position.y <= other.position.y + other.size.y and
                    self.position.y + self.size.y >= other.position.y)
        return (self.position.x < other.position.x + other.size.x and
                self.position.x + self.size.x > other.position.x and
                self.position.y < other.position.y + other.size.y and
                self.position.y + self.size.y > other.position.y)

    # -- combinations -------------------------------------------------------

    def intersection(self, other: "Rect2i") -> "Rect2i":
        """Overlapping region, or a zero rect if they do not intersect."""
        if not self.intersects(other):
            return Rect2i()
        x0 = max(self.position.x, other.position.x)
        y0 = max(self.position.y, other.position.y)
        x1 = min(self.position.x + self.size.x, other.position.x + other.size.x)
        y1 = min(self.position.y + self.size.y, other.position.y + other.size.y)
        return Rect2i(x0, y0, x1 - x0, y1 - y0)

    def merge(self, other: "Rect2i") -> "Rect2i":
        """Smallest rect containing both."""
        x0 = min(self.position.x, other.position.x)
        y0 = min(self.position.y, other.position.y)
        x1 = max(self.position.x + self.size.x, other.position.x + other.size.x)
        y1 = max(self.position.y + self.size.y, other.position.y + other.size.y)
        return Rect2i(x0, y0, x1 - x0, y1 - y0)

    def expand(self, to: Vector2i) -> "Rect2i":
        """Smallest rect containing this rect and the point `to`."""
        x0 = min(self.position.x, to[0])
        y0 = min(self.position.y, to[1])
        x1 = max(self.position.x + self.size.x, to[0])
        y1 = max(self.position.y + self.size.y, to[1])
        return Rect2i(x0, y0, x1 - x0, y1 - y0)

    def grow(self, amount: int) -> "Rect2i":
        """Grow (or shrink, if negative) by `amount` on every side."""
        return Rect2i(self.position.x - amount, self.position.y - amount,
                      self.size.x + amount * 2, self.size.y + amount * 2)

    def grow_individual(self, left: int, top: int, right: int, bottom: int) -> "Rect2i":
        return Rect2i(self.position.x - left, self.position.y - top,
                      self.size.x + left + right, self.size.y + top + bottom)

    __and__ = intersection
    __or__ = merge

    # -- helpers -----------------------------------------------------------

    def clamp_point(self, point: Vector2i) -> Vector2i:
        """Nearest point inside [position, end]."""
        cx = _clamp(point[0], self.position.x, self.position.x + self.size.x)
        cy = _clamp(point[1], self.position.y, self.position.y + self.size.y)
        return Vector2i(cx, cy)

    def corners(self) -> "tuple[Vector2i, Vector2i, Vector2i, Vector2i]":
        """Top-left, top-right, bottom-right, bottom-left (end exclusive)."""
        x0, y0 = self.position.x, self.position.y
        x1, y1 = x0 + self.size.x, y0 + self.size.y
        return (Vector2i(x0, y0), Vector2i(x1, y0), Vector2i(x1, y1), Vector2i(x0, y1))

    def iter_points(self):
        """Yield every integer cell Vector2i(x, y) covered by this rect."""
        r = self.abs()
        for gy in range(r.position.y, r.position.y + r.size.y):
            for gx in range(r.position.x, r.position.x + r.size.x):
                yield Vector2i(gx, gy)


# -- module helpers -----------------------------------------------------------

def _clamp(v: int, lo: int, hi: int) -> int:
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v
