#pragma once

#include <stdint.h>

class Regulator {
public:
	void init(unsigned long start_time, float start_temp);
	void update(unsigned long time, float temp);

private:
	unsigned long _previous_time = 0;
	float _previous_temp = 0.0f;

	float _temp_speed = 0.0f;

	bool _is_on = false;
	int _state = 0;
	unsigned long _state_time = 0;

	void _update_speed(unsigned long time, float temp);
	bool _regulate(unsigned long time, float temp);
	void _output(unsigned long time, float temp);

	unsigned long _target_temperature_at(unsigned long time);
};
