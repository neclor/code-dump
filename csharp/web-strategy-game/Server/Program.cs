using Microsoft.AspNetCore.SignalR;
using Server.Hubs;
using Server.Services;

namespace Server;

internal static class Program {

	private static void Main(string[] args) {
		WebApplicationBuilder builder = WebApplication.CreateBuilder(args);
		builder.Services.AddRazorComponents()
			.AddInteractiveWebAssemblyComponents();
		builder.Services.AddSignalR();

		builder.Services.AddSingleton<IUserIdProvider, PlayerIdUserIdProvider>();
		builder.Services.AddSingleton<GameManager>();

		WebApplication app = builder.Build();
		app.UseDefaultFiles();
		app.UseStaticFiles();
		app.MapStaticAssets();
		app.UseAntiforgery();

		app.MapHub<ServerHub>("/serverhub");

		app.MapFallbackToFile("index.html");

		app.Run();
	}
}
