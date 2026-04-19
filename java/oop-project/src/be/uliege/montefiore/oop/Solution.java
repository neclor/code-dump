package be.uliege.montefiore.oop;

import java.util.*;

public final class Solution {

	private final int width;
	private final int height;
	private final PlacedPiece[][] pieces;

	public Solution(int width, int height, List<List<PlacedPiece>> pieces) {
		if (width <= 0 || height <= 0) throw new IllegalArgumentException("Width and height must be positive");
		if (pieces == null || pieces.size() != height) throw new IllegalArgumentException("Pieces list must match height");

		this.width = width;
		this.height = height;
		this.pieces = new PlacedPiece[height][width];
		for (int y = 0; y < height; y++) {
			List<PlacedPiece> row = pieces.get(y);
			if (row == null || row.size() != width) throw new IllegalArgumentException("Number of columns in row " + y + " must match width");
			this.pieces[y] = row.toArray(new PlacedPiece[0]);
		}
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();

		for (PlacedPiece[] row : pieces) {
			for (PlacedPiece piece : row) {
				sb.append(piece.getPiece().getIndex()).append(" ").append(piece.getRotation().ordinal()).append('\n');;
			}
		}
		return sb.toString();
	}

	public void visualize() {
		int canvasHeight = height * 5;
		int canvasWidth = width * 5;
		char[][] canvas = new char[canvasHeight][canvasWidth];

		for (int y = 0; y < canvasHeight; y++) {
			for (int x = 0; x < canvasWidth; x++) {
				canvas[y][x] = ' ';
			}
		}

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				drawPiece(canvas, x, y, pieces[y][x]);
			}
		}

		for (char[] row : canvas) {
			System.out.println(row);
		}
	}

	private void drawPiece(char[][] canvas, int x, int y, PlacedPiece piece) {
		x = x * 5;
		y = y * 5;
		for (int i = 1; i <= 3; i++) {
			for (int j = 1; j <= 3; j++) {
				canvas[y + i][x + j] = '█';
			}
		}

		switch (piece.getSide(Piece.Direction.UP)) {
			case BUMP:
				canvas[y][x + 2] = '█';
				break;
			case PIT:
				canvas[y + 1][x + 2] = ' ';
				break;
			default: break;
		}

		switch (piece.getSide(Piece.Direction.RIGHT)) {
			case BUMP:
				canvas[y + 2][x + 4] = '█';
				break;
			case PIT:
				canvas[y + 2][x + 3] = ' ';
				break;
			default: break;
		}

		switch (piece.getSide(Piece.Direction.DOWN)) {
			case BUMP:
				canvas[y + 4][x + 2] = '█';
				break;
			case PIT:
				canvas[y + 3][x + 2] = ' ';
				break;
			default: break;
		}

		switch (piece.getSide(Piece.Direction.LEFT)) {
			case BUMP:
				canvas[y + 2][x] = '█';
				break;
			case PIT:
				canvas[y + 2][x + 1] = ' ';
				break;
			default: break;
		}
	}
}
