#include "button.h"

#include <Arduino.h>
#include <string.h>

#define BUTTON_DEBOUNCE_MS 30

/* ======= Public ======= */

void button_init(Button* self, uint8_t pin) {
	memset(self, 0, sizeof(*self));
	self->pin = pin;
	pinMode(pin, INPUT_PULLUP);
}

void button_update(Button* self, uint64_t now_ms) {
	self->was_down = self->down;
	if (now_ms - self->changed_ms < BUTTON_DEBOUNCE_MS) return;

	const bool down = digitalRead(self->pin) == LOW;
	if (down == self->down) return;

	self->down = down;
	self->changed_ms = now_ms;
}

bool button_is_down(const Button* self) { return self->down; }

bool button_pressed(const Button* self) { return self->down && !self->was_down; }

bool button_released(const Button* self) { return !self->down && self->was_down; }

uint64_t button_held_ms(const Button* self, uint64_t now_ms) { return self->down ? now_ms - self->changed_ms : 0; }
