#include <nvs.h>

#include "storage.h"

#define STORAGE_NAME "bike"

static nvs_handle_t handle;

void storage_init(void) { nvs_open(STORAGE_NAME, NVS_READWRITE, &handle); }

bool storage_read(const char* key, void* data, size_t size) {
	size_t stored = 0;
	if (nvs_get_blob(handle, key, NULL, &stored) != ESP_OK || stored != size) return false;
	return nvs_get_blob(handle, key, data, &stored) == ESP_OK;
}

void storage_write(const char* key, const void* data, size_t size) {
	nvs_set_blob(handle, key, data, size);
	nvs_commit(handle);
}
