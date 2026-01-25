#include <MAX6675.h>
#include <LiquidCrystal.h>

#include "logger.h"
#include "regulator.h"
#include "config.h"

MAX6675 thermo_couple(Max6675::select, Max6675::data, Max6675::clock);
LiquidCrystal lcd(Lcd::rs, Lcd::en, Lcd::d4, Lcd::d5, Lcd::d6, Lcd::d7);
Regulator regulator;

void setup() {
	Serial.begin(System::baud);

	pinMode(Relay::pin, OUTPUT);
	pinMode(Max6675::gnd, OUTPUT);
	pinMode(Max6675::vcc, OUTPUT);
	digitalWrite(Relay::pin, LOW);
	digitalWrite(Max6675::gnd, LOW);
	digitalWrite(Max6675::vcc, HIGH);

	SPI.begin();
	thermo_couple.begin();
	thermo_couple.setSPIspeed(4000000);

	lcd.begin(16, 2);
	Logger::lcd = &lcd;

	delay(500);

	int status = thermo_couple.read();
	regulator.init(millis(), thermo_couple.getCelsius());
}

void loop() {
	int status = thermo_couple.read();
	regulator.update(millis(), thermo_couple.getCelsius());
	delay(System::delay);
}
