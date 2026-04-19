package be.uliege.montefiore.oop;

public class PlacedPiece {

	private final Piece piece;
	private final Piece.Rotation rotation;

	public PlacedPiece(Piece piece, Piece.Rotation rotation) {
		if (piece == null) throw new IllegalArgumentException("Piece must not be null");

		this.piece = piece;
		this.rotation = rotation;
	}

	public Piece getPiece() { return piece; }

	public Piece.Rotation getRotation() { return rotation; }

	public Piece.Side getSide(Piece.Direction direction) { return piece.getSide(direction, rotation); }

	public boolean isCompatibleWith(Piece other, Piece.Rotation otherRotation, Piece.Direction otherPosition) { return piece.isCompatibleWith(rotation, other, otherRotation, otherPosition); }
	public boolean isCompatibleWith(PlacedPiece other, Piece.Direction otherPosition) {
		if (other == null) return true;
		return isCompatibleWith(other.getPiece(), other.getRotation(), otherPosition);
	}
}
