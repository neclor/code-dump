namespace Core.Players;

public class Lobby {

	public const int MaxPlayers = 10;

	public Player? Host {
		get;
		set {
			ArgumentNullException.ThrowIfNull(value);
			if (!_players.Contains(value)) {
				if (_players.Count >= MaxPlayers) return;
				_players.Add(value);
			}
			field = value;
		}
	}

	public IReadOnlyList<Player> Players => _players;
	private readonly List<Player> _players = [];

	public bool AddPlayer(Player player) {
		if (_players.Count >= MaxPlayers) return false;
		_players.Add(player);
		return true;
	}

	public bool RemovePlayer(Player player) {
		if (Host == player) Host = null;
		return _players.Remove(player);
	}

	public Player? GetPlayerById(Guid id) => _players.FirstOrDefault(p => p.Id == id);
}
