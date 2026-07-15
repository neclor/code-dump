import androidx.compose.runtime.*
import logic.Game
import org.jetbrains.compose.web.renderComposable
import ui.Body
import ui.style.AppStyle


fun main() {
    renderComposable(rootElementId = "root") {
        var gameViewModel by remember { mutableStateOf<GameViewModel?>(GameStorage.load()?.let { GameViewModel(it) }) }

        AppStyle()
        Body(
            gameViewModel,
            { rows, cols, winCount ->
                gameViewModel = GameViewModel(Game(rows, cols, winCount))
            },
            {
                gameViewModel = null
                GameStorage.clear()
            }
        )
    }
}

