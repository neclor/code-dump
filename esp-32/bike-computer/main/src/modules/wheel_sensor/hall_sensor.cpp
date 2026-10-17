// #include <Arduino.h>
// #include <esp_timer.h>
//
// #include "wheel_sensor.h"
//
// #define PIN_WHEEL_SENSOR 32
// #define HALL_SENSOR_DEBOUNCE_MS 50
// #define HALL_SENSOR_STOP_TIMEOUT_MS 3000
//
// static WheelSensorData state;
// static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
//
// /* ======= Static ======= */
//
// static void IRAM_ATTR on_pulse(void) {
// 	const uint64_t now = (uint64_t)esp_timer_get_time() / 1000;
//
// 	portENTER_CRITICAL_ISR(&mux);
// 	const uint64_t period = now - state.last_revolution_time_ms;
// 	if (state.revolution_count == 0 || period >= HALL_SENSOR_DEBOUNCE_MS) {
// 		state.last_period_ms = state.revolution_count == 0 ? 0 : period;
// 		state.last_revolution_time_ms = now;
// 		state.revolution_count++;
// 	}
// 	portEXIT_CRITICAL_ISR(&mux);
// }
//
// /* ======= Public ======= */
//
// void wheel_sensor_init(void) {
// 	pinMode(PIN_WHEEL_SENSOR, INPUT_PULLUP);
// 	attachInterrupt(digitalPinToInterrupt(PIN_WHEEL_SENSOR), on_pulse, FALLING);
// }
//
// WheelSensorData wheel_sensor_read(void) {
// 	portENTER_CRITICAL(&mux);
// 	const WheelSensorData data = state;
// 	portEXIT_CRITICAL(&mux);
// 	return data;
// }
//
// bool wheel_sensor_is_stopped(uint64_t now_ms) {
// 	const WheelSensorData data = wheel_sensor_read();
// 	if (data.last_period_ms == 0 || data.last_period_ms > HALL_SENSOR_STOP_TIMEOUT_MS) return true;
// 	return now_ms > data.last_revolution_time_ms && now_ms - data.last_revolution_time_ms > HALL_SENSOR_STOP_TIMEOUT_MS;
// }
