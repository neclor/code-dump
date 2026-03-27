namespace WowServer;

public class WowServerService(WowServerManager wowServerManager) : BackgroundService {

	private readonly WowServerManager _wowServerManager = wowServerManager;

	private int _idleMinutes = 0;

	protected override async Task ExecuteAsync(CancellationToken stoppingToken) {
		while (!stoppingToken.IsCancellationRequested) {

			if (await _wowServerManager.IsWorldRunningAsync()) {

				if (await _wowServerManager.GetOnlineCountAsync() == 0) {

					if (_idleMinutes >= _wowServerManager.Config.WorldIdleShutdownTimeout) {
						_idleMinutes = 0;
						await _wowServerManager.StopAllAsync();
						continue;
					}
					_idleMinutes++;

					await Task.Delay(TimeSpan.FromMinutes(1), stoppingToken);
					continue;

				} else {
					_idleMinutes = 0;
				}
				await Task.Delay(TimeSpan.FromMinutes(10), stoppingToken);

			} else {
				await Task.Delay(TimeSpan.FromMinutes(30), stoppingToken);
			}
		}
	}
}
