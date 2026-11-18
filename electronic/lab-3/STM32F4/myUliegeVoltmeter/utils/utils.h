/*
 *  INFO0064 - Embedded systems
 *
 *  Utility functions for using the board peripherals
 *
 */

#include "gfx.h"
#include "philo.h"

extern GFX_CTX *cur_gfx;

void  init_board(void);

#define LED_GREEN  1
#define LED_RED    2
#define LED_ORANGE 3
#define LED_BLUE   4

void  set_led(int, int);

void  display_text(char *);
