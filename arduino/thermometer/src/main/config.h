#pragma once

#include <stdint.h>

namespace System {
	constexpr unsigned long baud = 9600;
	constexpr unsigned long delay = 5000;
}

namespace Relay {
	constexpr uint8_t pin = 2;
}

namespace Max6675 {
	constexpr uint8_t gnd = 7;
	constexpr uint8_t vcc = 6;
	constexpr uint8_t clock = 5; // SCK / CLK
	constexpr uint8_t select = 4; // CS
	constexpr uint8_t data = 3; // SO / DO
}

namespace Lcd {
	constexpr uint8_t rs = 13;
	constexpr uint8_t en = 12;
	constexpr uint8_t d4 = 11;
	constexpr uint8_t d5 = 10;
	constexpr uint8_t d6 = 9;
	constexpr uint8_t d7 = 8;
}
