/*
 *  INFO0064 - Embedded systems
 *
 *  Utility functions for using the board peripherals
 *
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "utils.h"
#include "sdram.h"
#include "lcd.h"
#include "gfx.h"

#ifndef NULL
#define NULL	(void *)(0)
#endif

GFX_CTX *cur_gfx = NULL;

static void output_led(int, int, int);

static void output_led(int port, int pin, int state)
{
  if (state)
    gpio_clear(port, pin);
  else
    gpio_set(port, pin);
}

/* Initializes the STM32F429 board, including SDRAM and LCD */

void init_board(void)
{
  /* Set clock to 180 MHz. */
  
  rcc_clock_setup_pll(rcc_hse_8mhz_3v3 + RCC_CLOCK_3V3_180MHZ);

  /* Enable GPIOG clock. */

  rcc_periph_clock_enable(RCC_GPIOD);
  rcc_periph_clock_enable(RCC_GPIOG);
  rcc_periph_clock_enable(RCC_GPIOK);
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Setup I/O. */
  
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4 | GPIO5);
  gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);
  gpio_mode_setup(GPIOK, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

  gpio_set(GPIOD, GPIO4 | GPIO5);
  gpio_set(GPIOG, GPIO6);
  gpio_set(GPIOK, GPIO3);

  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2 | GPIO6);
  gpio_clear(GPIOA, GPIO2 | GPIO6);
  
  /* Init SDRAM */

  sdram_init();

  /* Init LCD */

  lcd_init();

  /* GFX init */
 
   cur_gfx = gfx_init(NULL, lcd_draw_pixel, (LCD_WIDTH), (LCD_HEIGHT),
                      GFX_FONT_LARGE);
   gfx_fill_screen(cur_gfx, GFX_COLOR_BLACK);
   lcd_show_frame();
}

/* Sets a LED on or off */

void set_led(int nb, int state)
{
  switch (nb)
    {
    case LED_GREEN:
      output_led(GPIOA, GPIO2, !state);
      output_led(GPIOG, GPIO6, state);
      break;

    case LED_RED:
      output_led(GPIOA, GPIO6, !state);
      output_led(GPIOD, GPIO5, state);
      break;

    case LED_ORANGE:
      output_led(GPIOD, GPIO4, state);
      break;

    case LED_BLUE:
      output_led(GPIOK, GPIO3, state);
      break;
      
    }
}

/* Displays a text on the screen */

void display_text(char *message)
{
  gfx_fill_screen(cur_gfx, GFX_COLOR_BLACK);
  gfx_set_text_cursor(cur_gfx, 10, (LCD_HEIGHT) / 2);
  gfx_set_text_color(cur_gfx, GFX_COLOR_WHITE, GFX_COLOR_BLACK);
  gfx_puts(cur_gfx, message);

  lcd_show_frame();
}
