package ui.style

import org.jetbrains.compose.web.css.*

object LayoutStyle : StyleSheet() {

    init {
        "html, body" style {
            margin(0.px)
            padding(0.px)
        }
    }

    val screen by style {
        display(DisplayStyle.Flex)
        flexDirection(FlexDirection.Column)
        alignItems(AlignItems.Center)
        justifyContent(JustifyContent.Center)
        minHeight(100.vh)
        backgroundColor(Colors.background)
        color(Colors.textPrimary)
        property("font-family", "'Segoe UI', system-ui, sans-serif")
        property("box-sizing", "border-box")
        padding(2.em)
    }

    val gameScreen by style {
        display(DisplayStyle.Flex)
        flexDirection(FlexDirection.Column)
        alignItems(AlignItems.Center)
        property("height", "100svh")
        property("overflow", "hidden")
        backgroundColor(Colors.background)
        color(Colors.textPrimary)
        property("font-family", "'Segoe UI', system-ui, sans-serif")
        property("box-sizing", "border-box")
        padding(1.em)
        property("gap", "0.75em")
    }

    val boardWrapper by style {
        property("flex", "1")
        property("min-height", "0")
        width(100.percent)
        property("container-type", "size")
        display(DisplayStyle.Flex)
        alignItems(AlignItems.Center)
        justifyContent(JustifyContent.Center)
    }

    val card by style {
        backgroundColor(Colors.surface)
        borderRadius(16.px)
        padding(2.em)
        property("box-shadow", "0 8px 32px rgba(0,0,0,0.10)")
        display(DisplayStyle.Flex)
        flexDirection(FlexDirection.Column)
        alignItems(AlignItems.Center)
        property("gap", "1.5em")
        property("min-width", "250px")
        maxWidth(90.vw)
    }

    val cardTitle by style {
        fontSize(2.em)
        fontWeight("700")
        color(Colors.textPrimary)
        property("letter-spacing", "2px")
        property("text-transform", "uppercase")
        margin(0.px)
    }

    val fields by style {
        display(DisplayStyle.Flex)
        flexDirection(FlexDirection.Column)
        property("gap", "0.75em")
        width(100.percent)
    }

    val row by style {
        display(DisplayStyle.Flex)
        alignItems(AlignItems.Center)
        justifyContent(JustifyContent.SpaceBetween)
        property("gap", "1em")
        width(100.percent)
    }

    val actions by style {
        display(DisplayStyle.Flex)
        alignItems(AlignItems.Center)
        property("gap", "0.75em")
        flexWrap(FlexWrap.Wrap)
        justifyContent(JustifyContent.Center)
        property("flex-shrink", "0")
    }
}