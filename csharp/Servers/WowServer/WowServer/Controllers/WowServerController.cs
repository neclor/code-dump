using Microsoft.AspNetCore.Mvc;

namespace WowServer.Controllers;

[ApiController]
[Route("[controller]")]
public class WowServerController(WowServerManager wowServerManager) : ControllerBase {

	private readonly WowServerManager _wowServerManager = wowServerManager;

	[HttpPost]
	public async Task<IActionResult> Start() {
		await _wowServerManager.StartAllAsync();
		return Ok();
	}

	[HttpGet]
	public async Task<IActionResult> Get() {
		return Ok(new {
			auth = await _wowServerManager.IsAuthRunningAsync(),
			world = await _wowServerManager.IsWorldRunningAsync(),
			online = await _wowServerManager.GetOnlineCountAsync()
		});
	}
}
