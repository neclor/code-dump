using System.Diagnostics;
using System.Text.Json;
using System.Text.RegularExpressions;

namespace WowServer;

public class WowServerManager(ServerConfig config) {

	public ServerConfig Config { get; } = config;

	private readonly SemaphoreSlim _lock = new(1, 1);

	public async Task StartAllAsync() {
		await _lock.WaitAsync();

		List<Task> tasks = [];
		if (!await IsAuthRunningAsync()) {
			tasks.Add(StartServiceAsync(Config.TmuxAuthSessionName, Config.AuthExe));
		}
		if (!await IsWorldRunningAsync()) {
			tasks.Add(StartServiceAsync(Config.TmuxWorldSessionName, Config.WorldExe));
		}
		await Task.WhenAll(tasks);

		_ = _lock.Release();
	}

	public async Task StopAllAsync() {
		await _lock.WaitAsync();

		List<Task> tasks = [];

		if (await IsAuthRunningAsync()) {
			tasks.Add(Tmux.ExecuteCommandAsync($"send-keys -t {Config.TmuxAuthSessionName} C-c"));
		}
		if (await IsWorldRunningAsync()) {
			tasks.Add(Tmux.ExecuteCommandAsync($"send-keys -t {Config.TmuxWorldSessionName} \"server shutdown {Config.WorldShutdownDelay}\" C-m"));
		}

		await Task.WhenAll(tasks);

		for (int i = 0; i < 600; i++) {
			if (!await IsAuthRunningAsync() && !await IsWorldRunningAsync()) break;
			await Task.Delay(TimeSpan.FromSeconds(1));
		}

		List<Task> killTasks = [
			Tmux.KillSessionAsync(Config.TmuxAuthSessionName),
			Tmux.KillSessionAsync(Config.TmuxWorldSessionName)
		];
		await Task.WhenAll(killTasks);

		_ = _lock.Release();
	}

	public async Task<bool> CreateAccount(string name, string password) {
		if (!await IsWorldRunningAsync() || !await IsAuthRunningAsync()) return false;

		await _lock.WaitAsync();

		_ = await Tmux.ExecuteCommandAsync($"send-keys -t {Config.TmuxWorldSessionName} \"account create {name} {password}\" C-m");
		await Task.Delay(500);
		string text = await Tmux.GetLastLinesAsync(Config.TmuxWorldSessionName, 2);

		_ = _lock.Release();

		Match match = Regex.Match(text, @"Account(?:\s+.*?)?\s+created");

		return match.Success;
	}

	public async Task<bool> IsAuthRunningAsync() {
		if (!await Tmux.HasSessionAsync(Config.TmuxAuthSessionName)) return false;

		using Process? process = Process.Start(new ProcessStartInfo {
			FileName = "pgrep",
			Arguments = $"-f {Config.AuthExe}",
			RedirectStandardOutput = true,
			UseShellExecute = false,
			CreateNoWindow = true
		});

		process?.WaitForExit();
		return process?.ExitCode == 0;
	}

	public async Task<bool> IsWorldRunningAsync() {
		if (!await Tmux.HasSessionAsync(Config.TmuxWorldSessionName)) return false;

		using Process? process = Process.Start(new ProcessStartInfo {
			FileName = "pgrep",
			Arguments = $"-f {Config.WorldExe}",
			RedirectStandardOutput = true,
			UseShellExecute = false,
			CreateNoWindow = true
		});

		process?.WaitForExit();
		return process?.ExitCode == 0;
	}

	public async Task<int> GetOnlineCountAsync() {
		if (!await IsWorldRunningAsync()) return 0;

		await _lock.WaitAsync();

		_ = await Tmux.ExecuteCommandAsync($"send-keys -t {Config.TmuxWorldSessionName} \"server info\" C-m");
		await Task.Delay(500);

		string text = await Tmux.GetLastLinesAsync(Config.TmuxWorldSessionName, 10);
		Match match = Regex.Match(text, @"Connected players:\s+(\d+)");

		_ = _lock.Release();

		if (match.Success && int.TryParse(match.Groups[1].Value, out int count)) {
			return count;
		}

		return -1;
	}

	private async Task StartServiceAsync(string sessionName, string exePath) {
		_ = await Tmux.KillSessionAsync(sessionName);

		string cmd = $"WINEPREFIX='{Config.WinePrefix}' wine '{exePath}'";
		_ = await Tmux.ExecuteCommandInNewSession(sessionName, cmd, Config.WorkingDirectory);
	}
}
