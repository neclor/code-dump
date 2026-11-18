/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014-2015 Chuck McManis <cmcmanis@mcmanis.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  INFO0064: Adapted to FreeRTOS (2021).
 *
 */

#ifndef LCD_SPI_H
#define LCD_SPI_H

#include "helpers.h"
#include "gfx.h"

#define FRAMEBUFFER_ADDRESS (0xc1000000U - 0x200000U)

void lcd_init(void);
void lcd_show_frame(void);
void lcd_draw_pixel(int, int, GFX_COLOR);

#define LCD_WIDTH   480
#define LCD_HEIGHT  800

#endif
