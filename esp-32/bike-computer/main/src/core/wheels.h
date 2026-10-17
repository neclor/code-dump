#ifndef WHEELS_H
#define WHEELS_H

#include <stdint.h>

typedef struct {
	const char* name;
	uint16_t circumference_mm;
} WheelSize;

static const WheelSize WHEEL_SIZES[] = {
	{"20x1.75", 1515},
	{"24x1.75", 1890},
	{"26x1.95", 2050},
	{"26x2.10", 2068},
	{"27.5x2.10", 2148},
	{"29x2.10", 2288},
	{"700x23C", 2096},
	{"700x25C", 2105},
	{"700x28C", 2136},
	{"700x32C", 2155},
	{"700x35C", 2170},
	{"700x38C", 2180},
	{"700x40C", 2200},
	{"700x45C", 2220},
	{"650Bx47", 2130},
};

#define WHEEL_SIZES_COUNT (sizeof(WHEEL_SIZES) / sizeof(WHEEL_SIZES[0]))

#endif // WHEELS_H
