#pragma once

#include <LiquidCrystal.h>

class Logger {
public:
	static LiquidCrystal* lcd;

	static Logger& get_instance();

	void print(const char* fmt, ...);
	void show(const char* fmt, ...);

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

private:
	Logger() = default;
	~Logger() = default;
};
