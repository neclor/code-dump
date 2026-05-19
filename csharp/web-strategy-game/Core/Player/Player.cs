using System.Drawing;
using Core.Resources;

namespace Core.Players;

public class Player() {

	public Guid Id { get; } = Guid.NewGuid();

	public string Name { get; set; } = "Player";

#pragma warning disable CA5394
	public Color Color { get; set; } = Color.FromKnownColor(Enum.GetValues<KnownColor>()[new Random().Next(1, Enum.GetValues<KnownColor>().Length)]);
#pragma warning restore CA5394

	public bool IsReady { get; set; }

	public PlayerResources Resources { get; } = new();

	public IList<IPlayerObject> PlayerObjects { get; } = [];
}
