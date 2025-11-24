from pysat.solvers import Glucose3
import time


KNIGHT_MOVES: list[tuple[int, int]] = [(1, 2), (2, 1), (2, -1), (1, -2), (-1, -2), (-2, -1), (-2, 1), (-1, 2)]


def main() -> None:
    test_function(solution_1)


def test_function(function) -> None:
    start_time = time.time()
    function(8, 8, 0, 0)
    end_time = time.time()
    execution_time = end_time - start_time
    print(f"{function.__name__} - {execution_time:.4f}s")


def solution_1(height: int, width: int, row_0: int, column_0: int) -> tuple[list[list[int]], Glucose3, list[int]]:
    check_bounds(height, width, row_0, column_0)

    solver: Glucose3 = Glucose3()
    add_constraints(solver, height, width)
    solver.add_clause([var_c(height, width, row_0, column_0, 0)])

    solution: list[list[int]] = get_solution(solver, height, width)

    cell_count: int = height * width
    variables: list[int] = [var_i(cell_count, i, turn) for i in range(cell_count) for turn in range(cell_count)]
    return solution, solver, variables


def check_bounds(height: int, width: int, row_0: int, column_0: int) -> None:
    if height <= 0: raise ValueError("Height must be positive.")
    if width <= 0: raise ValueError("Width must be positive.")
    if not (0 <= row_0 < height): raise ValueError("Starting row out of bounds.")
    if not (0 <= column_0 < width): raise ValueError("Starting column out of bounds.")


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


if __name__ == '__main__': main()
