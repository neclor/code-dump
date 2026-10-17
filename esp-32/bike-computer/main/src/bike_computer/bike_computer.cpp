#include "bike_computer.h"

#include <string.h>

#include "bike_storage/bike_storage.h"
#include "../modules/display/display.h"
#include "../modules/wheel_sensor/wheel_sensor.h"

#define AUTO_PAUSE true
#define DISPLAY_REFRESH_MS 250

#define PIN_BUTTON 33
#define LONG_PRESS_MS 1000

/* ======= Static ======= */

static void update_data(BikeComputer* self, uint64_t now_ms) {
	const WheelSensorData sensor = wheel_sensor_read();

	bike_data_set_auto_paused(&self->data, AUTO_PAUSE && wheel_sensor_is_stopped(now_ms));
	bike_data_add_revolutions(&self->data, sensor.revolution_count - self->last_revolution_count);
	self->last_revolution_count = sensor.revolution_count;

	bike_data_add_time(&self->data, now_ms - self->last_update_ms);
	self->last_update_ms = now_ms;
}

static void handle_input(BikeComputer* self, uint64_t now_ms) {
	Button* button = &self->button;
	const uint64_t held_ms = button_held_ms(button, now_ms);

	button_update(button, now_ms);
	if (!button_released(button)) return;

	if (held_ms >= LONG_PRESS_MS) bike_data_reset(&self->data);
	else bike_data_toggle_pause(&self->data);
}

static void render(BikeComputer* self, uint64_t now_ms) {
	if (now_ms - self->last_render_ms < DISPLAY_REFRESH_MS) return;
	self->last_render_ms = now_ms;

	const BikeData* data = &self->data;
	const WheelSensorData sensor = wheel_sensor_read();

	ViewModel view;
	view.speed_m_s = wheel_sensor_is_stopped(now_ms) ? 0.0f : bike_data_speed_m_s(data, sensor.last_period_ms);
	view.average_m_s = bike_data_average_m_s(data);
	view.distance_mm = data->distance_mm;
	view.time_ms = data->time_ms;
	view.paused = bike_data_is_paused(data);
	view.total_distance_mm = data->total_distance_mm;

	display_render(&view);
}

/* ======= Public ======= */

void bike_computer_init(BikeComputer* self, uint64_t now_ms) {
	memset(self, 0, sizeof(*self));
	bike_data_init(&self->data);

	bike_storage_init();
	display_init();
	wheel_sensor_init();
	button_init(&self->button, PIN_BUTTON);

	bike_storage_load(&self->data);

	self->last_revolution_count = wheel_sensor_read().revolution_count;
	self->last_update_ms = now_ms;
}

void bike_computer_update(BikeComputer* self, uint64_t now_ms) {
	update_data(self, now_ms);
	handle_input(self, now_ms);
	bike_storage_save_total_distance_if_needed(&self->data);
	render(self, now_ms);
}
