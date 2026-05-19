namespace Core.Common.Collision;

public interface ICollidable {

	CollisionLayer CollisionLayer { get; set; }

	CollisionLayer CollisionMask { get; set; }

	bool CanCollideWith(ICollidable other);
}
