using System.Numerics;
using System.Text;

namespace MathLibrary;

public readonly struct Matrix<T> : IEquatable<Matrix<T>> where T : INumber<T> { // IEnumerable<T>,

	private readonly T[] _data;

	public int Length => _data.Length;
	public int Rows { get; }
	public int Columns { get; }
	public bool IsSquare => Rows == Columns;

	public T this[int row, int column] {
		get => GetValue(row, column);
		set => SetValue(row, column, value);
	}

	public Matrix(int rows = 3, int columns = 3, T[]? values = default) {
		if (rows <= 0) throw new ArgumentOutOfRangeException(nameof(rows), "Number of rows must be positive.");
		if (columns <= 0) throw new ArgumentOutOfRangeException(nameof(columns), "Number of columns must be positive.");
		if (values is not null && rows * columns != values.Length) throw new ArgumentException("Size not matching", nameof(values));

		Rows = rows;
		Columns = columns;
		_data = (T[])(values?.Clone() ?? new T[rows * columns]);
	}

	public static Matrix<T> operator +(Matrix<T> matrix) => matrix.Clone();
	public static Matrix<T> operator -(Matrix<T> matrix) => matrix.Negate();
	public static Matrix<T> operator +(Matrix<T> a, Matrix<T> b) => a.Add(b);
	public static Matrix<T> operator -(Matrix<T> a, Matrix<T> b) => a.Subtract(b);
	public static Matrix<T> operator *(Matrix<T> matrix, T value) => matrix.Multiply(value);
	public static Matrix<T> operator *(T value, Matrix<T> matrix) => matrix * value;
	public static Matrix<T> operator *(Matrix<T> a, Matrix<T> b) => a.Multiply(b);
	public static Matrix<T> operator /(Matrix<T> matrix, T value) => matrix.Divide(value);
	public static bool operator ==(Matrix<T> a, Matrix<T> b) => Equals(a, b);
	public static bool operator !=(Matrix<T> a, Matrix<T> b) => !(a == b);

#pragma warning disable CA1000
	public static Matrix<T> Identity(int size) {
		if (size <= 0) throw new ArgumentException("Size must be positive");

		Matrix<T> result = new(size, size);
		for (int i = 0; i < size; i++) {
			result._data[result.IndexFrom(i, i)] = T.One;
		}

		return result;
	}
#pragma warning restore CA1000

	public Matrix<T> Clone() => new(Rows, Columns, _data);

	public T GetValue(int row, int column) {
		CheckBounds(row, column);
		return _data[IndexFrom(row, column)];
	}

	public void SetValue(int row, int column, T value) {
		CheckBounds(row, column);
		_data[IndexFrom(row, column)] = value;
	}

	public Matrix<T> Plus() => Clone();

	public Matrix<T> Negate() => Multiply(-T.One);

	public Matrix<T> Add(Matrix<T> other) {
		if (Rows != other.Rows || Columns != other.Columns) throw new ArgumentException("The matrix dimensions do not match", nameof(other));

		Matrix<T> result = Clone();
		for (int i = 0; i < Length; i++) {
			result._data[i] += other._data[i];
		}

		return result;
	}

	public Matrix<T> Subtract(Matrix<T> other) {
		if (Rows != other.Rows || Columns != other.Columns) throw new ArgumentException("The matrix dimensions do not match", nameof(other));

		Matrix<T> result = Clone();
		for (int i = 0; i < Length; i++) {
			result._data[i] -= other._data[i];
		}

		return result;
	}

	public Matrix<T> Multiply(T value) {
		Matrix<T> result = Clone();
		for (int i = 0; i < Length; i++) {
			result._data[i] *= value;
		}

		return result;
	}

	public Matrix<T> Multiply(Matrix<T> other) {
		if (Columns != other.Rows) throw new ArgumentException("The matrix dimensions do not match", nameof(other));

		Matrix<T> result = new(Rows, other.Columns);
		for (int rowA = 0; rowA < Rows; rowA++) {
			for (int columnA = 0; columnA < Columns; columnA++) {
				T valueA = _data[IndexFrom(rowA, columnA)];
				for (int columnB = 0; columnB < other.Columns; columnB++) {
					result._data[result.IndexFrom(rowA, columnB)] += valueA * other._data[other.IndexFrom(columnA, columnB)];
				}
			}
		}

		return result;
	}

	public Matrix<T> Divide(T value) {
		Matrix<T> result = Clone();
		for (int i = 0; i < Length; i++) {
			result._data[i] /= value;
		}

		return result;
	}

	public Matrix<T> Power(int value) {
		if (!IsSquare) throw new ArgumentException("Matrix should be square");
		if (value < 0) throw new ArgumentOutOfRangeException(nameof(value), "Value must be positive.");

		if (value == 0) return Identity(Rows);

		Matrix<T> result = Clone();
		for (int i = 1; i < value; i++) {
			result = result.Multiply(this);
		}

		return result;
	}

	public Matrix<T> SwapRows(int row1, int row2) {
		CheckRowBounds(row1);
		CheckRowBounds(row2);
		if (row1 == row2) return Clone();

		Matrix<T> result = Clone();
		for (int column = 0; column < Columns; column++) {
			(result._data[IndexFrom(row1, column)], result._data[IndexFrom(row2, column)]) = (result._data[IndexFrom(row2, column)], result._data[IndexFrom(row1, column)]);
		}

		return result;
	}

	public Matrix<T> RemoveRow(int row) {
		if (Rows < 2) throw new InvalidOperationException("Matrix must have at least two row");
		CheckRowBounds(row);

		Matrix<T> result = new(Rows - 1, Columns);
		for (int r = 0; r < Rows; r++) {
			if (r == row) continue;
			for (int column = 0; column < Columns; column++) {
				int targetRow = r < row ? r : r - 1;
				result._data[result.IndexFrom(targetRow, column)] = _data[IndexFrom(r, column)];
			}
		}
		return result;
	}

	public Matrix<T> RemoveColumn(int column) {
		if (Columns < 2) throw new InvalidOperationException("Matrix must have at least two column");
		CheckColumnBounds(column);

		Matrix<T> result = new(Rows, Columns - 1);
		for (int row = 0; row < Rows; row++) {
			for (int c = 0; c < Columns; c++) {
				if (c == column) continue;
				int targetColumn = c < column ? c : c - 1;
				result._data[result.IndexFrom(row, targetColumn)] = _data[IndexFrom(row, c)];
			}
		}
		return result;
	}

	public Matrix<T> MultiplyRow(int row, T value) {
		CheckRowBounds(row);

		Matrix<T> result = Clone();
		for (int i = IndexFrom(row, 0); i < IndexFrom(row, Columns); i++) {
			result._data[i] *= value;
		}

		return result;
	}

	public Matrix<T> DivideRow(int row, T value) {
		CheckRowBounds(row);

		Matrix<T> result = Clone();
		for (int i = IndexFrom(row, 0); i < IndexFrom(row, Columns); i++) {
			result._data[i] /= value;
		}

		return result;
	}

	public Matrix<T> AddMultipliedRow(int row1, int row2, T factor) {
		CheckRowBounds(row1);
		CheckRowBounds(row2);

		Matrix<T> result = Clone();
		for (int column = 0; column < Columns; column++) {
			result._data[IndexFrom(row1, column)] += result._data[IndexFrom(row2, column)] * factor;
		}
		return result;
	}

	public Matrix<T> Transpose() {
		Matrix<T> result = new(Columns, Rows);
		for (int row = 0; row < Rows; row++) {
			for (int column = 0; column < Columns; column++) {
				result._data[result.IndexFrom(column, row)] = _data[IndexFrom(row, column)];
			}
		}
		return result;
	}

	public T Determinant() {
		if (!IsSquare) throw new ArgumentException("Matrix should be square");

		T sum = T.Zero;
		for (int column = 0; column < Columns; column++) {
			T diagonalProduct = T.One;
			for (int row = 0; row < Rows; row++) {
				diagonalProduct *= _data[IndexFrom(row, (column + row) % Columns)];
			}
			sum += diagonalProduct;
		}

		for (int column = 0; column < Columns; column++) {
			T diagonalProduct = T.One;
			for (int row = 0; row < Rows; row++) {
				diagonalProduct *= _data[IndexFrom(Rows - (row + 1), (column + row) % Columns)];
			}
			sum -= diagonalProduct;
		}

		return sum;
	}

	public int Rang() {
		// TODO implement
		return 0;
	}

	public Matrix<T> Inverse() {
		if (!IsSquare) throw new ArgumentException("Matrix should be square");
		if (Determinant() == T.Zero) throw new ArgumentException("Matrix is ​​not invertible");

		Matrix<T> matrix = Clone();
		Matrix<T> result = Identity(Rows);

		for (int column = 0; column < Columns; column++) {

			int maxRow = column;
			T pivotValue = matrix._data[IndexFrom(column, column)];
			for (int row = column + 1; row < Rows; row++) {
				T value = matrix._data[IndexFrom(row, column)];
				if (T.Abs(value) > T.Abs(pivotValue)) {
					maxRow = row;
					pivotValue = value;
				}
			}
			matrix = matrix.DivideRow(maxRow, pivotValue).SwapRows(column, maxRow);
			result = result.DivideRow(maxRow, pivotValue).SwapRows(column, maxRow);

			for (int row = column + 1; row < Rows; row++) {
				T value = matrix._data[IndexFrom(row, column)];

				matrix = matrix.AddMultipliedRow(row, column, -value);
				result = result.AddMultipliedRow(row, column, -value);
			}
		}

		for (int column = matrix.Columns - 1; column > 0; column--) {
			for (int row = 0; row < column; row++) {
				T value = matrix._data[IndexFrom(row, column)];
				matrix = matrix.AddMultipliedRow(row, column, -value);
				result = result.AddMultipliedRow(row, column, -value);
			}
		}

		return result;
	}

	public (Matrix<T> P, Matrix<T> L, Matrix<T> U) PLUDecomposition() {
		if (!IsSquare) throw new ArgumentException("Matrix should be square");
		if (Determinant() == T.Zero) throw new ArgumentException("Matrix is ​​not invertible");

		Matrix<T> p = Identity(Rows);
		Matrix<T> l = new(Rows, Columns);
		Matrix<T> u = Clone();

		for (int column = 0; column < Columns - 1; column++) {

			int maxRow = column;
			T pivotValue = u._data[IndexFrom(column, column)];
			for (int row = column + 1; row < Rows; row++) {
				T value = u._data[IndexFrom(row, column)];
				if (T.Abs(value) > T.Abs(pivotValue)) {
					maxRow = row;
					pivotValue = value;
				}
			}
			p = p.SwapRows(column, maxRow);
			l = l.SwapRows(column, maxRow);
			u = u.SwapRows(column, maxRow);

			for (int row = column + 1; row < Rows; row++) {
				T value = u._data[IndexFrom(row, column)];
				T factor = value / pivotValue;

				l._data[IndexFrom(row, column)] = factor;
				u = u.AddMultipliedRow(row, column, -factor);
			}
		}

		l = l.Add(Identity(Rows));
		return (p, l, u);
	}

	public bool Equals(Matrix<T> other) {
		if (Rows != other.Rows || Columns != other.Columns) return false;
		for (int row = 0; row < Rows; row++) {
			for (int column = 0; column < Columns; column++) {
				if (_data[IndexFrom(row, column)] != other._data[other.IndexFrom(row, column)]) return false;
			}
		}
		return true;
	}

	public override bool Equals(object? obj) => obj is Matrix<T> m && Equals(m);

	public override int GetHashCode() {
		HashCode hash = new();

		hash.Add(Rows);
		hash.Add(Columns);

		foreach (T value in _data) {
			hash.Add(value);
		}

		return hash.ToHashCode();
	}

	public override string ToString() {
		StringBuilder stringBuilder = new();

		for (int row = 0; row < Rows; row++) {
			stringBuilder.Append("[ ");

			for (int column = 0; column < Columns; column++) {
				stringBuilder.Append(_data[IndexFrom(row, column)]);
				if (column < Columns - 1) stringBuilder.Append(", ");
			}

			stringBuilder.AppendLine(" ]");
		}

		return stringBuilder.ToString();
	}

	private static int IndexFrom(int row, int column, int columns) => row * columns + column;

	private static void CheckValueBounds<TValue>(TValue value, TValue min, TValue max) where TValue : IComparable<TValue> {
		ArgumentOutOfRangeException.ThrowIfLessThan(value, min, nameof(value));
		ArgumentOutOfRangeException.ThrowIfGreaterThan(value, max, nameof(value));
	}

	private int IndexFrom(int row, int column) => IndexFrom(row, column, Columns);

	private void CheckBounds(int row, int column) {
		CheckRowBounds(row);
		CheckColumnBounds(column);
	}

	private void CheckRowBounds(int row) => CheckValueBounds(row, 0, Rows - 1);

	private void CheckColumnBounds(int column) => CheckValueBounds(column, 0, Columns - 1);
}
