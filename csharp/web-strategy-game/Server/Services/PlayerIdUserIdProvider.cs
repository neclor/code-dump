using Microsoft.AspNetCore.SignalR;

namespace Server.Services;

internal class PlayerIdUserIdProvider : IUserIdProvider {

	public string? GetUserId(HubConnectionContext connection) => connection.GetHttpContext()?.Request.Query["playerId"];
}
