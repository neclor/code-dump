package be.uliege.montefiore.oop;

import java.util.*;

public final class BruteForceSolver {

	private final PuzzleData data;
	private final List<Piece> corners;
	private final List<Piece> edges;
	private final List<Piece> inners;

	private Board board;

	private boolean[] usedCorners;
	private boolean[] usedEdges;
	private boolean[] usedInners;

	public BruteForceSolver(PuzzleData data) {
		this.data = data;
		corners = data.getCornerPieces();
		edges = data.getEdgePieces();
		inners = data.getInnerPieces();

		board = new Board(data.getWidth(), data.getHeight());
	}

	public Solution solve() {
		board = new Board(data.getWidth(), data.getHeight());
		usedCorners = new boolean[corners.size()];
		usedEdges = new boolean[edges.size()];
		usedInners = new boolean[inners.size()];

		if (
			corners.size() != 4 ||
			edges.size() != (data.getWidth() - 2 + data.getHeight() - 2) * 2 ||
			inners.size() != (data.getWidth() - 2) * (data.getHeight() - 2)
		) return null;

		if (!enumerateCorners()) return null;

		return new Solution(board.getWidth(), board.getHeight(), board.getAllPieces());
	}

	private boolean enumerateCorners() {
		boolean isAllUsed = true;
		for (int i = 0; i < corners.size(); i++) {
			if (usedCorners[i]) continue;
			isAllUsed = false;

			Piece piece = corners.get(i);
			usedCorners[i] = true;

			for (Piece.Rotation rotation : piece.getUniqueRotations()) {
				if (board.trySetNextCorner(piece, rotation)) {
					if (enumerateCorners()) return true;
					board.tryRemoveLastCorner();
				}
			}
			usedCorners[i] = false;
		}
		if (isAllUsed) return enumerateEdges();
		return false;
	}

	private boolean enumerateEdges() {
		boolean isAllUsed = true;
		for (int i = 0; i < edges.size(); i++) {
			if (usedEdges[i]) continue;
			isAllUsed = false;

			Piece piece = edges.get(i);
			usedEdges[i] = true;

			for (Piece.Rotation rotation : piece.getUniqueRotations()) {
				if (board.trySetNextEdge(piece, rotation)) {
					if (enumerateEdges()) return true;
					board.tryRemoveLastEdge();
				}
			}
			usedEdges[i] = false;
		}
		if (isAllUsed) return enumerateInners();
		return false;
	}

	private boolean enumerateInners() {
		boolean isAllUsed = true;
		for (int i = 0; i < inners.size(); i++) {
			if (usedInners[i]) continue;
			isAllUsed = false;

			Piece piece = inners.get(i);
			usedInners[i] = true;

			for (Piece.Rotation rotation : piece.getUniqueRotations()) {
				if (board.trySetNextInner(piece, rotation)) {
					if (enumerateInners()) return true;
					board.tryRemoveLastInner();
				}
			}
			usedInners[i] = false;
		}
		return isAllUsed;
	}
}
