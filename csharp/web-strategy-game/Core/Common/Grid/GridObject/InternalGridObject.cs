namespace Core.Common.Grid;

public class InternalGridObject(IGridObject owner) : IGridObject {

	public Grid<GridCell>? Grid { get; set; }

	public Vector2I Position {
		get => _position;
		set => TrySetPosition(value);
	}

	private Vector2I _position = Vector2I.Zero;

	private readonly IGridObject _owner = owner;

	public bool CanSetPosition(int x, int y) {
		if (Grid == null) return true;
		if (!Grid.IsInBounds(x, y)) return false;

		return Grid[x, y].CanAddGridObject(_owner);
	}

	public bool CanShiftPosition(int dx, int dy) => CanSetPosition(_position.X + dx, _position.Y + dy);

	public bool TrySetPosition(int x, int y) {
		if (!CanSetPosition(x, y)) return false;

		Vector2I oldPosition = _position;
		_position = new(x, y);

		if (Grid == null) return true;

		Grid[oldPosition].RemoveGridObject(_owner);
		Grid[_position].TryAddGridObject(_owner);
		return true;
	}

	public bool TryShiftPosition(int dx, int dy) => TrySetPosition(_position.X + dx, _position.Y + dy);

	public bool CanSetPosition(Vector2I position) => CanSetPosition(position.X, position.Y);

	public bool CanShiftPosition(Vector2I delta) => CanShiftPosition(delta.X, delta.Y);

	public bool TrySetPosition(Vector2I position) => TrySetPosition(position.X, position.Y);

	public bool TryShiftPosition(Vector2I delta) => TryShiftPosition(delta.X, delta.Y);
}
