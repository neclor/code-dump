namespace WowServer;

public class ServerConfig {

	public string TmuxAuthSessionName { get; set; } = "wow-auth";
	public string TmuxWorldSessionName { get; set; } = "wow-world";

	public string WinePrefix { get; set; } = "/home/neclor/servers/wow/wine";

	public string AuthExe { get; set; } = "authserver.exe";
	public string WorldExe { get; set; } = "worldserver.exe";
	public string WorkingDirectory { get; set; } = "/home/neclor/servers/wow/azc-wotlk";

	public int WorldShutdownDelay { get; set; } = 10;
	public int WorldIdleShutdownTimeout { get; set; } = 30;
}
