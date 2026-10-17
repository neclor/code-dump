#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint8_t pin;
	bool down;
	bool was_down;
	uint64_t changed_ms;
} Button;

void button_init(Button* self, uint8_t pin);
void button_update(Button* self, uint64_t now_ms);

bool button_is_down(const Button* self);
bool button_pressed(const Button* self);
bool button_released(const Button* self);
uint64_t button_held_ms(const Button* self, uint64_t now_ms);

#endif // BUTTON_H
