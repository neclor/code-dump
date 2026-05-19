using Microsoft.AspNetCore.SignalR;
using Server.Services;
using Shared.Interfaces;
using Core;

namespace Server.Hubs;

internal class ServerHub(GameManager gameManager) : Hub<IClientHub>, IServerHub {

	private readonly GameManager _gameManager = gameManager;

	public async Task<(string GameId, Guid PlayerId)> CreateGameAsync() {
		if (Context.UserIdentifier == null) throw new InvalidOperationException("Invalid UserId");
		return _gameManager.CreateGame(Context.UserIdentifier);
	}

	public async Task<(bool Joined, Guid PlayerId)> JoinGameAsync(string gameId) {
		if (Context.UserIdentifier == null) throw new InvalidOperationException("Invalid UserId");

		if (!_gameManager.Games.TryGetValue(gameId, out GameSession? gameSession)) throw new InvalidOperationException("Invalid GameId");



		return gameSession.TryJoin()

		gameSession.PlayerIdMap.AddOrUpdate(Context.UserIdentifier, )


		Guid playerId = gameSession.AddPlayer(Context.UserIdentifier);
		await Groups.AddToGroupAsync(Context.ConnectionId, gameId).ConfigureAwait(true);
		await Clients.Group(gameId).SendAsync("PlayerJoined", playerId).ConfigureAwait(true);
		return true;
	}




	public async Task<Guid> GetPlayerId(string gameId) {
		if (Context.UserIdentifier == null) throw new InvalidOperationException("Invalid UserId");
		if (!_gameManager.Games.TryGetValue(gameId, out GameSession? gameSession)) throw new InvalidOperationException("Invalid GameId");
		if (!gameSession.PlayerIdMap.TryGetValue(Context.UserIdentifier, out Guid playerId)) throw new InvalidOperationException("Player not found in game");
		return playerId;
	}


		/*
		public async Task<string?> JoinGameAsync(string gameCode) {
			if (!_gameManager.Games.TryGetValue(gameCode, out Game? game)) return null;


			Player





			await Groups.AddToGroupAsync(Context.ConnectionId, gameId).ConfigureAwait(true);

			// await Clients.Group(code).SendAsync("PlayerJoined", Context.ConnectionId);

			await Clients.Group(gameId).SendAsync("JoinedAsync", Context.ConnectionId).ConfigureAwait(true);
			await Clients.Group(gameId).SendAsync("JoinedAsyncTest", Context.ConnectionId).ConfigureAwait(true);

			Console.WriteLine("Game joined: " + gameId + " user: " + Context.ConnectionId);

			return true;
		}
		*/


		/*
		public async Task Send(string message) => await Clients.All.SendAsync("Receive", message).ConfigureAwait(true);


		private string GenerateLobbyCode() {
			string lobbyCode;
			do {
				lobbyCode = Guid.NewGuid().ToString();
			} while (_lobbies.ContainsKey(lobbyCode));
			return lobbyCode;
		}
		*/
	}
