namespace Core.Common.Collision;

public static class CollisionLayerUtils {

	public static bool Intersects(CollisionLayer a, CollisionLayer b) => (a & b) != 0;

	public static bool Includes(CollisionLayer mask, CollisionLayer layer) => (mask & layer) == layer;

	public static CollisionLayer Combine(params CollisionLayer[] layers) {
		ArgumentNullException.ThrowIfNull(layers);

		CollisionLayer result = CollisionLayer.None;
		for (int i = 0; i < layers.Length; i++) {
			result |= layers[i];
		}
		return result;
	}

	public static CollisionLayer Exclude(CollisionLayer mask, CollisionLayer layer) => mask & ~layer;

	public static bool CanCollide(ICollidable a, ICollidable b) {
		ArgumentNullException.ThrowIfNull(a);
		ArgumentNullException.ThrowIfNull(b);

		return Intersects(a.CollisionLayer, b.CollisionMask) || Intersects(b.CollisionLayer, a.CollisionMask);
	}
}
