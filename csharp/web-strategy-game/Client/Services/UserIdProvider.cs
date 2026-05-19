using System;
using System.Threading.Tasks;
using Microsoft.JSInterop;

namespace Client.Services;

internal class UserIdProvider(IJSRuntime js) {
	private const string StorageKey = "user_id";

	private readonly IJSRuntime _js = js ?? throw new ArgumentNullException(nameof(js));
	private Guid _userId;

	public async Task<Guid> GetuserIdAsync() {
		if (_userId != default) return _userId;

		string stringId = await _js.InvokeAsync<string>("localStorage.getItem", StorageKey).ConfigureAwait(true);
		if (Guid.TryParse(stringId, out _userId)) return _userId;

		_userId = Guid.NewGuid();
		await _js.InvokeVoidAsync("localStorage.setItem", StorageKey, _userId.ToString()).ConfigureAwait(true);

		return _userId;
	}
}
