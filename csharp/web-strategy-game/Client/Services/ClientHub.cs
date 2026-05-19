using Shared.Interfaces;

namespace Client.Services;

internal class ClientHub : IClientHub {

	public async Task ReciveMessageAsync(string message) => Console.WriteLine("Message: " + message);
}
