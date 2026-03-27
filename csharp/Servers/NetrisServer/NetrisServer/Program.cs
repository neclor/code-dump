namespace NetrisServer;

internal class Program {

	public static void Main(string[] args) {
		WebApplicationBuilder builder = WebApplication.CreateBuilder(args);

		_ = builder.Services.AddControllers();

		WebApplication app = builder.Build();

		_ = app.MapControllers();

		app.Run();
	}
}
