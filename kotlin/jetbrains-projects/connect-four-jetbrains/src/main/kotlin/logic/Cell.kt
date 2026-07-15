package logic

import kotlinx.serialization.Serializable

@Serializable
enum class Cell {
    Empty,
    Red,
    Yellow
}