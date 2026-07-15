package logic

class Board(val rows: Int = 6, val cols: Int = 7) {
    init {
        require(rows > 0 && cols > 0) { "Board must exist" }
    }

    private val grid: Array<Array<Cell>> = Array(rows) { Array(cols) { Cell.Empty } }

    fun getCell(row: Int, col: Int): Cell {
        require(col in 0 until cols) { "Col out of range" }
        require(row in 0 until rows) { "Row out of range" }
        return grid[row][col]
    }

    operator fun get(row: Int, col: Int): Cell = getCell(row, col)

    fun drop(col: Int, cell: Cell): Int? {
        require(col in 0 until cols) { "Col out of range" }

        for (row in rows - 1 downTo 0) {
            if (grid[row][col] != Cell.Empty) continue
            grid[row][col] = cell
            return row
        }
        return null
    }

    operator fun set(col: Int, cell: Cell) = drop(col, cell)

    fun load(board: List<List<Cell>>) {
        for (row in 0 until rows)
            for (col in 0 until cols)
                grid[row][col] = board[row][col]
    }

    fun isColumnFull(col: Int): Boolean = grid[0][col] != Cell.Empty

    fun isFull(): Boolean = (0 until cols).all { isColumnFull(it) }

    fun getMaxLineLength(row: Int, col: Int): Int {
        require(row in 0 until rows) { "Row out of range" }
        require(col in 0 until cols) { "Col out of range" }

        val cell: Cell = grid[row][col]
        return maxOf(
            getLineLength(row, col, cell, 1, 0),
            getLineLength(row, col, cell, 0, 1),
            getLineLength(row, col, cell, 1, 1),
            getLineLength(row, col, cell, 1, -1)
        )
    }

    fun reset() {
        for (row in grid) row.fill(Cell.Empty)
    }

    private fun getLineLength(row: Int, col: Int, cell: Cell, y: Int, x: Int): Int {
        if (grid[row][col] == Cell.Empty) return 0
        var count = 1
        count += countInDirection(row, col, cell, y, x)
        count += countInDirection(row, col, cell, -y, -x)
        return count
    }

    private fun countInDirection(row: Int, col: Int, cell: Cell, y: Int, x: Int): Int {
        var r: Int = row + y
        var c: Int = col + x
        var count = 0
        while (r in 0 until rows && c in 0 until cols && grid[r][c] == cell) {
            count++
            r += y
            c += x
        }
        return count
    }
}
