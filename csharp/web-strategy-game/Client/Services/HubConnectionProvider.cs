using Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.SignalR.Client;
using Shared.Interfaces;
using Client.SignalRClientSourceGenerator;

namespace Client.Services;

internal class HubConnectionProvider(NavigationManager navigationManager, UserIdProvider userIdProvider, ClientHub clientHub) {

	private readonly NavigationManager _naviagionManager = navigationManager;
	private readonly UserIdProvider _userIdProvider = userIdProvider;
	private readonly ClientHub _clientHub = clientHub;

#pragma warning disable CS8618
	public HubConnection HubConnection { get; private set; }
	public IServerHub ServerConnection { get; private set; }
#pragma warning restore CS8618

	public async Task InitializeAsync() {
		if (HubConnection is not null) return;

		Guid userId = await _userIdProvider.GetuserIdAsync().ConfigureAwait(true);

		HubConnection = new HubConnectionBuilder()
			.WithUrl(_naviagionManager.ToAbsoluteUri($"/serverhub?userId={userId}"))
			.WithAutomaticReconnect()
			.Build();

		ServerConnection = HubConnection.ServerProxy<IServerHub>();
		_ = HubConnection.ClientRegistration<IClientHub>(_clientHub);
	}
}
