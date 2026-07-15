package logic

import kotlinx.serialization.Serializable

@Serializable
enum class GameState {
    Turn,
    Win,
    Draw
}

