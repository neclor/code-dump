using System.Text.Json;
using Microsoft.AspNetCore.Mvc;

namespace NetrisServer.Controllers;

[ApiController]
[Route("[controller]")]

#pragma warning disable CA1515
public class LeaderboardController : ControllerBase {
#pragma warning restore CA1515

	private const string FilePath = "leaderboard.csv";
	private const int MaxEntries = 200;

	internal record ScoreEntry(string Name, int Score);

	[HttpPost]
	public IActionResult Post([FromBody] JsonElement json) {
		string name = json.GetProperty("name").GetString() ?? "";
		int score = json.GetProperty("score").GetInt32();

		if (string.IsNullOrWhiteSpace(name) || score <= 0) return BadRequest();

		List<ScoreEntry> scores = [];
		if (System.IO.File.Exists(FilePath)) {
			scores = [.. System.IO.File.ReadAllLines(FilePath)
				.Select(l => {
					string[] parts = l.Split(',');
					return new ScoreEntry(parts[1], int.Parse(parts[0], null));
				})];
		}

		scores.Add(new ScoreEntry(name, score));

		List<ScoreEntry> topScores = [.. scores
			.GroupBy(s => s.Name)
			.Select(g => g.OrderByDescending(x => x.Score).First())
			.OrderByDescending(x => x.Score)
			.Take(MaxEntries)];

		System.IO.File.WriteAllLines(FilePath, topScores.Select(s => $"{s.Score},{s.Name}"));
		return Ok();
	}

	[HttpGet]
	public IActionResult Get() {
		if (!System.IO.File.Exists(FilePath))
			return Ok(Array.Empty<object>());

		List<ScoreEntry> scores = [.. System.IO.File.ReadAllLines(FilePath)
			.Select(l => {
				string[] parts = l.Split(',');
				return new ScoreEntry(parts[1], int.Parse(parts[0], null));
			})];

		return Ok(scores);
	}
}
