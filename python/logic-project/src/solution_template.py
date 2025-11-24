from pysat.solvers import Glucose3
import random


KNIGHT_MOVES: list[tuple[int, int]] = [(1, 2), (2, 1), (2, -1), (1, -2), (-1, -2), (-2, -1), (-2, 1), (-1, 2)]


def question1(M: int, N: int, i0: int, j0: int) -> tuple[list[list[int]], Glucose3, list[int]]:
    height, width, row_0, column_0 = M, N, i0, j0
    check_bounds(height, width, row_0, column_0)

    solver: Glucose3 = Glucose3()
    add_constraints(solver, height, width)
    solver.add_clause([var_c(height, width, row_0, column_0, 0)])

    solution: list[list[int]] = get_solution(solver, height, width)

    cell_count: int = height * width
    variables: list[int] = [var_i(cell_count, i, turn) for i in range(cell_count) for turn in range(cell_count)]
    return solution, solver, variables


def question3() -> int:
    height: int = 3
    width: int = 4
    solution_count: int = 0
    for row in range(height):
        for column in range(width):

            solver: Glucose3 = Glucose3()
            add_constraints(solver, height, width)
            solver.add_clause([var_c(height, width, row, column, 0)])

            while True:
                if not solver.solve(): break
                model: list[int] | None = solver.get_model()
                if model is None: break
                solver.add_clause([-var for var in model])
                solution_count += 1

    return solution_count


def question4():
    return question3() / 4


def question5(M: int, N: int, i0: int, j0: int) -> list[tuple[int, int, int]]:
    height, width, row_0, column_0 = M, N, i0, j0
    check_bounds(height, width, row_0, column_0)
    solver: Glucose3 = Glucose3()
    add_constraints(solver, height, width)
    solver.add_clause([var_c(height, width, row_0, column_0, 0)])
    constraints: list[tuple[int, int, int]] = [(1, 1, 1)]
    # YOUR CODE HERE
    return constraints


def add_constraints(solver: Glucose3, height: int, width: int) -> None:
    add_turn_constraints(solver, height, width)
    add_cell_constraints(solver, height, width)
    add_knight_move_constraints(solver, height, width)


# At each turn, only one cell is visited
def add_turn_constraints(solver: Glucose3, height: int, width: int) -> None:
    cell_count: int = height * width

    for turn in range(cell_count):
        solver.add_clause([var_i(cell_count, i, turn) for i in range(cell_count)]) # At least one cell is visited

        for i in range(cell_count - 1):
            for j in range(i + 1, cell_count):
                solver.add_clause([-var_i(cell_count, i, turn), -var_i(cell_count, j, turn)]) # At most one cell is visited


# Each cell is visited exactly once
def add_cell_constraints(solver: Glucose3, height: int, width: int) -> None:
    cell_count: int = height * width

    for i in range(cell_count):
        solver.add_clause([var_i(cell_count, i, turn) for turn in range(cell_count)]) # Cell is visited at least once

        for turn_1 in range(cell_count - 1):
            for turn_2 in range(turn_1 + 1, cell_count):
                solver.add_clause([-var_i(cell_count, i, turn_1), -var_i(cell_count, i, turn_2)]) # Cell is visited at most once


# Valid knight moves
def add_knight_move_constraints(solver: Glucose3, height: int, width: int) -> None:
    cell_count: int = height * width

    for turn in range(cell_count - 1):
        for row_1 in range(height):
            for column_1 in range(width):

                next_positions: list[int] = []
                for delta_row, delta_column in KNIGHT_MOVES:
                    row_2: int = row_1 + delta_row
                    column_2: int = column_1 + delta_column
                    if not (0 <= row_2 < height and 0 <= column_2 < width): continue
                    next_positions.append(var_c(height, width, row_2, column_2, turn + 1))

                solver.add_clause([-var_c(height, width, row_1, column_1, turn)] + next_positions)


def get_solution(solver: Glucose3, height: int, width: int) -> list[list[int]]:
    cell_count: int = height * width
    solution: list[list[int]] = [[-1 for _ in range(width)] for _ in range(height)]
    if solver.solve():
        model: list[int] | None = solver.get_model()
        if model is not None:
            for turn in range(cell_count):
                for row in range(height):
                    for column in range(width):
                        if model[var_c(height, width, row, column, turn) - 1] > 0:
                            solution[row][column] = turn
    return solution


# Variable by coords
def var_c(height: int, width: int, row: int, column: int, turn: int) -> int:
    if not (0 <= row < height and 0 <= column < width): return 0
    return var_i(height * width, row * width + column, turn)


# Variable by index
def var_i(cell_count: int, index: int, turn: int) -> int:
    if not (0 <= index < cell_count): return 0
    return turn * cell_count + index + 1


def check_bounds(height: int, width: int, row_0: int, column_0: int) -> None:
    if height <= 0: raise ValueError("Height must be positive.")
    if width <= 0: raise ValueError("Width must be positive.")
    if not (0 <= row_0 < height): raise ValueError("Starting row out of bounds.")
    if not (0 <= column_0 < width): raise ValueError("Starting column out of bounds.")




class ChessBoard:
    KNIGHT_MOVES: list[tuple[int, int]] = [(1, 2), (2, 1), (2, -1), (1, -2), (-1, -2), (-2, -1), (-2, 1), (-1, 2)]


    def __init__(self, height: int, width: int, start_row: int, start_column: int) -> None:
        if height <= 0: raise ValueError("Height must be positive.")
        if width <= 0: raise ValueError("Width must be positive.")
        if not (0 <= start_row < height): raise ValueError("Starting row out of bounds.")
        if not (0 <= start_column < width): raise ValueError("Starting column out of bounds.")

        self.height: int = height
        self.width: int = width
        self.start_row: int = start_row
        self.start_column: int = start_column
        self.cell_count: int = height * width
        self.solver: Glucose3 = Glucose3()

        self._add_constraints()


    def _add_constraints(self) -> None:
        for row in range(self.height):
            for column in range(self.width):
                self._cell_add_constraints(row, column)


    def _cell_add_constraints(self, row: int, column: int) -> None:

        if row == self.start_row and column == self.start_column:





        pass






    def _var_c(self, row: int, column: int, move: tuple[int, int]) -> int:
        if not (0 <= row < self.height and 0 <= column < self.width): raise ValueError("Row or column out of bounds.")
        return self._var_i(row * self.width + column, move)


    def _var_i(self, index: int, move: tuple[int, int]) -> int:
        if not (0 <= index < self.cell_count and move in self.KNIGHT_MOVES): raise ValueError("Index out of bounds or invalid move.")
        return (len(self.KNIGHT_MOVES) + 1) * index + self.KNIGHT_MOVES.index(move) + 1


    def _reverse_move(self, move: tuple[int, int]) -> tuple[int, int]:
        if move not in self.KNIGHT_MOVES: raise ValueError("Invalid knight move.")
        return (-move[0], -move[1])
