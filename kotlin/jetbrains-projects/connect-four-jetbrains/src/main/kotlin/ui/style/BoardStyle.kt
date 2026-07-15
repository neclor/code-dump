package ui.style

import org.jetbrains.compose.web.css.*

object BoardStyle : StyleSheet() {

    val board by style {
        property("width", "min(85cqw, calc(85cqh * var(--cols) / var(--rows)))")
        property("aspect-ratio", "var(--cols) / var(--rows)")
        property("box-sizing", "border-box")
        display(DisplayStyle.Grid)
        gridTemplateColumns("repeat(var(--cols), 1fr)")
        gridTemplateRows("repeat(var(--rows), 1fr)")
        gap(4.px)
        backgroundColor(Colors.board)
        borderRadius(12.px)
        padding(8.px)
        property("box-shadow", "0 8px 24px rgba(37,99,235,0.3)")
    }

    val cell by style {
        property("aspect-ratio", "1")
        cursor("pointer")
        backgroundColor(Colors.board)
        display(DisplayStyle.Flex)
        alignItems(AlignItems.Center)
        justifyContent(JustifyContent.Center)
        borderRadius(4.px)
    }

    val piece by style {
        width(70.percent)
        height(70.percent)
        borderRadius(50.percent)
        property("box-shadow", "0 2px 8px rgba(0,0,0,0.2)")
    }

    val pieceEmpty by style {
        backgroundColor(rgba(255, 255, 255, 0.9))
    }
    val pieceRed by style {
        backgroundColor(Colors.pieceRed)
    }
    val pieceYellow by style {
        backgroundColor(Colors.pieceYellow)
    }

    val dropAnim by keyframes {
        from {
            property("transform", "translateY(calc((var(--drop-row) + 1) * -143%))")
        }
        to {
            property("transform", "translateY(0)")
        }
    }

    val pieceDrop by style {
        animation(dropAnim) {
            duration(0.5.s)
            timingFunction(AnimationTimingFunction.EaseOut)
        }
    }
}