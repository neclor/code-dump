from pysat.solvers import Glucose3
import time


class RowColumnSolution:

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
        self.size_sum: int = height + width
        self.cell_count: int = height * width
        self.solver: Glucose3 = Glucose3()

        self._add_constraints()


    def _add_constraints(self) -> None:
        self.solver.add_clause([self._var_row(self.start_row, 0)])
        self.solver.add_clause([self._var_column(self.start_column, 0)])

        self._add_turn_constraints()
        self._add_cell_constraints()
        self.add_knight_move_constraints()


    # At each turn, exactly one row and column is visited
    def _add_turn_constraints(self) -> None:
        for turn in range(self.cell_count):
            # At least one row is visited
            self.solver.add_clause([self._var_row(row, turn) for row in range(self.height)]) # (r0 ∨ r1 ∨ ...)

            # At least one column is visited
            self.solver.add_clause([self._var_column(column, turn) for column in range(self.width)]) # (c0 ∨ c1 ∨ ...)

            # At most one row is visited
            for row_1 in range(self.height - 1):
                for row_2 in range(row_1 + 1, self.height):
                    self.solver.add_clause([-self._var_row(row_1, turn), -self._var_row(row_2, turn)]) # (-r0 ∨ -r1) ∧ (-r0 ∨ -r2) ∧ (-r1 ∨ -r2) ∧ ...

            # At most one column is visited
            for column_1 in range(self.width - 1):
                for column_2 in range(column_1 + 1, self.width):
                    self.solver.add_clause([-self._var_column(column_1, turn), -self._var_column(column_2, turn)]) # (-c0 ∨ -c1) ∧ (-c0 ∨ -c2) ∧ (-c1 ∨ -c2) ∧ ...


    # Each cell is visited once
    def _add_cell_constraints(self) -> None:
        for row in range(self.height):
            for column in range(self.width):
                # Cell is visited at most once
                # p_i ≡ r_i ∧ c_i
                # (-p_0 ∨ -p_1) ∧ (-p_0 ∨ -p_2) ∧ (-p_1 ∨ -p_2) ∧ ...
                # (-r_0 ∨ -c_0 ∨ -r_1 ∨ -c_1) ∧ (-r_0 ∨ -c_0 ∨ -r_2 ∨ -c_2) ∧ (-r_1 ∨ -c_1 ∨ -r_2 ∨ -c_2) ∧ ...
                for turn_1 in range(self.cell_count - 1):
                    for turn_2 in range(turn_1 + 1, self.cell_count):
                        self.solver.add_clause([-self._var_row(row, turn_1), -self._var_column(column, turn_1), -self._var_row(row, turn_2), -self._var_column(column, turn_2)])


    # Valid knight moves
    def add_knight_move_constraints(self) -> None:
        # pi ≡ ri ∧ ci
        # p0 -> p1 ∨ p2 ∨ p3 ...
        # -p0 ∨ p1 ∨ p2 ∨ p3 ...
        # (-r0 ​∨ -c0)​ ∨ ((r1 ​∧ c1​) ∨ (r2 ​∧ c2​) ∨ (r3 ​∧ c3​) ∨ ...)
        # (-r0 ​∨ -c0) ∨ ((r1 ∨ r2 ∨ r3 ∨ ...) ​∧ (c1 ∨ r2 ∨ r3 ∨ ...) ​∧ ...)
        for turn in range(self.cell_count - 1):
            for row in range(self.height):
                for column in range(self.width):
                    pref_clause: list[int] = [-self._var_row(row, turn), -self._var_column(column, turn)]

                    next_possible_positions: list[list[int]] = []
                    for delta_row, delta_column in self.KNIGHT_MOVES:
                        next_row: int = row + delta_row
                        next_column: int = column + delta_column
                        if not (0 <= next_row < self.height and 0 <= next_column < self.width): continue
                        next_possible_positions.append([self._var_row(next_row, turn + 1), self._var_column(next_column, turn + 1)])

                    for clause in self._cartesian_product(next_possible_positions):
                        self.solver.add_clause(pref_clause + clause)


    def _var_row(self, row: int, turn: int) -> int:
        if not (0 <= row < self.height): raise ValueError("Row out of bounds.")
        return self._var_i(row, turn)


    def _var_column(self, column: int, turn: int) -> int:
        if not (0 <= column < self.width): raise ValueError("Column out of bounds.")
        return self._var_i(self.height + column, turn)


    def _var_i(self, index: int, turn: int) -> int:
        if not (0 <= index < self.size_sum and 0 <= turn < self.cell_count): raise ValueError("Index out of bounds.")
        return self.size_sum * turn + index + 1


    def _cartesian_product(self, lists: list[list]) -> list[list]:
        result: list[list] = [[]]
        for ls in lists:
            result = [combination + [i] for combination in result for i in ls]
        return result


def main() -> None:
    start_time = time.time()

    board = RowColumnSolution(6, 6, 0, 0)
    board.solver.solve()

    end_time = time.time()
    execution_time = end_time - start_time
    print(f"{execution_time:.4f}s")


if __name__ == '__main__': main()
