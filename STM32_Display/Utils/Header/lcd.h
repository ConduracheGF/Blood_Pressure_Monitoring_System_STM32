/*
 * lcd.h
 *
 *	@attention
 *	Acest fisier este creat cu scop de header pentru driverele personale.
 *	Se vor crea functionalitati de baza ale display-ului.
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_LCD_H_
#define HEADER_LCD_H_

#include "stdint.h"
#include "stdbool.h"

// Dimensiuni LCD
#define LCD_WIDTH 320
#define LCD_HEIGHT 480

// CULORI RGB
#define CULOARE_NEGRU 0x0000
#define CULOARE_ALB 0xFFFF
#define CULOARE_ROSU 0xF800
#define CULOARE_VERDE 0x07E0
#define CULOARE_ALBASTRU 0x001F

// comenzi driver display ILI9486
#define CMD_SLEEP_OUT 0x11
#define CMD_DISPLAY_ON 0x29
#define CMD_COLUMN_ADDR_SET 0x2A
#define CMD_ROW_ADDR_SET 0x2B
#define CMD_MEMORY_WRITE 0x2C
#define CMD_MEMORY_ACCESS 0x36
#define CMD_PIXEL_FORMAT_16BIT 0x3A
#define CMD_POWER_CTRL_3 0xC2
#define CMD_VCOM_CTRL 0xC5
#define CMD_GAMMA_POSITIVE 0xE0
#define CMD_GAMMA_NEGATIVE 0xE1
#define CMD_GAMMA_DIGITAL 0xE2

// Command parameters
#define PIXEL_FORMAT_16BIT 0x55
#define MEMORY_ACCESS_PORTRAIT_BGR 0x48

void lcd_init();
bool lcd_set_window(uint16_t x_start, uint16_t y_start, uint16_t x_finish, uint16_t y_finish);
void lcd_write_pixels(const uint16_t *pixels, uint32_t nr_of_pixels);
void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

#endif /* HEADER_LCD_H_ */
