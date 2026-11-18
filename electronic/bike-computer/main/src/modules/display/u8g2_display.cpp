#include <U8g2lib.h>

#include "../../utils/utils.h"
#include "display.h"

#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

#define LEFT_HALF 0
#define RIGHT_HALF 66

#define FONT_SMALL u8g2_font_6x10_tf
#define FONT_BIG u8g2_font_9x15B_tf

static u8g2_t u8g2;

/* ======= Static ======= */

static void draw_value(int x, int y, const char* value, const char* label) {
	u8g2_SetFont(&u8g2, FONT_BIG);
	const int width = u8g2_DrawStr(&u8g2, x, y, value);

	u8g2_SetFont(&u8g2, FONT_SMALL);
	u8g2_DrawStr(&u8g2, x + width + 1, y, label);
}

/* ======= Public ======= */

void display_init(void) {
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
	u8x8_SetPin_HW_I2C(u8g2_GetU8x8(&u8g2), U8X8_PIN_NONE, PIN_I2C_SCL, PIN_I2C_SDA);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
}

void display_render(const ViewModel* v) {
	const unsigned long s = v->time_ms / 1000;
	char text[24];

	u8g2_ClearBuffer(&u8g2);
	u8g2_DrawVLine(&u8g2, 63, 0, 64);

	u8g2_SetFont(&u8g2, FONT_SMALL);
	snprintf(text, sizeof(text), "%lu:%02lu:%02lu", s / 3600, s / 60 % 60, s % 60);
	u8g2_DrawStr(&u8g2, LEFT_HALF, 11, text);
	if (v->paused) u8g2_DrawStr(&u8g2, RIGHT_HALF, 11, "PAUSE");

	snprintf(text, sizeof(text), "%.2f", mm_to_km(v->distance_mm));
	draw_value(LEFT_HALF, 36, text, "km");
	snprintf(text, sizeof(text), "%.1f", m_s_to_kmh(v->speed_m_s));
	draw_value(RIGHT_HALF, 36, text, "km/h");

	snprintf(text, sizeof(text), "%.0f", mm_to_km(v->total_distance_mm));
	draw_value(LEFT_HALF, 56, text, "tot");
	snprintf(text, sizeof(text), "%.1f", m_s_to_kmh(v->average_m_s));
	draw_value(RIGHT_HALF, 56, text, "avg");

	u8g2_SendBuffer(&u8g2);
}
