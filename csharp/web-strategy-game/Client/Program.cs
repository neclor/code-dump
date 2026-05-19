using Client.Services;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;

namespace Client;

internal static class Program {

	private static async Task Main(string[] args) {
		WebAssemblyHostBuilder builder = WebAssemblyHostBuilder.CreateDefault(args);
		builder.RootComponents.Add<App>("#app");
		builder.RootComponents.Add<HeadOutlet>("head::after");

		builder.Services.AddScoped(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) });

		builder.Services.AddSingleton<UserIdProvider>();
		builder.Services.AddSingleton<ClientHub>();
		builder.Services.AddSingleton<HubConnectionProvider>();

		WebAssemblyHost app = builder.Build();

		HubConnectionProvider hub = app.Services.GetRequiredService<HubConnectionProvider>();
		await hub.InitializeAsync().ConfigureAwait(true);
		_ = hub.HubConnection?.StartAsync();

		await app.RunAsync().ConfigureAwait(true);
	}
}
