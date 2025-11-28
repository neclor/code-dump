from pysat.solvers import Glucose3
import random


def question1(M: int, N: int, i0: int, j0: int) -> tuple[list[list[int]], Glucose3, list[int]]:
    height, width, start_row, start_column = M, N, i0, j0

    board: ChessBoard = ChessBoard(height, width, start_row, start_column)

    return board.get_matrix_solution(), board.solver, board.get_variables()


def question3() -> int:
    height: int = 3
    width: int = 4

    solution_count: int = 0
    for row in range(height):
        for column in range(width):
            solution_count += len(ChessBoard(height, width, row, column).get_all_solutions())

    return solution_count


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


    height: int = 3
    width: int = 4

    solutions: list[list[list[int]]] = []
    for row in range(height):
        for column in range(width):
            solutions += ChessBoard(height, width, row, column).get_all_matrix_solutions()

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


    def __init__(self, height: int, width: int, start_row: int = 0, start_column: int = 0) -> None:
        if height <= 0: raise ValueError("Height must be positive.")
        if width <= 0: raise ValueError("Width must be positive.")
        if not (0 <= start_row < height): raise ValueError("Starting row out of bounds.")
        if not (0 <= start_column < width): raise ValueError("Starting column out of bounds.")

        self.height: int = height
        self.width: int = width
        self.start_row: int = start_row
        self.start_column: int = start_column

        self.cell_count: int = height * width
        self.black_cell_count: int = self.cell_count // 2
        self.white_cell_count: int = self.cell_count - self.black_cell_count

        self.start_cell_color: int = (start_row + start_column) % 2 # 0 - white, 1 - black

        self.var_count_for_even_turn: int = 0
        self.var_count_for_odd_turn: int = 0
        if self.start_cell_color == 1:
            self.var_count_for_even_turn = self.black_cell_count - 1
            self.var_count_for_odd_turn = self.white_cell_count
        else:
            self.var_count_for_even_turn = self.white_cell_count - 1
            self.var_count_for_odd_turn = self.black_cell_count

        self.solver: Glucose3 = Glucose3()
        self._add_constraints()


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


    def get_variables(self) -> list[int]:
        return [self.var_i(i, turn) for turn in range(1, self.cell_count) for i in range(self._get_turn_var_count(turn))]


    # Variable by coords
    def var_c(self, row: int, column: int, turn: int) -> int:
        if not (0 <= row < self.height and 0 <= column < self.width): raise ValueError("Row or column out of bounds.")
        if self._is_start_cell(row, column): raise ValueError("No variable for the starting position.")
        if self._get_cell_color(row, column) != self._get_turn_color(turn): raise ValueError("No variable for this cell at this turn.")

        index: int = (row * self.width + column) // 2
        if (self.start_cell_color == self._get_turn_color(turn)) and ((self.start_row < row) or (self.start_row == row and self.start_column < column)):
            index -= 1

        return self.var_i(index, turn)


    # Variable by index
    def var_i(self, index: int, turn: int) -> int:
        if not (0 < turn < self.cell_count): raise ValueError("Turn out of bounds.")
        if not (0 <= index < self._get_turn_var_count(turn)): raise ValueError("Index out of bounds.")
        return self.var_count_for_odd_turn * (turn // 2) + self.var_count_for_even_turn * ((turn - 1) // 2) + index + 1


    def _get_turn_var_count(self, turn: int) -> int:
        return self.var_count_for_even_turn if turn % 2 == 0 else self.var_count_for_odd_turn


    def _get_start_turn(self, row: int, column: int) -> int:
        return 2 if self.start_cell_color == self._get_cell_color(row, column) else 1


    def _get_cell_color(self, row: int, column: int) -> int:
        return (row + column) % 2


    def _get_turn_color(self, turn: int) -> int:
        return (self.start_cell_color + turn) % 2


    def _is_start_cell(self, row: int, column: int) -> bool:
        return row == self.start_row and column == self.start_column


    def _add_constraints(self) -> None:
        self._add_knight_move_constraints()
        self._add_turn_constraints()
        self._add_cell_constraints()


    # At each turn, exactly one cell is visited
    def _add_turn_constraints(self) -> None:
        for turn in range(1, self.cell_count):
            var_count: int = self._get_turn_var_count(turn)
            self.solver.add_clause([self.var_i(i, turn) for i in range(var_count)]) # At least one cell is visited

            for i in range(var_count - 1):
                for j in range(i + 1, var_count):
                    self.solver.add_clause([-self.var_i(i, turn), -self.var_i(j, turn)]) # At most one cell is visited


    # Each cell is visited exactly once
    def _add_cell_constraints(self) -> None:
        for row in range(self.height):
            for column in range(self.width):
                if self._is_start_cell(row, column): continue

                start_turn: int = self._get_start_turn(row, column)
                self.solver.add_clause([self.var_c(row, column, turn) for turn in range(start_turn, self.cell_count, 2)]) # Cell is visited at least once

                for turn_1 in range(start_turn, self.cell_count - 2, 2):
                    for turn_2 in range(turn_1 + 2, self.cell_count, 2):
                        self.solver.add_clause([-self.var_c(row, column, turn_1), -self.var_c(row, column, turn_2)]) # Cell is visited at most once


    # Valid knight moves
    def _add_knight_move_constraints(self) -> None:
        for row in range(self.height):
            for column in range(self.width):

                start_turn: int = 0
                cell_count: int = 0
                if self._is_start_cell(row, column):
                    start_turn = 0
                    cell_count = 1
                else:
                    start_turn = self._get_start_turn(row, column)
                    cell_count = self.cell_count - 1

                for turn in range(start_turn, cell_count, 2):
                    next_positions: list[int] = []
                    for delta_row, delta_column in self.KNIGHT_MOVES:
                        next_row: int = row + delta_row
                        next_column: int = column + delta_column
                        if (
                            (not (0 <= next_row < self.height and 0 <= next_column < self.width)) or
                            (next_row == self.start_row and next_column == self.start_column)
                        ): continue
                        next_positions.append(self.var_c(next_row, next_column, turn + 1))

                    if self._is_start_cell(row, column):
                        self.solver.add_clause(next_positions)
                    else:
                        self.solver.add_clause([-self.var_c(row, column, turn)] + next_positions)


    def _model_to_solution(self, model: list[int]) -> list[tuple[int, int]]:
        solution: list[tuple[int, int]] = [(self.start_row, self.start_column)]
        for turn in range(1, self.cell_count):
            for row in range(self.height):
                for column in range(self.width):
                    if (
                        self._get_cell_color(row, column) != self._get_turn_color(turn) or
                        self._is_start_cell(row, column)
                    ): continue

                    if model[self.var_c(row, column, turn) - 1] > 0:
                        solution.append((row, column))
        return solution


    def _list_to_matrix(self, solution: list[tuple[int, int]]) -> list[list[int]]:
        matrix_solution: list[list[int]] = [[-1 for _ in range(self.width)] for _ in range(self.height)]
        for i, (row, column) in enumerate(solution):
            matrix_solution[row][column] = i
        return matrix_solution


def main() -> None:
    import time
    board: ChessBoard = ChessBoard(8, 8)

    start_time = time.time()
    board.solver.solve()
    end_time = time.time()

    execution_time = end_time - start_time
    print(f"{execution_time:.4f}s")


def var_test() -> None:
    board = ChessBoard(4, 5, 2, 2)
    for turn in range(1, board.cell_count):
        matrix: list[list] = []
        for row in range(board.height):
            matrix_row: list = []
            for column in range(board.width):
                if board._get_cell_color(row, column) != board._get_turn_color(turn):
                    matrix_row.append("_")
                elif board._is_start_cell(row, column):
                    matrix_row.append("s")
                else:
                    matrix_row.append(str(board.var_c(row, column, turn)))

            matrix.append(matrix_row)

        for line in matrix:
            print(line)
        print()

if __name__ == '__main__': main()
