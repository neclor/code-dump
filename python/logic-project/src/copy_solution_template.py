from pysat.solvers import Glucose3
import random


def question1(M: int, N: int, i0: int, j0: int) -> tuple[list[list[int]], Glucose3, list[int]]:
    height, width, start_row, start_column = M, N, i0, j0

    board: ChessBoard = ChessBoard(height, width, start_row, start_column)

    return board.get_matrix_solution(), board.solver, board.get_variables()


def question3() -> int:
    return len(ChessBoard(3, 4).get_all_solutions())


def question4() -> int:

    def solutions_equal(a: list[list[int]], b: list[list[int]]) -> bool:
        for i in range(len(a)):
            for j in range(len(a[0])):
                if a[i][j] != b[i][j]: return False
        return True


    def fliped_horizontally(solution: list[list[int]]) -> list[list[int]]:
        new_solution: list[list[int]] = []
        for line in solution:
            new_solution.append(line[::-1])
        return new_solution


    def fliped_vertically(solution: list[list[int]]) -> list[list[int]]:
        new_solution: list[list[int]] = []
        for line in solution[::-1]:
            new_solution.append(line[:])
        return new_solution


    def rotated_180(solution: list[list[int]]) -> list[list[int]]:
        return fliped_horizontally(fliped_vertically(solution))


    def check_symetry(a: list[list[int]], b: list[list[int]]) -> bool:
        if (
            solutions_equal(a, b) or
            solutions_equal(fliped_horizontally(a), b) or
            solutions_equal(fliped_vertically(a), b) or
            solutions_equal(rotated_180(a), b)
            ): return True
        return False


    board: ChessBoard = ChessBoard(3, 4)
    solutions: list[list[list[int]]] = board.get_all_matrix_solutions()

    i: int = 0
    while (i < len(solutions) - 1):
        for j in range(len(solutions) - 1, i, -1):
            if check_symetry(solutions[i], solutions[j]): del solutions[j]
        i += 1

    return len(solutions)


def question5(M: int, N: int, i0: int, j0: int) -> list[tuple[int, int, int]]:
    height, width, start_row, start_column = M, N, i0, j0

    board: ChessBoard = ChessBoard(height, width, start_row, start_column)
    solutions: list[list[tuple[int, int]]] = board.get_all_solutions()

    if len(solutions) == 0: return []

    random_index: int = random.randrange(len(solutions))
    main_solution: list[tuple[int, int]] = solutions[random_index]
    del solutions[random_index]

    print(len(solutions))




    constraints: list[tuple[int, int, int]] = [(1, 1, 1)]
    # YOUR CODE HERE
    return constraints


class ChessBoard:
    KNIGHT_MOVES: list[tuple[int, int]] = [(1, 2), (2, 1), (2, -1), (1, -2), (-1, -2), (-2, -1), (-2, 1), (-1, 2)]


    def __init__(self, height: int, width: int, start_row: int | None = None, start_column: int | None = None) -> None:
        if height <= 0: raise ValueError("Height must be positive.")
        if width <= 0: raise ValueError("Width must be positive.")

        self.height: int = height
        self.width: int = width
        self.cell_count: int = height * width
        self.solver: Glucose3 = Glucose3()

        if start_row is not None and start_column is not None:
            self.set_start_position(start_row, start_column)
        self._add_constraints()


    def reset_solver(self) -> None:
        self.solver = Glucose3()
        self._add_constraints()


    def set_start_position(self, start_row: int = 0, start_column: int = 0) -> None:
        if not (0 <= start_row < self.height): raise ValueError("Starting row out of bounds.")
        if not (0 <= start_column < self.width): raise ValueError("Starting column out of bounds.")
        self.solver.add_clause([self.var_c(start_row, start_column, 0)])


    # Variable by coords
    def var_c(self, row: int, column: int, turn: int) -> int:
        if not (0 <= row < self.height and 0 <= column < self.width): raise ValueError("Row or column out of bounds.")
        return self.var_i(row * self.width + column, turn)


    # Variable by index
    def var_i(self, index: int, turn: int) -> int:
        if not (0 <= index < self.cell_count and 0 <= turn < self.cell_count): raise ValueError("Index or turn out of bounds.")
        return turn * self.cell_count + index + 1


    def get_variables(self) -> list[int]:
        return [self.var_i(i, turn) for i in range(self.cell_count) for turn in range(self.cell_count)]


    def get_solution(self) -> list[tuple[int, int]]:
        if not self.solver.solve(): return []
        model: list[int] | None = self.solver.get_model()
        if model is None: return []
        return self._model_to_solution(model)


    def get_all_solutions(self) -> list[list[tuple[int, int]]]:
        solutions: list[list[tuple[int, int]]] = []
        while self.solver.solve():
            solutions.append(self.get_solution())
            model: list[int] | None = self.solver.get_model()
            if model is None: break
            self.solver.add_clause([-var for var in model])
        return solutions


    def get_matrix_solution(self) -> list[list[int]]:
        return self._list_to_matrix(self.get_solution())


    def get_all_matrix_solutions(self) -> list[list[list[int]]]:
        return [self._list_to_matrix(solution) for solution in self.get_all_solutions()]


    def _model_to_solution(self, model: list[int]) -> list[tuple[int, int]]:
        solution: list[tuple[int, int]] = []
        for turn in range(self.cell_count):
            for row in range(self.height):
                for column in range(self.width):
                    if model[self.var_c(row, column, turn) - 1] > 0:
                        solution.append((row, column))
        return solution


    def _list_to_matrix(self, solution: list[tuple[int, int]]) -> list[list[int]]:
        matrix_solution: list[list[int]] = [[-1 for _ in range(self.width)] for _ in range(self.height)]
        for i, (row, column) in enumerate(solution):
            matrix_solution[row][column] = i
        return matrix_solution


    def _add_constraints(self) -> None:
        self._add_turn_constraints()
        self._add_knight_move_constraints()
        self._add_cell_constraints()


    # At each turn, exactly one cell is visited
    def _add_turn_constraints(self) -> None:
        for turn in range(self.cell_count):
            self.solver.add_clause([self.var_i(i, turn) for i in range(self.cell_count)]) # At least one cell is visited

            for i in range(self.cell_count - 1):
                for j in range(i + 1, self.cell_count):
                    self.solver.add_clause([-self.var_i(i, turn), -self.var_i(j, turn)]) # At most one cell is visited


    # Each cell is visited exactly once
    def _add_cell_constraints(self) -> None:
        for i in range(self.cell_count):
            self.solver.add_clause([self.var_i(i, turn) for turn in range(self.cell_count)]) # Cell is visited at least once

            for turn_1 in range(self.cell_count - 1):
                for turn_2 in range(turn_1 + 1, self.cell_count):
                    self.solver.add_clause([-self.var_i(i, turn_1), -self.var_i(i, turn_2)]) # Cell is visited at most once


    # Valid knight moves
    def _add_knight_move_constraints(self) -> None:
        for turn in range(self.cell_count - 1):
            for row in range(self.height):
                for column in range(self.width):

                    next_positions: list[int] = []
                    for delta_row, delta_column in self.KNIGHT_MOVES:
                        next_row: int = row + delta_row
                        next_column: int = column + delta_column
                        if not (0 <= next_row < self.height and 0 <= next_column < self.width): continue
                        next_positions.append(self.var_c(next_row, next_column, turn + 1))

                    self.solver.add_clause([-self.var_c(row, column, turn)] + next_positions)


def main() -> None:
    import time
    board: ChessBoard = ChessBoard(8, 8, 0, 0)

    start_time = time.time()
    board.solver.solve()
    end_time = time.time()

    execution_time = end_time - start_time
    print(f"{execution_time:.4f}s")


if __name__ == '__main__': main()
