#ifndef BIKE_COMPUTER_H
#define BIKE_COMPUTER_H

#include <stdbool.h>
#include <stdint.h>

#include "../core/bike_data/bike_data.h"
#include "../modules/button/button.h"

typedef struct {
	BikeData data;

	Button button;

	uint32_t last_revolution_count;
	uint64_t last_update_ms;
	uint64_t last_render_ms;
} BikeComputer;

void bike_computer_init(BikeComputer* self, uint64_t now_ms);
void bike_computer_update(BikeComputer* self, uint64_t now_ms);

#endif // BIKE_COMPUTER_H
