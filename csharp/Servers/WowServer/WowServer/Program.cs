
namespace WowServer;

public class Program {

	public static void Main(string[] args) {
		WebApplicationBuilder builder = WebApplication.CreateBuilder(args);

		_ = builder.Configuration.AddJsonFile("wow_server_manager_config.json", false, true);
		ServerConfig serverConfig = builder.Configuration.Get<ServerConfig>() ?? new ServerConfig();

		_ = builder.Services
			.AddSingleton(serverConfig)
			.AddSingleton<WowServerManager>()
			.AddHostedService<WowServerService>()
			.AddControllers();

		WebApplication app = builder.Build();
		_ = app.MapControllers();

		app.Run();
	}
}
