using System.Collections.Concurrent;
using Core;

namespace Server.Services;

internal class GameManager {

	public IReadOnlyDictionary<string, GameSession> Games => _games;
	public readonly ConcurrentDictionary<string, GameSession> _games = [];

	public (string GameId, Guid PlayerId) CreateGame(string userId) {
		GameSession gameSession = new();




		_games[gameSession.Id] = gameSession;
		return (gameSession.Id, );
	}
}
