/*
 *  INFO0064 - Embedded systems
 *
 *  Busy-waiting delays for initializing SDRAM and LCD
 *
 */

#include "delay.h"

void delay_1ms(void)
{
  int i;

  for (i = 0; i < 50000; i++)
    __asm__("nop");
}

void delay_20ms(void)
{
  int i;

  for (i = 0; i < 1000000; i++)
    __asm__("nop");
}

void delay_200ms(void)
{
  int i;

  for (i = 0; i < 10000000; i++)
    __asm__("nop");
}
