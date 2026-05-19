using Core.Common.Collision;

namespace Core.Common.Grid;

public class GridCell {

	public IReadOnlyList<IGridObject> GridObjects => _gridObjects;

	private readonly List<IGridObject> _gridObjects = [];

	public bool CanAddGridObject(IGridObject gridObject) {
		ArgumentNullException.ThrowIfNull(gridObject);
		if (ContainsGridObject(gridObject)) return false;
		if (gridObject is not ICollidable collidable) return true;

		foreach (ICollidable other in _gridObjects.OfType<ICollidable>()) {
			if (collidable.CanCollideWith(other)) return false;
		}
		return true;
	}

	public bool TryAddGridObject(IGridObject gridObject) {
		if (!CanAddGridObject(gridObject)) return false;

		_gridObjects.Add(gridObject);
		return true;
	}

	public void ClearGridObjects() => _gridObjects.Clear();

	public bool ContainsGridObject(IGridObject gridObject) => _gridObjects.Contains(gridObject);

	public bool RemoveGridObject(IGridObject gridObject) => _gridObjects.Remove(gridObject);
}
