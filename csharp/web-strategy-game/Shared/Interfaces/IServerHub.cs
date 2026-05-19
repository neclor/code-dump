using System.Drawing;
using Shared.Dto;

namespace Shared.Interfaces;

public interface IServerHub {

	Task<string> CreateGameAsync();









	//Task<Guid?> JoinGameAsync(string gameCode);
	//Task<bool> SendTurnAsync();

	//Task UpdatePlayer(PlayerDto data);
}
