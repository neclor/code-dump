#include <Arduino.h>
#include <esp_timer.h>

#include "wheel_sensor.h"

#define PIN_WHEEL_SENSOR 32

#define ANALOG_HALL_SAMPLE_PERIOD_MS 1
#define ANALOG_HALL_CALIBRATION_SAMPLES 64
#define ANALOG_HALL_TRIGGER_MV 300
#define ANALOG_HALL_RELEASE_MV 100
#define ANALOG_HALL_DEBOUNCE_MS 50
#define ANALOG_HALL_STOP_TIMEOUT_MS 3000

static WheelSensorData state;
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

static int rest_mv;
static bool magnet_near;

/* ======= Static ======= */

static uint64_t now_ms(void) { return (uint64_t)esp_timer_get_time() / 1000; }

static void count_revolution(uint64_t now) {
	portENTER_CRITICAL(&mux);
	const uint64_t period = now - state.last_revolution_time_ms;
	if (state.revolution_count == 0 || period >= ANALOG_HALL_DEBOUNCE_MS) {
		state.last_period_ms = state.revolution_count == 0 ? 0 : period;
		state.last_revolution_time_ms = now;
		state.revolution_count++;
	}
	portEXIT_CRITICAL(&mux);
}

static void sample_task(void* arg) {
	while (true) {
		const int deviation_mv = abs((int)analogReadMilliVolts(PIN_WHEEL_SENSOR) - rest_mv);

		if (!magnet_near && deviation_mv > ANALOG_HALL_TRIGGER_MV) {
			magnet_near = true;
			count_revolution(now_ms());
		} else if (magnet_near && deviation_mv < ANALOG_HALL_RELEASE_MV) {
			magnet_near = false;
		}

		vTaskDelay(pdMS_TO_TICKS(ANALOG_HALL_SAMPLE_PERIOD_MS));
	}
}

/* ======= Public ======= */

void wheel_sensor_init(void) {
	analogSetPinAttenuation(PIN_WHEEL_SENSOR, ADC_11db);

	long sum_mv = 0;
	for (int i = 0; i < ANALOG_HALL_CALIBRATION_SAMPLES; i++) {
		sum_mv += analogReadMilliVolts(PIN_WHEEL_SENSOR);
		delay(2);
	}
	rest_mv = sum_mv / ANALOG_HALL_CALIBRATION_SAMPLES;

	xTaskCreatePinnedToCore(sample_task, "wheel_sensor", 2048, NULL, 1, NULL, 0);
}

WheelSensorData wheel_sensor_read(void) {
	portENTER_CRITICAL(&mux);
	const WheelSensorData data = state;
	portEXIT_CRITICAL(&mux);
	return data;
}

bool wheel_sensor_is_stopped(uint64_t now_ms) {
	const WheelSensorData data = wheel_sensor_read();
	if (data.last_period_ms == 0 || data.last_period_ms > ANALOG_HALL_STOP_TIMEOUT_MS) return true;
	return now_ms > data.last_revolution_time_ms && now_ms - data.last_revolution_time_ms > ANALOG_HALL_STOP_TIMEOUT_MS;
}
