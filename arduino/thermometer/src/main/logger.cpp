#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <Arduino.h>

LiquidCrystal* Logger::lcd = nullptr;

Logger& Logger::get_instance() {
	static Logger instance;
    return instance;
}

void Logger::print(const char* fmt, ...) {
	char buf[128];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

	Serial.print(buf);
}

void Logger::show(const char* fmt, ...) {
	if (lcd == nullptr) return;

	char buf[17];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

	lcd->setCursor(0, 0);
	lcd->print("                ");
	lcd->setCursor(0, 0);
	lcd->print(buf);
}
