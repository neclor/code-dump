package logic

import kotlinx.serialization.Serializable

@Serializable
data class GameSave(
    val rows: Int,
    val cols: Int,
    val winCount: Int,
    val state: GameState,
    val player: Player,
    val board: List<List<Cell>>
)
