#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	float speed_m_s;
	float average_m_s;
	uint64_t distance_mm;
	uint64_t time_ms;
	bool paused;
	uint64_t total_distance_mm;
} ViewModel;

void display_init(void);
void display_render(const ViewModel* view);

#endif // DISPLAY_H
