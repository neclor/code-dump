#ifndef WHEEL_SENSOR_H
#define WHEEL_SENSOR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint32_t revolution_count;
	uint64_t last_period_ms;
	uint64_t last_revolution_time_ms;
} WheelSensorData;

void wheel_sensor_init(void);
WheelSensorData wheel_sensor_read(void);
bool wheel_sensor_is_stopped(uint64_t now_ms);

#endif // WHEEL_SENSOR_H
