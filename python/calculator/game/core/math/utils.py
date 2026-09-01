"""Scalar math helpers shared by the vector types.

Ports of the pieces of Godot 4's `Math` singleton that the vector classes need,
plus a couple of C-style integer helpers. All operate on plain `int` / `float`.
"""

import math

CMP_EPSILON: float = 1e-05
UNIT_EPSILON: float = 0.001

INT_MIN: int = -2147483648
INT_MAX: int = 2147483647


def signf(v: float) -> float:
    """-1.0, 0.0 or 1.0 (0.0 for zero, unlike math.copysign)."""
    return float((v > 0.0) - (v < 0.0))


def signi(v: int) -> int:
    """-1, 0 or 1."""
    return (v > 0) - (v < 0)


def lerp(a: float, b: float, t: float) -> float:
    return a + (b - a) * t


def clampf(v: float, lo: float, hi: float) -> float:
    return lo if v < lo else (hi if v > hi else v)


def clampi(v: int, lo: int, hi: int) -> int:
    return lo if v < lo else (hi if v > hi else v)


def round_half_away(v: float) -> float:
    """Round half away from zero, like C/Godot round() (not Python's round())."""
    return float(math.floor(v + 0.5)) if v >= 0.0 else float(math.ceil(v - 0.5))


def inverse(v: float) -> float:
    """1 / v, returning +inf for 0 instead of raising."""
    return float("inf") if v == 0.0 else 1.0 / v


def fposmod(a: float, b: float) -> float:
    """Modulo whose result takes the sign of `b` (Godot's Math.fposmod)."""
    r = math.fmod(a, b)
    if r != 0.0 and ((r < 0.0) != (b < 0.0)):
        r += b
    return r


def snapped(value: float, step: float) -> float:
    if step != 0.0:
        return math.floor(value / step + 0.5) * step
    return value


def snappedi(value: int, step: int) -> int:
    if step != 0:
        return int(math.floor(value / step + 0.5)) * step
    return value


def idiv(a: int, b: int) -> int:
    """Integer division truncated toward zero (like Godot / C++, not Python //)."""
    q = abs(a) // abs(b)
    return -q if (a < 0) != (b < 0) else q


def imod(a: int, b: int) -> int:
    """Remainder matching idiv(): result takes the sign of `a`."""
    return a - idiv(a, b) * b


def is_equal_approx(a: float, b: float) -> bool:
    if a == b:
        return True
    tol = CMP_EPSILON * abs(a)
    if tol < CMP_EPSILON:
        tol = CMP_EPSILON
    return abs(a - b) < tol


def is_zero_approx(a: float) -> bool:
    return abs(a) < CMP_EPSILON


def cubic_interpolate(p_from: float, p_to: float, p_pre: float, p_post: float, w: float) -> float:
    return 0.5 * (
        (p_from * 2.0)
        + (-p_pre + p_to) * w
        + (2.0 * p_pre - 5.0 * p_from + 4.0 * p_to - p_post) * (w * w)
        + (-p_pre + 3.0 * p_from - 3.0 * p_to + p_post) * (w * w * w)
    )


def cubic_interpolate_in_time(
    p_from: float,
    p_to: float,
    p_pre: float,
    p_post: float,
    w: float,
    to_t: float,
    pre_t: float,
    post_t: float,
) -> float:
    t = lerp(0.0, to_t, w)
    a1 = lerp(p_pre, p_from, 0.0 if pre_t == 0.0 else (t - pre_t) / -pre_t)
    a2 = lerp(p_from, p_to, 0.5 if to_t == 0.0 else t / to_t)
    a3 = lerp(p_to, p_post, 1.0 if (post_t - to_t) == 0.0 else (t - to_t) / (post_t - to_t))
    b1 = lerp(a1, a2, 0.0 if (to_t - pre_t) == 0.0 else (t - pre_t) / (to_t - pre_t))
    b2 = lerp(a2, a3, 1.0 if post_t == 0.0 else t / post_t)
    return lerp(b1, b2, 0.5 if to_t == 0.0 else t / to_t)
