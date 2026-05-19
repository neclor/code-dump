using Core.Common;
using Core.Common.Collision;
using Core.Common.Grid;

namespace Core.World.Objects;

public abstract class WorldObject : IGridObject, ICollidable {

	public Guid Id { get; set; } = Guid.NewGuid();

	public Grid<GridCell>? Grid { get => _internalGridObject.Grid; set => _internalGridObject.Grid = value; }
	public Vector2I Position { get => _internalGridObject.Position; set => _internalGridObject.Position = value; }

	public CollisionLayer CollisionLayer { get; set; } = CollisionLayer.Layer0;
	public CollisionLayer CollisionMask { get; set; } = CollisionLayer.Layer0;

	private readonly InternalGridObject _internalGridObject;

	protected WorldObject() => _internalGridObject = new(this);

	public bool CanSetPosition(int x, int y) => _internalGridObject.CanSetPosition(x, y);
	public bool CanShiftPosition(int dx, int dy) => _internalGridObject.CanShiftPosition(dx, dy);
	public bool TrySetPosition(int x, int y) => _internalGridObject.TrySetPosition(x, y);
	public bool TryShiftPosition(int dx, int dy) => _internalGridObject.TryShiftPosition(dx, dy);
	public bool CanSetPosition(Vector2I position) => _internalGridObject.CanSetPosition(position);
	public bool CanShiftPosition(Vector2I delta) => _internalGridObject.CanShiftPosition(delta);
	public bool TrySetPosition(Vector2I position) => _internalGridObject.TrySetPosition(position);
	public bool TryShiftPosition(Vector2I delta) => _internalGridObject.TryShiftPosition(delta);

	public bool CanCollideWith(ICollidable other) => CollisionLayerUtils.CanCollide(this, other);
}
