package ui.style

import androidx.compose.runtime.Composable
import org.jetbrains.compose.web.css.Style

@Composable
fun AppStyle() {
    Style(LayoutStyle)
    Style(BoardStyle)
    Style(ControlsStyle)
}