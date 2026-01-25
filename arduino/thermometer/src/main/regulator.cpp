#include "regulator.h"

#include <Arduino.h>

#include "logger.h"
#include "config.h"
#include "constants.h"

void Regulator::init(unsigned long start_time, float start_temperature) {
	_previous_time = start_time;
	_previous_temp = start_temperature;
}

void Regulator::update(unsigned long time, float temp) {
	_update_speed(time, temp);

	_is_on = _regulate(time, temp);
	digitalWrite(Relay::pin, _is_on ? HIGH : LOW);

	_output(time, temp);
}

void Regulator::_update_speed(unsigned long time, float temp) {
	unsigned long delta = time - _previous_time;
	if (delta <= 0) return;


	float minutes = (float)delta / TimeMs::minute;
	_temp_speed = (temp - _previous_temp) / minutes;

	_previous_time = time;
	_previous_temp = temp;
}

bool Regulator::_regulate(unsigned long time, float temp) {
	float speed = 2.0f / TimeMs::minute;

	float temp_1 = 70.0f;
	unsigned long time_1 = TimeMs::hour;

	float temp_2 = 140.0f;
	unsigned long time_2 = 2 * TimeMs::hour;

	float temp_3 = 180.0f;
	unsigned long time_3 = 3 * TimeMs::hour;

	float temp_4 = 20.0f;

	switch (_state) {
		case 0:
			if (temp >= temp_1) {
				_state_time = time;
				_state++;
				return false;
			}
			return _temp_speed < speed;

		case 1:
			if (time >= _state_time + time_1) {
				_state++;
				return false;
			}
			return temp < temp_1;

		case 2:
			if (temp >= temp_2) {
				_state_time = time;
				_state++;
				return false;
			}
			return _temp_speed < speed;

		case 3:
			if (time >= _state_time + time_2) {
				_state++;
				return false;
			}
			return temp < temp_2;

		case 4:
			if (temp >= temp_3) {
				_state_time = time;
				_state++;
				return false;
			}
			return _temp_speed < speed;

		case 5:
			if (time >= _state_time + time_3) {
				_state++;
				return false;
			}
			return temp < temp_3;

		case 6:
			if (temp <= temp_4) {
				_state_time = time;
				_state++;
				return false;
			}
			return _temp_speed < -speed;

		default:
			return false;
	}
}

void Regulator::_output(unsigned long time, float temp) {
	char* status = _is_on ? "ON " : "OFF";
	Logger::get_instance().print("%03d.%01d C°, %03d.%01d C°/min, %s, (State: %d)\n", (int)temp, abs((int)(temp * 10)) % 10, (int)_temp_speed, abs((int)(_temp_speed * 10)) % 10, status, _state);
	Logger::get_instance().show("%s %03d.%01d C°", status, (int)temp, abs((int)(temp * 10)) % 10);
}
