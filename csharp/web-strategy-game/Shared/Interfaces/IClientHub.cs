namespace Shared.Interfaces;

public interface IClientHub {

	Task ReciveMessageAsync(string message);
}
