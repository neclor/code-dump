package be.uliege.montefiore.oop;

import java.io.*;
import java.util.*;
import java.util.regex.*;
import java.nio.file.*;
import java.util.stream.*;

public final class Parser {

	private static final Pattern sizePattern = Pattern.compile("^(\\d+)\\s+(\\d+)$");
	private static final Pattern piecePattern = Pattern.compile("^[PFB]{4}$");

	public static PuzzleData parseFile(Path path) {
		if (!Files.exists(path)) throw new IllegalArgumentException("File not found: " + path);

		try {
			return parseLines(Files.lines(path));
		} catch (IOException e) {
			throw new IllegalArgumentException("Error reading file: " + path, e);
		}
	}

	public static PuzzleData parseLines(Stream<String> lines) {
		Iterator<String> iterator = lines.iterator();

		String firstLine = iterator.next();

		Matcher matcher = sizePattern.matcher(firstLine);
		if (!matcher.matches()) {
			throw new IllegalArgumentException("Header parsing error");
		}

		int columns = Integer.parseInt(matcher.group(1));
		int rows = Integer.parseInt(matcher.group(2));
		if (columns <= 0 || rows <= 0) {
			throw new IllegalArgumentException("Invalid puzzle size: " + columns + "*" + rows);
		}

		ArrayList<Piece> pieces = new ArrayList<>();

		while (iterator.hasNext()) {
			String line = iterator.next();

			matcher = piecePattern.matcher(line);
			if (!matcher.matches()) {
				throw new IllegalArgumentException("DataObjects.Piece parsing error: " + line);
			}

			List<Piece.Side> sides = Arrays.asList(parsePieceLine(line));
			pieces.add(new Piece(pieces.size() + 1, sides));
		}

		return new PuzzleData(columns, rows, pieces);
	}

	private static Piece.Side[] parsePieceLine(String line) {
		Piece.Side[] sides = new Piece.Side[line.length()];
		for (int i = 0; i < line.length(); i++) {
			switch (line.charAt(i)) {
				case 'B':
					sides[i] = Piece.Side.BUMP;
					break;
				case 'F':
					sides[i] = Piece.Side.FLAT;
					break;
				case 'P':
					sides[i] = Piece.Side.PIT;
					break;
				default:
					throw new IllegalArgumentException("Invalid char in line: " + line.charAt(i));
			}
		}
		return sides;
	}
}
