#include <math.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

#include "utils.h"
#include "lcd.h"
#include "gfx.h"

static SemaphoreHandle_t sem;

/* --- Swamp ogre ----------------------------------------------------------
 * Coordinates are given for a 240 px wide screen, origin at screen centre,
 * y pointing down. They are scaled to LCD_WIDTH, and x is additionally
 * multiplied by f to fake a rotation around the vertical axis.
 */

#define OGRE_S ((float) (LCD_WIDTH) / 240.0f)

static GFX_COLOR rgb(int r, int g, int b)
{
  GFX_COLOR c;

  c.c.r = r;
  c.c.g = g;
  c.c.b = b;
  return c;
}

static int ox(float x, float f)
{
  return (int) (x * OGRE_S * f + ((LCD_WIDTH) / 2) + 0.5f);
}

static int oy(float y)
{
  return (int) (y * OGRE_S + ((LCD_HEIGHT) / 2) + 0.5f);
}

static void ogre_ellipse(GFX_CTX *g, float cx, float cy, float rx, float ry,
                         float f, GFX_COLOR c)
{
  int dy, y0, y1, ry_px;
  float t, hw;

  ry_px = (int) (ry * OGRE_S + 0.5f);
  if (ry_px <= 0)
    return;

  y0 = oy(cy);
  for (dy = -ry_px; dy <= ry_px; dy++)
    {
      t = (float) dy / ry_px;
      hw = rx * sqrtf(1.0f - t * t);
      y1 = y0 + dy;
      gfx_draw_line_abs(g, ox(cx - hw, f), y1, ox(cx + hw, f), y1, c);
    }
}

static void ogre_triangle(GFX_CTX *g, float x0, float y0, float x1, float y1,
                          float x2, float y2, float f, GFX_COLOR c)
{
  gfx_fill_triangle_abs(g, ox(x0, f), oy(y0), ox(x1, f), oy(y1),
                        ox(x2, f), oy(y2), c);
}

static void draw_logo(GFX_CTX *g, float f)
{
  GFX_COLOR skin  = rgb(118, 168,  48);
  GFX_COLOR dark  = rgb( 62,  98,  22);
  GFX_COLOR light = rgb(150, 196,  72);
  GFX_COLOR horn  = rgb(232, 218, 182);
  GFX_COLOR white = rgb(250, 250, 240);
  GFX_COLOR pupil = rgb( 70,  40,  20);
  GFX_COLOR mouth = rgb(110,  20,  30);
  GFX_COLOR shirt = rgb(150, 110,  60);

  /* Shoulders / vest */
  ogre_ellipse(g, 0, 120, 110, 40, f, shirt);

  /* Horns */
  ogre_triangle(g, -70, -55, -38, -78, -92, -125, f, horn);
  ogre_triangle(g,  70, -55,  38, -78,  92, -125, f, horn);

  /* Head */
  ogre_ellipse(g, 0, 0, 88, 92, f, skin);

  /* Messy hair tuft */
  ogre_ellipse(g, -14, -86, 14, 10, f, dark);
  ogre_ellipse(g,   6, -90, 12, 10, f, dark);
  ogre_ellipse(g,  22, -84, 10,  8, f, dark);

  /* Seen from behind: only head, horns and hair */
  if (f < 0)
    return;

  /* Heavy brow */
  ogre_triangle(g, -58, -42, -12, -36, -50, -50, f, dark);
  ogre_triangle(g,  58, -42,  12, -36,  50, -50, f, dark);

  /* Eyes */
  ogre_ellipse(g, -32, -18, 17, 20, f, white);
  ogre_ellipse(g,  32, -18, 17, 20, f, white);
  ogre_ellipse(g, -28, -14,  7,  9, f, pupil);
  ogre_ellipse(g,  28, -14,  7,  9, f, pupil);

  /* Cheeks */
  ogre_ellipse(g, -56, 28, 14, 9, f, light);
  ogre_ellipse(g,  56, 28, 14, 9, f, light);

  /* Big flat nose */
  ogre_ellipse(g,  0, 14, 20, 13, f, dark);
  ogre_ellipse(g, -7, 16,  4,  3, f, pupil);
  ogre_ellipse(g,  7, 16,  4,  3, f, pupil);

  /* Wide grin: dark ellipse, top half covered by skin */
  ogre_ellipse(g, 0, 50, 46, 20, f, mouth);
  ogre_ellipse(g, 0, 40, 48, 13, f, skin);

  /* Single crooked tooth */
  ogre_triangle(g, 10, 46, 24, 46, 17, 60, f, white);
}

static void task_logo(void *p __attribute__ ((unused)))
{
  GFX_CTX *gfx;
  float angle = 0.0;

  gfx = gfx_init(NULL, lcd_draw_pixel, (LCD_WIDTH), (LCD_HEIGHT),
                 GFX_FONT_LARGE);

  for (angle = 0;; angle -= 0.1)
    {
      gfx_fill_screen(gfx, GFX_COLOR_BLACK);

      draw_logo(gfx, cosf(angle));
      lcd_show_frame();
    }
}

static void task_led(void *p __attribute__ ((unused)))
{
  static int state = 0;

  for (;;)
    {
      xSemaphoreTake(sem, portMAX_DELAY);
      set_led(LED_RED, state);

      state = !state;
    }
}

void exti0_isr(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  static int state = 0;

  set_led(LED_GREEN, state);
  state = !state;

  xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
  exti_reset_request(EXTI0);
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

  /* Interrupts */

  exti_select_source(EXTI0, GPIOA);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
  exti_enable_request(EXTI0);

  nvic_set_priority(NVIC_EXTI0_IRQ, 0x10);
  nvic_enable_irq(NVIC_EXTI0_IRQ);

  /* Create tasks */

  xTaskCreate(task_logo, "logo", 512, NULL, 3, NULL);
  xTaskCreate(task_led, "led", 512, NULL, 4, NULL);

  /* Start OS */

  vTaskStartScheduler();

  /* We should never get here */

  for (;;);
}
