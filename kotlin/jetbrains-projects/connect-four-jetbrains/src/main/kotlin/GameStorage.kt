import kotlinx.browser.localStorage
import kotlinx.serialization.json.Json
import kotlinx.serialization.encodeToString
import logic.Game
import logic.GameSave

object GameStorage {
    private const val KEY = "connect_four"

    fun save(game: Game) {
        localStorage.setItem(KEY, Json.encodeToString(game.toSave()))
    }

    fun load(): Game? = try {
        localStorage.getItem(KEY)
            ?.let { Json.decodeFromString<GameSave>(it) }
            ?.let { Game.fromSave(it) }
    } catch (_: Exception) { null }

    fun clear() = localStorage.removeItem(KEY)
}
