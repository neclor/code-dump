#include "bike_storage.h"

#include "../../modules/storage/storage.h"

#define KEY_WHEEL "wheel"
#define KEY_ODOMETER "odometer"

#define DEFAULT_WHEEL_INDEX 13 // 700x45C
#define ODOMETER_SAVE_STEP_MM 1000000 // 1 km

static uint64_t saved_total_distance_mm;

/* ======= Public ======= */

void bike_storage_init(void) { storage_init(); }

void bike_storage_load(BikeData* data) {
	uint8_t wheel_index = 0;
	if (!storage_read(KEY_WHEEL, &wheel_index, sizeof(wheel_index)) || !bike_data_set_wheel(data, wheel_index)) {
		bike_data_set_wheel(data, DEFAULT_WHEEL_INDEX);
		bike_storage_save_wheel(data);
	}

	storage_read(KEY_ODOMETER, &data->total_distance_mm, sizeof(data->total_distance_mm));
	saved_total_distance_mm = data->total_distance_mm;
}

void bike_storage_save_wheel(const BikeData* data) { storage_write(KEY_WHEEL, &data->wheel_index, sizeof(data->wheel_index)); }

void bike_storage_save_total_distance(const BikeData* data) {
	storage_write(KEY_ODOMETER, &data->total_distance_mm, sizeof(data->total_distance_mm));
	saved_total_distance_mm = data->total_distance_mm;
}

void bike_storage_save_total_distance_if_needed(const BikeData* data) {
	if (data->total_distance_mm - saved_total_distance_mm >= ODOMETER_SAVE_STEP_MM) bike_storage_save_total_distance(data);
}
