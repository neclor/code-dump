package ui

import GameViewModel
import androidx.compose.runtime.Composable

@Composable
fun Body(view: GameViewModel?, onStart: (rows: Int, cols: Int, winCount: Int) -> Unit, onExit: () -> Unit) {
    if (view == null) {
        StartScreen(onStart)
    } else {
        GameScreen(view, onExit)
    }
}