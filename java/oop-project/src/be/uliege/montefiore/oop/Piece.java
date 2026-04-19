package be.uliege.montefiore.oop;

import java.util.*;

public final class Piece {

	public enum Side {
		BUMP,
		FLAT,
		PIT
	}

	public enum Rotation {
		NONE,
		CLOCKWISE,
		UPSIDE_DOWN,
		COUNTERCLOCKWISE
	}

	public enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	}

	private static final int SIDE_COUNT = 4;

	private final int index;
	private final Side[] sides;

	private List<Rotation> cachedUniqueRotations;

	public Piece(int index, Collection<Side> sides) {
		if (index < 0) throw new IllegalArgumentException("Index must be non-negative");
		if (sides == null || sides.size() != SIDE_COUNT) throw new IllegalArgumentException("Sides must have exactly " + SIDE_COUNT + " elements");

		this.index = index;
		this.sides = sides.toArray(new Side[0]);
	}

	public boolean isCorner() { return flatCount() == 2; }
	public boolean isEdge() { return flatCount() == 1; }
	public boolean isInner() { return flatCount() == 0; }

	public int getIndex() { return index; }

	public Side getSide(Direction direction) { return getSide(direction, Rotation.NONE); }
	public Side getSide(Direction direction, Rotation rotation) { return sides[(direction.ordinal() + SIDE_COUNT - rotation.ordinal()) % SIDE_COUNT]; }

	public List<Side> getSides() { return getSides(Rotation.NONE); }
	public List<Side> getSides(Rotation rotation) {
		ArrayList<Side> turnedSides = new ArrayList<>();
		for (int i = 0; i < SIDE_COUNT; i++) {
			turnedSides.add(sides[(i + SIDE_COUNT - rotation.ordinal()) % SIDE_COUNT]);
		}
		return Collections.unmodifiableList(turnedSides);
	}

	public boolean isCompatibleWith(Rotation thisRotation, Piece other, Rotation otherRotation, Direction otherPosition) {
		if (other == null) return true;

		Side a = getSide(otherPosition, thisRotation);
		Direction opposite = Direction.values()[(otherPosition.ordinal() + 2) % SIDE_COUNT];
		Side b = other.getSide(opposite, otherRotation);

		return ((a == Side.BUMP && b == Side.PIT) || (a == Side.PIT && b == Side.BUMP));
	}

	public List<Piece.Rotation> getUniqueRotations() {
		if (cachedUniqueRotations != null) return cachedUniqueRotations;

		ArrayList<Rotation> unique = new ArrayList<>();
		for (Rotation i : Rotation.values()) {
			boolean inUnique = false;
			for (Rotation j : unique) {
				if (getSides(i).equals(getSides(j))) {
					inUnique = true;
					break;
				}
			}
			if (inUnique) continue;
			unique.add(i);
		}
		cachedUniqueRotations = Collections.unmodifiableList(unique);
		return cachedUniqueRotations;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for (Piece.Side side : getSides()) {
			switch (side) {
				case BUMP: sb.append("B"); break;
				case FLAT: sb.append("F"); break;
				case PIT: sb.append("P"); break;
			}
		}
		return sb.toString();
	}

	private int flatCount() {
		int count = 0;
		for (Side side : sides) {
			if (side == Side.FLAT) {
				count++;
			}
		}
		return count;
	}
}
