#include <Arduino.h>
#include <esp_timer.h>

#include "src/bike_computer/bike_computer.h"

static BikeComputer computer;

static uint64_t now_ms(void) { return (uint64_t)esp_timer_get_time() / 1000; }

void setup(void) {
	bike_computer_init(&computer, now_ms());
}

void loop(void) {
	bike_computer_update(&computer, now_ms());
	delay(10);
}
