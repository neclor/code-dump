using System.Diagnostics;
using System.Text.RegularExpressions;

namespace WowServer;

public static class Tmux {

	public static async Task<bool> ExecuteCommandAsync(string args, string directory = "/", bool silent = false) {
		using Process? process = Process.Start(new ProcessStartInfo {
			FileName = "tmux",
			Arguments = args,
			WorkingDirectory = directory,
			RedirectStandardOutput = silent,
			RedirectStandardError = silent,
			UseShellExecute = false,
			CreateNoWindow = true
		});
		if (process is null) return false;

		await process.WaitForExitAsync();
		return process.ExitCode == 0;
	}

	public static async Task<bool> ExecuteCommandInNewSession(string sessionName, string args, string directory = "/", bool silent = false) {
		_ = await ExecuteCommandAsync($"new-session -d -s {sessionName}", directory, silent);
		return await ExecuteCommandAsync($"send-keys -t {sessionName} \"{args}\" C-m", directory, silent);
	}

	public static async Task<bool> HasSessionAsync(string sessionName) => await ExecuteCommandAsync($"has-session -t {sessionName}", silent: true);

	public static async Task<bool> KillSessionAsync(string sessionName) => await ExecuteCommandAsync($"kill-session -t {sessionName}", silent: true);

	public static async Task<string> GetLastLinesAsync(string sessionName, int lineCount) {
		using Process? process = Process.Start(new ProcessStartInfo {
			FileName = "tmux",
			Arguments = $"capture-pane -pt {sessionName}",
			RedirectStandardOutput = true,
			UseShellExecute = false,
			CreateNoWindow = true
		});
		if (process is null) return "";

		string output = await process.StandardOutput.ReadToEndAsync();
		await process.WaitForExitAsync();

		IEnumerable<string> lastLines = output.Split('\n')
			.Where(line => !string.IsNullOrWhiteSpace(line))
			.TakeLast(lineCount);

		return string.Join(Environment.NewLine, lastLines);
	}
}
