package logic

class Game(val rows: Int = 6, val cols: Int = 7, val winCount: Int = 4) {
    init {
        require(rows > 0) { "Rows must be > 0" }
        require(cols > 0) { "Cols must be > 0" }
        require(winCount > 0 && winCount <= maxOf(rows, cols)) { "winCount cannot be bigger than field" }
    }

    companion object {
        fun fromSave(save: GameSave): Game {
            val game = Game(save.rows, save.cols, save.winCount)
            game.restore(save)
            return game
        }
    }

    var state: GameState = GameState.Turn
        private set

    var player: Player = Player.Yellow
        private set

    private val board = Board(rows, cols)

    fun getCell(row: Int, col: Int): Cell = board.getCell(row, col)

    operator fun get(row: Int, col: Int): Cell = getCell(row, col)

    fun getBoard(): List<List<Cell>> = List(rows) { row -> List(cols) { col -> board[row, col] } }

    fun drop(col: Int): Int? {
        require(col in 0 until cols) { "Drop col out of range" }

        if (state != GameState.Turn) return null

        val row: Int = board.drop(col, player.cell) ?: return null

        if (board.getMaxLineLength(row, col) >= winCount) {
            state = GameState.Win
            return row
        }

        if (board.isFull()) {
            state = GameState.Draw
        }

        player = player.next

        return row
    }

    fun reset() {
        board.reset()
        state = GameState.Turn
        player = Player.Yellow
    }

    fun toSave() = GameSave(rows, cols, winCount, state, player, getBoard())

    private fun restore(save: GameSave) {
        state = save.state
        player = save.player
        board.load(save.board)
    }
}
