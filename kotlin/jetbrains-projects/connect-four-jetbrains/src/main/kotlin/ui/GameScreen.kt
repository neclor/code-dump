package ui

import GameViewModel
import androidx.compose.runtime.*
import logic.Cell
import logic.GameState
import org.jetbrains.compose.web.dom.*
import ui.style.BoardStyle
import ui.style.ControlsStyle
import ui.style.LayoutStyle

@Composable
fun GameScreen(view: GameViewModel, onExit: () -> Unit) {
    Div({ classes(LayoutStyle.gameScreen) }) {
        StatusBar(view)
        Div({ classes(LayoutStyle.boardWrapper) }) {
            Board(view)
        }
        Div({ classes(LayoutStyle.actions) }) {
            Button({ classes(ControlsStyle.button); onClick { view.onReset() } }) { Text("Restart") }
            Button({ classes(ControlsStyle.button, ControlsStyle.buttonDanger); onClick { onExit() } }) { Text("Exit") }
        }
    }
}

@Composable
private fun StatusBar(view: GameViewModel) {
    val text = when (view.state) {
        GameState.Turn -> "Turn: ${view.player}"
        GameState.Win -> "Winner: ${view.player}!"
        GameState.Draw -> "Draw!"
    }
    Div({ classes(ControlsStyle.status) }) { Text(text) }
}

@Composable
private fun Board(view: GameViewModel) {
    Div({
        classes(BoardStyle.board)
        style {
            property("--cols", view.cols.toString())
            property("--rows", view.rows.toString())
        }
    }) {
        for (row in 0 until view.rows) {
            for (col in 0 until view.cols) {
                Div({
                    classes(BoardStyle.cell)
                    onClick { view.onDrop(col) }
                }) {
                    Piece(view.board[row][col], if (view.lastDrop == Pair(row, col)) row else null)
                }
            }
        }
    }
}

@Composable
private fun Piece(cell: Cell, dropRow: Int?) {
    val pieceClass = when (cell) {
        Cell.Empty -> BoardStyle.pieceEmpty
        Cell.Red -> BoardStyle.pieceRed
        Cell.Yellow -> BoardStyle.pieceYellow
    }
    Div({
        classes(BoardStyle.piece, pieceClass)
        if (dropRow != null) {
            classes(BoardStyle.pieceDrop)
            style { property("--drop-row", dropRow.toString()) }
        }
    })
}