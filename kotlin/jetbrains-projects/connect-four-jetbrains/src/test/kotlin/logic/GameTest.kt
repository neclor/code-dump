package logic

import kotlin.test.*

class GameTest {

    @Test
    fun initialState() {
        val game = Game()
        assertEquals(GameState.Turn, game.state)
        assertEquals(Player.Yellow, game.player)
    }

    @Test
    fun playerAlternates() {
        val game = Game()
        game.drop(0)
        assertEquals(Player.Red, game.player)
        game.drop(1)
        assertEquals(Player.Yellow, game.player)
    }

    @Test
    fun winsVertically() {
        val game = Game()
        game.drop(0); game.drop(1)
        game.drop(0); game.drop(1)
        game.drop(0); game.drop(1)
        game.drop(0)
        assertEquals(GameState.Win, game.state)
        assertEquals(Player.Yellow, game.player)
    }

    @Test
    fun winsHorizontally() {
        val game = Game()
        game.drop(0); game.drop(4)
        game.drop(1); game.drop(5)
        game.drop(2); game.drop(6)
        game.drop(3)
        assertEquals(GameState.Win, game.state)
        assertEquals(Player.Yellow, game.player)
    }

    @Test
    fun winsDiagonalDownRight() {
        val game = Game()
        game.drop(0); game.drop(1)
        game.drop(1); game.drop(2)
        game.drop(6); game.drop(2)
        game.drop(2); game.drop(3)
        game.drop(6); game.drop(3)
        game.drop(6); game.drop(3)
        game.drop(3)
        assertEquals(GameState.Win, game.state)
        assertEquals(Player.Yellow, game.player)
    }

    @Test
    fun winsDiagonalDownLeft() {
        val game = Game()
        game.drop(3); game.drop(2)
        game.drop(2); game.drop(1)
        game.drop(6); game.drop(1)
        game.drop(1); game.drop(0)
        game.drop(6); game.drop(0)
        game.drop(6); game.drop(0)
        game.drop(0)
        assertEquals(GameState.Win, game.state)
        assertEquals(Player.Yellow, game.player)
    }

    @Test
    fun draw() {
        val game = Game(rows = 1, cols = 3, winCount = 3)
        game.drop(0); game.drop(1);
        game.drop(2)
        assertEquals(GameState.Draw, game.state)
    }

    @Test
    fun cannotDropAfterGameEnds() {
        val game = makeYellowWin()
        assertNull(game.drop(0))
    }

    @Test
    fun cannotDropInFullColumn() {
        val game = Game(rows = 1, cols = 2, winCount = 2)
        game.drop(0)
        assertNull(game.drop(0))
    }

    @Test
    fun resetRestoresGame() {
        val game = makeYellowWin()
        game.reset()
        assertEquals(GameState.Turn, game.state)
        assertEquals(Player.Yellow, game.player)
        assertNotNull(game.drop(0))
    }

    @Test
    fun customWinCount() {
        val game = Game(winCount = 3)
        game.drop(0); game.drop(4)
        game.drop(1); game.drop(5)
        game.drop(2)
        assertEquals(GameState.Win, game.state)
    }

    private fun makeYellowWin(): Game {
        val game = Game()
        game.drop(0); game.drop(4)
        game.drop(1); game.drop(5)
        game.drop(2); game.drop(6)
        game.drop(3)
        return game
    }
}