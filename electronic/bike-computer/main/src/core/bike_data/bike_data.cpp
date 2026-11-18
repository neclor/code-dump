#include "bike_data.h"

#include <string.h>

#include "../wheels.h"

/* ======= Static ======= */

static uint16_t circumference_mm(const BikeData* self) { return WHEEL_SIZES[self->wheel_index].circumference_mm; }

/* ======= Public ======= */

void bike_data_init(BikeData* self) { memset(self, 0, sizeof(*self)); }

bool bike_data_set_wheel(BikeData* self, uint8_t wheel_index) {
	if (wheel_index >= WHEEL_SIZES_COUNT) return false;
	self->wheel_index = wheel_index;
	return true;
}

bool bike_data_is_paused(const BikeData* self) { return self->manual_paused || self->auto_paused; }

void bike_data_add_revolutions(BikeData* self, uint32_t revolutions) {
	const uint64_t mm = (uint64_t)revolutions * circumference_mm(self);
	self->total_distance_mm += mm;
	if (!self->manual_paused) self->distance_mm += mm;
}

void bike_data_add_time(BikeData* self, uint64_t ms) { if (!bike_data_is_paused(self)) self->time_ms += ms; }

void bike_data_set_auto_paused(BikeData* self, bool paused) { self->auto_paused = paused; }

void bike_data_toggle_pause(BikeData* self) { self->manual_paused = !self->manual_paused; }

void bike_data_reset(BikeData* self) {
	self->distance_mm = 0;
	self->time_ms = 0;
	self->manual_paused = false;
}

void bike_data_reset_total(BikeData* self) { self->total_distance_mm = 0; }

float bike_data_speed_m_s(const BikeData* self, uint64_t wheel_period_ms) {
	if (wheel_period_ms == 0) return 0.0f;
	return (float)circumference_mm(self) / wheel_period_ms;
}

float bike_data_average_m_s(const BikeData* self) {
	if (self->time_ms == 0) return 0.0f;
	return (float)self->distance_mm / self->time_ms;
}
