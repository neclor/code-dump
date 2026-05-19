using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Numerics;

namespace Core.Common;

public struct Vector2I(int x = 0, int y = 0) : IEquatable<Vector2I> {

	public enum Axis {
		X,
		Y,
		ElementCount
	}

	public int X { readonly get; set; } = x;
	public int Y { readonly get; set; } = y;

	public int this[int index] {
		readonly get {
			return index switch {
				0 => X,
				1 => Y,
				_ => throw new ArgumentOutOfRangeException(nameof(index)),
			};
		}
		set {
			switch (index) {
				case 0:
					X = value;
					break;
				case 1:
					Y = value;
					break;
				default:
					throw new ArgumentOutOfRangeException(nameof(index));
			}
		}
	}

	public static Vector2I MinValue { get; } = new(int.MinValue, int.MinValue);
	public static Vector2I MaxValue { get; } = new(int.MaxValue, int.MaxValue);
	public static Vector2I UnitX { get; } = new(1, 0);
	public static Vector2I UnitY { get; } = new(0, 1);
	public static Vector2I Zero { get; } = new(0, 0);
	public static Vector2I One { get; } = new(1, 1);
	public static Vector2I Up { get; } = new(0, -1);
	public static Vector2I Down { get; } = new(0, 1);
	public static Vector2I Right { get; } = new(1, 0);
	public static Vector2I Left { get; } = new(-1, 0);

	public static Vector2I operator +(Vector2I left, Vector2I right) => left.Add(right);
	public static Vector2I operator -(Vector2I left, Vector2I right) => left.Subtract(right);
	public static Vector2I operator -(Vector2I vector) => vector.Negate();
	public static Vector2I operator *(Vector2I vector, int scale) => vector.Multiply(scale);
	public static Vector2I operator *(int scale, Vector2I vector) => vector * scale;
	public static Vector2I operator *(Vector2I left, Vector2I right) => left.Multiply(right);
	public static Vector2I operator /(Vector2I vector, int divisor) => vector.Divide(divisor);
	public static Vector2I operator /(Vector2I left, Vector2I right) => left.Divide(right);
	public static Vector2I operator %(Vector2I vector, int divisor) => vector.Mod(divisor);
	public static Vector2I operator %(Vector2I left, Vector2I right) => left.Mod(right);
	public static bool operator ==(Vector2I left, Vector2I right) => left.Equals(right);
	public static bool operator !=(Vector2I left, Vector2I right) => !(left == right);
	public static bool operator <(Vector2I left, Vector2I right) => left.CompareTo(right) < 0;
	public static bool operator >(Vector2I left, Vector2I right) => left.CompareTo(right) > 0;
	public static bool operator <=(Vector2I left, Vector2I right) => left.CompareTo(right) <= 0;
	public static bool operator >=(Vector2I left, Vector2I right) => left.CompareTo(right) >= 0;
	public static implicit operator Vector2(Vector2I value) => value.ToVector2();
	public static explicit operator Vector2I(Vector2 value) => FromVector2(value);

	public readonly Vector2I Add(Vector2I other) => new(X + other.X, Y + other.Y);

	public readonly Vector2I Subtract(Vector2I other) => new(X - other.X, Y - other.Y);

	public readonly Vector2I Negate() => new(-X, -Y);

	public readonly Vector2I Multiply(int scale) => new(X * scale, Y * scale);

	public readonly Vector2I Multiply(Vector2I other) => new(X * other.X, Y * other.Y);

	public readonly Vector2I Divide(int divisor) => new(X / divisor, Y / divisor);

	public readonly Vector2I Divide(Vector2I divisor) => new(X / divisor.X, Y / divisor.Y);

	public readonly Vector2I Mod(int divisor) => new(X % divisor, Y % divisor);

	public readonly Vector2I Mod(Vector2I divisor) => new(X % divisor.X, Y % divisor.Y);

	public readonly int CompareTo(Vector2I other) {
		int result = X.CompareTo(other.X);
		if (result != 0) return result;
		return Y.CompareTo(other.Y);
	}

	public static Vector2I FromVector2(Vector2 value) => new((int)value.X, (int)value.Y);

	public readonly Vector2 ToVector2() => new(X, Y);

	public readonly void Deconstruct(out int x, out int y) {
		x = X;
		y = Y;
	}

	public readonly Vector2I Abs() => new(int.Abs(X), int.Abs(Y));

	public readonly float Aspect() => (float)X / (float)Y;

	public readonly Vector2I Clamp(Vector2I min, Vector2I max) => new(int.Clamp(X, min.X, max.X), int.Clamp(Y, min.Y, max.Y));

	public readonly Vector2I Clamp(int min, int max) => new(int.Clamp(X, min, max), int.Clamp(Y, min, max));

	public readonly int DistanceSquaredTo(Vector2I to) => (to - this).LengthSquared();

	public readonly float DistanceTo(Vector2I to) => (to - this).Length();

	public readonly float Length() => float.Sqrt(X * X + Y * Y);

	public readonly int LengthSquared() => X * X + Y * Y;

	public readonly Vector2I Max(Vector2I with) => new(int.Max(X, with.X), int.Max(Y, with.Y));

	public readonly Vector2I Max(int with) => new(int.Max(X, with), int.Max(Y, with));

	public readonly Vector2I Min(Vector2I with) => new(int.Min(X, with.X), int.Min(Y, with.Y));

	public readonly Vector2I Min(int with) => new(int.Min(X, with), int.Min(Y, with));

	public readonly Axis MaxAxisIndex() => X >= Y ? Axis.X : Axis.Y;

	public readonly Axis MinAxisIndex() => X >= Y ? Axis.Y : Axis.X;

	public readonly Vector2I Sign() => new(int.Sign(X), int.Sign(Y));

	public readonly Vector2I Snapped(Vector2I step) => new(Snapped(X, step.X), Snapped(Y, step.Y));

	public readonly Vector2I Snapped(int step) => new(Snapped(X, step), Snapped(Y, step));

	public override readonly bool Equals([NotNullWhen(true)] object? obj) => obj is Vector2I other ? Equals(other) : false;

	public readonly bool Equals(Vector2I other) => X == other.X && Y == other.Y;

	public override readonly int GetHashCode() => HashCode.Combine(X, Y);

	public override readonly string ToString() => ToString(null);

	public readonly string ToString(string? format) => $"({X.ToString(format, CultureInfo.InvariantCulture)}, {Y.ToString(format, CultureInfo.InvariantCulture)})";

	private static int Snapped(int s, int step) => (int)Snapped((float)s, (float)step);

	private static float Snapped(float s, float step) {
		if (step == 0f) return s;

		return float.Floor(s / step + 0.5f) * step;
	}
}
