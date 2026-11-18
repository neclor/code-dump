/*
 *  INFO0064 - Embedded systems
 *
 *  Utility functions for using the board peripherals
 *
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/adc.h>


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
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOD);
  rcc_periph_clock_enable(RCC_GPIOG);
  rcc_periph_clock_enable(RCC_GPIOK);

  /* Setup I/O. */
  gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);//TB set PA1 to analog input mode
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4 | GPIO5);
  gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);
  gpio_mode_setup(GPIOK, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

  gpio_set(GPIOD, GPIO4 | GPIO5);
  gpio_set(GPIOG, GPIO6);
  gpio_set(GPIOK, GPIO3);

  /* Setup ADC */
  rcc_periph_clock_enable(RCC_ADC1);
  adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);
  adc_disable_scan_mode(ADC1);
  adc_set_single_conversion_mode(ADC1);
  adc_set_sample_time(ADC1, ADC_CHANNEL1, ADC_SMPR_SMP_3CYC);
  uint8_t channels = ADC_CHANNEL1; 
  adc_set_regular_sequence(ADC1, 1, &channels);
  adc_set_multi_mode(ADC_CCR_MULTI_INDEPENDENT);
  adc_power_on(ADC1);
  
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
      output_led(GPIOG, GPIO6, state);
      break;

    case LED_RED:
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


