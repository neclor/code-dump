package be.uliege.montefiore.oop;

import java.nio.file.*;

public class Puzzle {

	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("usage: java Puzzle <file>");
			return;
		}

		try {
			PuzzleData data = Parser.parseFile(Paths.get(args[0]));
			BruteForceSolver solver = new BruteForceSolver(data);
			Solution solution = solver.solve();
			if (solution == null) {
				System.out.println("No solution found");
				return;
			}
			System.out.println(solution);

		} catch (Exception e) {
			System.out.println("Error: " + e.getMessage());
		}
	}
}
