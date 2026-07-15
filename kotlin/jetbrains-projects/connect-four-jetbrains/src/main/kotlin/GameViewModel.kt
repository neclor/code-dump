import androidx.compose.runtime.*
import logic.Game

class GameViewModel(private val game: Game) {
    val rows
        get() = game.rows
    val cols
        get() = game.cols

    var state by mutableStateOf(game.state)
        private set

    var player by mutableStateOf(game.player)
        private set

    var board by mutableStateOf(game.getBoard())
        private set

    var lastDrop by mutableStateOf<Pair<Int, Int>?>(null)
        private set

    fun onDrop(col: Int) {
        val row: Int? = game.drop(col)
        lastDrop = if (row != null) Pair(row, col) else null
        sync()
        GameStorage.save(game)
    }

    fun onReset() {
        game.reset()
        lastDrop = null
        sync()
        GameStorage.clear()
    }

    private fun sync() {
        state = game.state
        player = game.player
        board = game.getBoard()
    }
}