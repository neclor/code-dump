#include <stdlib.h>
#include <math.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h> 

#include "lcd.h"
#include "gfx.h"
#include "utils.h"

#define TABLE_RADIUS ((LCD_WIDTH) / 4)

static volatile int philo_is_init = 0, screen_is_busy = 0;
static unsigned char chairs[NB_PHILOSOPHERS], forks[NB_PHILOSOPHERS];

static SemaphoreHandle_t fork_sems[NB_PHILOSOPHERS];

static void display_philo(int, int, int);
static void redraw(void);
static void check_init(void);

static void display_philo(int x, int y, int s)
{
  y += s;
  gfx_draw_circle_at(cur_gfx, x, y - 2 * s, 3 * s / 4, GFX_COLOR_GREEN);
  gfx_draw_line_abs(cur_gfx, x, y, x, y - 5 * s / 4, GFX_COLOR_GREEN);
  gfx_draw_line_abs(cur_gfx, x - s, y, x + s, y, GFX_COLOR_GREEN);
  gfx_draw_line_abs(cur_gfx, x - s, y + s, x, y, GFX_COLOR_GREEN);
  gfx_draw_line_abs(cur_gfx, x + s, y + s, x, y, GFX_COLOR_GREEN);
}

static void redraw(void)
{
  int   is_busy, i, x, y, x2, y2;
  float a;

  for (;;)
    {
      taskENTER_CRITICAL();
      is_busy = screen_is_busy;
      screen_is_busy = 1;
      taskEXIT_CRITICAL();

      if (!is_busy)
        break;

      vTaskDelay(pdMS_TO_TICKS(50));
    }
 
  gfx_fill_screen(cur_gfx, GFX_COLOR_BLACK);
  gfx_fill_circle_at(cur_gfx, (LCD_WIDTH) / 2, (LCD_HEIGHT) / 2,
                     (TABLE_RADIUS), GFX_COLOR_LTYELLOW);

  for (i = 0; i < NB_PHILOSOPHERS; i++)
    {
      a = (2 * M_PI * i) / NB_PHILOSOPHERS; 
      x = (LCD_WIDTH) / 2 + 0.75 * (TABLE_RADIUS) * cosf(a);
      y = (LCD_HEIGHT) / 2 + 0.75 * (TABLE_RADIUS) * sinf(a);
      gfx_draw_circle_at(cur_gfx, x, y,
                     (TABLE_RADIUS) / 5, GFX_COLOR_BLUE);

      if (chairs[i])
          display_philo((LCD_WIDTH) / 2 + 1.5 * (TABLE_RADIUS) * cosf(a),
                        (LCD_HEIGHT) / 2 + 1.5 * (TABLE_RADIUS) * sinf(a),
                        (LCD_WIDTH) / 20);
      
      a += M_PI / NB_PHILOSOPHERS;

      if (forks[i])
        {
          x = (LCD_WIDTH) / 2 + 0.7 * (TABLE_RADIUS) * cosf(a);
          y = (LCD_HEIGHT) / 2 + 0.7 * (TABLE_RADIUS) * sinf(a);

          x2 = (LCD_WIDTH) / 2 + 0.9 * (TABLE_RADIUS) * cosf(a);
          y2 = (LCD_HEIGHT) / 2 + 0.9 * (TABLE_RADIUS) * sinf(a);
          
          
          gfx_draw_line_at(cur_gfx, x, y, x2, y2, GFX_COLOR_RED);
        }
    }
  
  lcd_show_frame();

  screen_is_busy = 0;
}

static void check_init(void)
{
  int is_init, i;
  
  taskENTER_CRITICAL();
  is_init = philo_is_init;
  philo_is_init = 1;
  taskEXIT_CRITICAL();

  if (is_init)
    return;

  taskENTER_CRITICAL();
  for (i = 0; i < NB_PHILOSOPHERS; i++)
    {
      chairs[i] = 0;
      forks[i] = 1;
      fork_sems[i] = xSemaphoreCreateBinary();
      xSemaphoreGive(fork_sems[i]);
    }
  taskEXIT_CRITICAL();

  redraw();
}

void think(void)
{
  check_init();
  vTaskDelay(pdMS_TO_TICKS(200 + random() % 500));
}

void eat(void)
{
  check_init();
  vTaskDelay(pdMS_TO_TICKS(random() % 100));
}

void sit(int n)
{
  if (n <= 0 || n > NB_PHILOSOPHERS)
    return;

  check_init();
  
  taskENTER_CRITICAL();
  chairs[n - 1] = 1;
  taskEXIT_CRITICAL();

  redraw();
}

void leave(int n)
{
  if (n <= 0 || n > NB_PHILOSOPHERS)
    return;

  check_init();

  taskENTER_CRITICAL();
  chairs[n - 1] = 0;
  taskEXIT_CRITICAL();

  redraw();
}

void take_fork(int n, int s)
{
  if (n <= 0 || n > NB_PHILOSOPHERS)
    return;

  n += 2 * NB_PHILOSOPHERS - 1;
  
  if (s)
    n--;

  n %= NB_PHILOSOPHERS;
  
  check_init();

  xSemaphoreTake(fork_sems[n], portMAX_DELAY);
  taskENTER_CRITICAL();
  forks[n] = 0;
  taskEXIT_CRITICAL();

  redraw();
}

void release_fork(int n, int s)
{
   if (n <= 0 || n > NB_PHILOSOPHERS)
    return;

  n += 2 * NB_PHILOSOPHERS - 1;
  
  if (s)
    n--;

  n = n % NB_PHILOSOPHERS;
  
  check_init();

  xSemaphoreGive(fork_sems[n]);
  taskENTER_CRITICAL();
  forks[n] = 1;
  taskEXIT_CRITICAL();

  redraw();
}
