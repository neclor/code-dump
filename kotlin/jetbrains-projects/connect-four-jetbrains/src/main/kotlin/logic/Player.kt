package logic

import kotlinx.serialization.Serializable

@Serializable
sealed class Player(val cell: Cell) {
    @Serializable
    object Red : Player(Cell.Red)
    @Serializable
    object Yellow : Player(Cell.Yellow)

    val next: Player
        get() = when (this) {
            is Red -> Yellow
            is Yellow -> Red
        }

    override fun toString(): String = this::class.simpleName ?: "Player"
}