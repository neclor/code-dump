using Microsoft.AspNetCore.Mvc;

namespace WowServer.Controllers;

[ApiController]
[Route("[controller]")]
public class WowServerController(WowServerManager wowServerManager) : ControllerBase {

	private readonly WowServerManager _wowServerManager = wowServerManager;

	[HttpPost("start")]
	public async Task<IActionResult> Start() {
		await _wowServerManager.StartAllAsync();
		return Ok();
	}

	[HttpPost("account/create")]
	public async Task<IActionResult> CreateAccount(string name, string password) {
		if (string.IsNullOrWhiteSpace(name) || string.IsNullOrWhiteSpace(password)) return BadRequest("Name and password required");

		return Ok(new {
			created = await _wowServerManager.CreateAccount(name, password)
		});
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
