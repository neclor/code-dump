#include <math.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>


#include "utils.h"
#include "lcd.h"
#include "gfx.h"

static SemaphoreHandle_t sem;
static QueueHandle_t q;

#define DX ((LCD_WIDTH) / 6)
#define DY ((int) ((DX) * 0.57735 + 0.5))
#define AVG_WINDOW 10

static void draw_triangle(GFX_CTX *gfx, int x, int y, int dir,
                          int r, int g, int b, float f)
{
  int x1, x2, y1, y2, y3;
  GFX_COLOR c;

  x1 = (int) (x * (DX) * f + 0.5) + (LCD_WIDTH) / 2;
  x2 = (int) ((x + dir) * (DX) * f + 0.5) + (LCD_WIDTH) / 2;
  y1 = y * (DY) +  (LCD_HEIGHT) / 2;
  y2 = y1 - (DY);
  y3 = y1 + (DY);

  c.c.r = r;
  c.c.g = g;
  c.c.b = b;

  gfx_fill_triangle_abs(gfx, x1, y1, x2, y2, x2, y3, c);
}

static void draw_logo(GFX_CTX *g, float f)
{
  draw_triangle(g, -1, -3, -1, 228, 225, 219, f);
  draw_triangle(g, -2, -2,  1, 186, 185, 171, f);
  draw_triangle(g, -1, -1, -1, 250, 168,  25, f);
  draw_triangle(g, -2,  0,  1, 255, 206,   1, f);
  draw_triangle(g, -1,  1, -1, 244, 130,  69, f);
  draw_triangle(g, -2,  2,  1, 238,  58,  61, f);

  draw_triangle(g,  0, -2, -1, 173, 203, 126, f);
  draw_triangle(g, -1, -1,  1, 119, 191, 67,  f);
  draw_triangle(g,  0,  0, -1,  45, 163, 74 , f);
  draw_triangle(g, -1,  1,  1,   0, 141, 72,  f);
  draw_triangle(g,  0,  2, -1,   0, 190, 219, f);
  draw_triangle(g, -1,  3,  1,   0, 102, 178, f);

  draw_triangle(g,  1,  3, -1,  91,  44, 134, f);
  draw_triangle(g,  0,  2,  1, 159,  93, 164, f);
  draw_triangle(g,  2,  2, -1,  88,  94, 169, f);
  draw_triangle(g,  1,  1,  1, 127, 159, 210, f);

  draw_triangle(g,  2, -2, -1,   0, 118, 128, f);
  draw_triangle(g,  1, -3,  1,  91, 163, 173, f);
}

static void unsigned_to_string(unsigned d, char *s)
{
 s[10] = 0;
 for(int i = 0; i < 10; i++)
   {
     s[9-i] = (char) (d % 10 + 48); // Convert least significant digit into its ASCII value
     d = (unsigned) d / 10;     // Integer division
   }
}


static void task_logo(void *p __attribute__ ((unused)))
{
  GFX_CTX *gfx;
  float angle = 0.0;

  gfx = gfx_init(NULL, lcd_draw_pixel, (LCD_WIDTH), (LCD_HEIGHT),
                 GFX_FONT_LARGE);

  for (angle = 0;; angle -= 0.1)
    {
      /*
	fetch value and convert it into a string
      */
      unsigned x;
      xQueueReceive(q, &x, portMAX_DELAY);
      char *s = malloc(11);
      if (!s)
	continue;
      unsigned_to_string(x, s);

      /*
	display value
      */
      gfx_fill_screen(gfx, GFX_COLOR_BLACK);
      gfx_set_text_cursor(gfx, 10, 20);
      gfx_set_text_color(gfx, GFX_COLOR_WHITE, GFX_COLOR_BLACK);
      gfx_puts(gfx, s);
      /*
	draw logo
      */
      draw_logo(gfx, cosf(angle));
      lcd_show_frame();
    }
}




static void task_adc(void *p __attribute__ ((unused)))
{
  double sum = 0;
  unsigned cnt = 0;

  double sq_sum = 0;

  adc_start_conversion_regular(ADC1);
  for(;;cnt++)
    {
      xSemaphoreTake(sem, portMAX_DELAY);
      unsigned res;
      res = adc_read_regular(ADC1);
      res = res & 0x00000FFF;
      sum += (double)res * 1000 * 3.3 / (double) 0x00000FFF;

      double res_d = (double)res * 1000 * 3.3 / (double) 0x00000FFF;
      sq_sum += res_d * res_d;

      if(cnt == AVG_WINDOW)
      {
        unsigned sq_mean = (unsigned) (sq_sum / AVG_WINDOW);
        unsigned rt_sq_mean = (unsigned) sqrt(sq_mean);

        unsigned mean = (unsigned) (sum / AVG_WINDOW);
        xQueueSend(q, &rt_sq_mean, portMAX_DELAY);
        cnt = 0;
        sum = 0;
        sq_sum = 0;
      }
      adc_start_conversion_regular(ADC1);
    }


}



static int led_1_state = 0;
void adc_isr(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  set_led(LED_GREEN, led_1_state);
  led_1_state = !led_1_state;

  xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
  adc_clear_flag(ADC1,ADC_SR_EOC);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }

int main(void)
{
  /* Initialize board */

  init_board();

  set_led(LED_RED, 0);
  set_led(LED_GREEN, 0);

  /* Semaphore */

  sem = xSemaphoreCreateBinary();

  /*Queue */

  q = xQueueCreate(10, sizeof(unsigned));

  /* Interrupts */

  exti_select_source(EXTI0, GPIOA);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
  exti_enable_request(EXTI0);

  nvic_set_priority(NVIC_EXTI0_IRQ, 0x10);
  nvic_enable_irq(NVIC_EXTI0_IRQ);

  adc_enable_eoc_interrupt(ADC1);

  nvic_set_priority(NVIC_ADC_IRQ, 0x10);
  nvic_enable_irq(NVIC_ADC_IRQ);
  /* Create tasks */

  xTaskCreate(task_logo, "logo", 512, NULL, 3, NULL);
  xTaskCreate(task_adc, "acd", 512, NULL, 4, NULL);

  /* Start OS */

  vTaskStartScheduler();

  /* We should never get here */

  for (;;);
}

