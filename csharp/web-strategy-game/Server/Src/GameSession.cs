using System.Collections.Concurrent;
using Core;

namespace Server;

internal class GameSession {

	public string Id { get; } = Guid.NewGuid().ToString()[..8];
	public ConcurrentDictionary<string, Guid> PlayerIdMap { get; } = [];
	public Game Game { get; } = new();

	
	void bool TryJoin() {
		// Implement join logic if needed
		return true;
	}

}
