package be.uliege.montefiore.oop;

import java.util.*;
import java.util.function.*;

public class PuzzleData {

	private final int width;
	private final int height;
	private final Piece[] pieces;

	public PuzzleData(int width, int height, Collection<Piece> pieces) {
		if (width <= 0 || height <= 0) throw new IllegalArgumentException("Width and height must be positive");
		if (pieces == null || pieces.size() != width * height) throw new IllegalArgumentException("Pieces collection must match width * height");

		this.width = width;
		this.height = height;
		this.pieces = pieces.toArray(new Piece[0]);
	}

	public int getWidth() { return width; }

	public int getHeight() { return height; }

	public List<Piece> getAllPieces() { return Collections.unmodifiableList(Arrays.asList(pieces)); }

	public List<Piece> getCornerPieces() { return filterPieces(Piece::isCorner); }
	public List<Piece> getEdgePieces() { return filterPieces(Piece::isEdge); }
	public List<Piece> getInnerPieces() { return filterPieces(Piece::isInner); }
	private List<Piece> filterPieces(Predicate<Piece> condition) {
		ArrayList<Piece> result = new ArrayList<>();
		for (Piece piece : pieces) {
			if (condition.test(piece)) {
				result.add(piece);
			}
		}
		return Collections.unmodifiableList(result);
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(width).append(" ").append(height).append("\n");
		for (Piece piece : pieces) {
			sb.append(piece).append("\n");
		}
		return sb.toString();
	}
}
