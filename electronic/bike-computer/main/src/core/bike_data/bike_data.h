#ifndef BIKE_DATA_H
#define BIKE_DATA_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint64_t distance_mm;
	uint64_t time_ms;
	bool manual_paused;
	bool auto_paused;

	uint64_t total_distance_mm;

	uint8_t wheel_index;
} BikeData;

void bike_data_init(BikeData* self);

bool bike_data_set_wheel(BikeData* self, uint8_t wheel_index);

bool bike_data_is_paused(const BikeData* self);
void bike_data_add_revolutions(BikeData* self, uint32_t revolutions);
void bike_data_add_time(BikeData* self, uint64_t ms);

void bike_data_set_auto_paused(BikeData* self, bool paused);
void bike_data_toggle_pause(BikeData* self);
void bike_data_reset(BikeData* self);
void bike_data_reset_total(BikeData* self);

float bike_data_speed_m_s(const BikeData* self, uint64_t wheel_period_ms);
float bike_data_average_m_s(const BikeData* self);

#endif // BIKE_DATA_H
