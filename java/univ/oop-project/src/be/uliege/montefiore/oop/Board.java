package be.uliege.montefiore.oop;

import java.util.*;

public final class Board {

	private final int[][] corners;
	private final int[][] edges;

	private final int width;
	private final int height;
	private final PlacedPiece[][] pieces;

	public Board(int width, int height) {
		if (width <= 0 || height <= 0) throw new IllegalArgumentException("Width and height must be positive");

		this.width = width;
		this.height = height;
		this.pieces = new PlacedPiece[height][width];

		corners = new int[][] { { 0, 0 }, { width - 1, 0 }, { width - 1, height - 1 }, { 0, height - 1 } };
		edges = new int[(width - 2 + height - 2) * 2][];
		int i = 0;
		for (int x = 1; x < width - 1; x++) {
			edges[i++] = new int[] {x, 0};
			edges[i++] = new int[] {x, height - 1};
		}
		for (int y = 1; y < height - 1; y++) {
			edges[i++] = new int[] {0, y};
			edges[i++] = new int[] {width - 1, y};
		}
	}

	public int getWidth() { return width; }

	public int getHeight() { return height; }

	public boolean hasPiece(int x, int y) { return getPiece(x, y) != null; }

	public PlacedPiece getPiece(int x, int y) {
		validateBounds(x, y);
		return pieces[y][x];
	}

	public List<List<PlacedPiece>> getAllPieces() {
		ArrayList<List<PlacedPiece>> result = new ArrayList<>();
		for (PlacedPiece[] row : pieces) {
			result.add(Collections.unmodifiableList(Arrays.asList(row)));
		}
		return Collections.unmodifiableList(result);
	}

	public boolean tryRemovePiece(int x, int y) {
		if (!hasPiece(x, y)) return false;
		pieces[y][x] = null;
		return true;
	}

	public boolean trySetPiece(int x, int y, Piece piece, Piece.Rotation rotation) {
		validateBounds(x, y);
		if (piece == null) {
			pieces[y][x] = null;
			return true;
		}

		if (!isCompatible(x, y - 1, piece, rotation, Piece.Direction.UP)) return false;
		if (!isCompatible(x + 1, y, piece, rotation, Piece.Direction.RIGHT)) return false;
		if (!isCompatible(x, y + 1, piece, rotation, Piece.Direction.DOWN)) return false;
		if (!isCompatible(x - 1, y, piece, rotation, Piece.Direction.LEFT)) return false;

		pieces[y][x] = new PlacedPiece(piece, rotation);
		return true;
	}

	public boolean trySetNextCorner(Piece piece, Piece.Rotation rotation) {
		if (!piece.isCorner()) return false;
		for (int[] corner : corners) {
			int x = corner[0], y = corner[1];
			if (!hasPiece(x, y)) return trySetPiece(x, y, piece, rotation);
		}
		return false;
	}

	public boolean tryRemoveLastCorner() {
		for (int i = corners.length - 1; i >= 0; i--) {
			int x = corners[i][0], y = corners[i][1];
			if (tryRemovePiece(x, y)) return true;
		}
		return false;
	}

	public boolean trySetNextEdge(Piece piece, Piece.Rotation rotation) {
		if (!piece.isEdge()) return false;
		for (int[] edge : edges) {
			int x = edge[0], y = edge[1];
			if (!hasPiece(x, y)) return trySetPiece(x, y, piece, rotation);
		}
		return false;
	}

	public boolean tryRemoveLastEdge() {
		for (int i = edges.length - 1; i >= 0; i--) {
			int x = edges[i][0], y = edges[i][1];
			if (tryRemovePiece(x, y)) return true;
		}
		return false;
	}

	public boolean trySetNextInner(Piece piece, Piece.Rotation rotation) {
		if (!piece.isInner()) return false;
		for (int y = 1; y < height - 1; y++) {
			for (int x = 1; x < width - 1; x++) {
				if (!hasPiece(x, y)) return trySetPiece(x, y, piece, rotation);
			}
		}
		return false;
	}

	public boolean tryRemoveLastInner() {
		for (int y = height - 2; y > 0 ; y--) {
			for (int x = width - 2; x > 0 ; x--) {
				if (tryRemovePiece(x, y)) return true;
			}
		}
		return false;
	}

	public boolean isInBounds(int x, int y) { return 0 <= x && x < width && 0 <= y && y < height; }

	private boolean isCompatible(int x, int y, Piece piece, Piece.Rotation rotation, Piece.Direction direction) {
		if (isInBounds(x, y)) {
			PlacedPiece placedPiece = getPiece(x, y);
			if (placedPiece == null) { return true; }
			return piece.isCompatibleWith(rotation, placedPiece.getPiece(), placedPiece.getRotation(), direction);
		} else {
			return piece.getSide(direction, rotation) == Piece.Side.FLAT;
		}
	}

	private void validateBounds(int x, int y) { if (!isInBounds(x, y)) throw new IllegalArgumentException("Position out of range"); }
}
