package ui.style

import org.jetbrains.compose.web.css.*

object ControlsStyle : StyleSheet() {

    val button by style {
        padding(1.em, 2.4.em)
        backgroundColor(Colors.accent)
        color(Colors.textInverse)
        property("border", "none")
        borderRadius(10.px)
        cursor("pointer")
        fontSize(1.5.em)
        fontWeight("600")
        property("letter-spacing", "0.3px")
        property("transition", "opacity 0.15s ease")
    }

    val buttonDanger by style {
        backgroundColor(Colors.danger)
    }

    val label by style {
        fontSize(15.px)
        color(Colors.textPrimary)
        property("min-width", "85px")
        fontWeight("500")
    }

    val input by style {
        width(70.px)
        padding(8.px, 10.px)
        borderRadius(8.px)
        property("border", "1px solid #e2e8f0")
        backgroundColor(Colors.surface)
        color(Colors.textPrimary)
        fontSize(15.px)
        property("text-align", "center")
        property("outline", "none")
    }

    val status by style {
        fontSize(2.2.em)
        fontWeight("700")
        color(Colors.textPrimary)
        property("letter-spacing", "0.5px")
        property("flex-shrink", "0")
        property("text-align", "center")
    }
}