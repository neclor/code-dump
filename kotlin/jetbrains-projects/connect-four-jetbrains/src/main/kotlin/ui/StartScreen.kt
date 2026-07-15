package ui

import androidx.compose.runtime.*
import org.jetbrains.compose.web.attributes.InputType
import org.jetbrains.compose.web.css.*
import org.jetbrains.compose.web.dom.*
import ui.style.ControlsStyle
import ui.style.LayoutStyle

@Composable
fun StartScreen(onStart: (rows: Int, cols: Int, winCount: Int) -> Unit) {
    var rows by remember { mutableStateOf(6) }
    var cols by remember { mutableStateOf(7) }
    var winCount by remember { mutableStateOf(4) }

    val isValid = rows > 0 && cols > 0 && winCount > 0 && winCount <= maxOf(rows, cols)

    Div({ classes(LayoutStyle.screen) }) {
        Div({ classes(LayoutStyle.card) }) {
            H2({ classes(LayoutStyle.cardTitle) }) { Text("Connect Four") }
            Div({ classes(LayoutStyle.fields) }) {
                FieldRow("Rows", rows) { rows = it }
                FieldRow("Cols", cols) { cols = it }
                FieldRow("Win count", winCount) { winCount = it }
            }
            Button({
                classes(ControlsStyle.button)
                style { width(100.percent) }
                if (!isValid) attr("disabled", "")
                onClick { if (isValid) onStart(rows, cols, winCount) }
            }) { Text("Start") }
        }
    }
}

@Composable
private fun FieldRow(labelText: String, value: Int, onChange: (Int) -> Unit) {
    Div({ classes(LayoutStyle.row) }) {
        Label { Span({ classes(ControlsStyle.label) }) { Text(labelText) } }
        Input(InputType.Number) {
            classes(ControlsStyle.input)
            value(value)
            onInput { onChange(it.value?.toInt() ?: value) }
        }
    }
}