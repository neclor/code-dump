using Core.World;

namespace Core;

public class Game {

	private Lobby Lobby { get; } = new();

	public WorldManager WorldManager { get; } = new();

	public bool TryStartGame() {
		if (Lobby.Players.Count < 2) return false;

		foreach (Player player in Lobby.Players) {
			if (!player.IsReady) return false;
		}

		WorldManager.InitializeWorld(Lobby.Players);
		return true;
	}
}
