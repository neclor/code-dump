namespace Core.Common.Grid;

public interface IGridObject {

	Grid<GridCell>? Grid { get; set; }

	Vector2I Position { get; set; }

	bool CanSetPosition(int x, int y);

	bool CanShiftPosition(int dx, int dy);

	bool TrySetPosition(int x, int y);

	bool TryShiftPosition(int dx, int dy);

	bool CanSetPosition(Vector2I position);

	bool CanShiftPosition(Vector2I delta);

	bool TrySetPosition(Vector2I position);

	bool TryShiftPosition(Vector2I delta);
}
