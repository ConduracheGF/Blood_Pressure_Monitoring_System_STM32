/*
 * lcd_io.h
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_LCD_IO_H_
#define HEADER_LCD_IO_H_

#include <stdint.h>

void lcd_io_init();
void lcd_io_reset();
void lcd_io_write_data(uint8_t data);
void lcd_io_write_command(uint8_t cmd);
void lcd_io_write_pixels(const uint16_t *pixels, uint32_t nr_of_pixels);
void lcd_io_fill_color(uint16_t color, uint32_t nr_of_pixels);
uint16_t lcd_io_test_data_bus();

#endif /* HEADER_LCD_IO_H_ */
