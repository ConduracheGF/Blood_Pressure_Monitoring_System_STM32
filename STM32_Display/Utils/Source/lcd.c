/*
 * lcd.c
 *	Fisier ce contine corpurile de definitie ale functionalitatilor de baza ale display-ului.
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "Utils/Header/lcd.h"
#include "IO/Header/sys_io.h"
#include "IO/Header/lcd_io.h"

static void write_command_with_parameters(uint8_t cmd, const uint8_t *parameters, uint8_t nr_of_parameters)
{
	lcd_io_write_command(cmd);
	for (uint8_t index = 0; index < nr_of_parameters; index++)
	{
		lcd_io_write_data(parameters[index]);
	}
}

void lcd_init()
{
	static const uint8_t gamma_positive[15] = {
			0x0F, 0x1F, 0x1C, 0x0C, 0x0F,
			0x08, 0x48, 0x98, 0x37, 0x0A,
			0x13, 0x04, 0x11, 0x0D, 0x00
	};

	static const uint8_t gamma_negative[15] = {
			0x07, 0x32, 0x2E, 0x0B, 0x0D,
			0x05, 0x47, 0x75, 0x37, 0x06,
			0x10, 0x03, 0x24, 0x20, 0x00
	};

	lcd_io_init();
	lcd_io_reset();

	lcd_io_write_command(CMD_SLEEP_OUT);
	sys_io_delay_ms(120);

	write_command_with_parameters(CMD_PIXEL_FORMAT_16BIT, (const uint8_t[]){PIXEL_FORMAT_16BIT}, 1);
	write_command_with_parameters(CMD_POWER_CTRL_3, (const uint8_t[]){0x44}, 1);
	write_command_with_parameters(CMD_GAMMA_POSITIVE, gamma_positive, 15);
	write_command_with_parameters(CMD_GAMMA_NEGATIVE, gamma_negative, 15);
	write_command_with_parameters(CMD_GAMMA_DIGITAL, gamma_negative, 15);
	write_command_with_parameters(CMD_MEMORY_ACCESS, (const uint8_t[]){MEMORY_ACCESS_PORTRAIT_BGR}, 1);

	lcd_io_write_command(CMD_DISPLAY_ON);
	sys_io_delay_ms(25);
}

/*
 *  Scrie in memorie in zona selectata de display din RAM
 *  Daca fereastra este invalida sau este afara din ecran
 *  => false = send nothing
*/
bool lcd_set_window(uint16_t x_start, uint16_t y_start, uint16_t x_finish, uint16_t y_finish)
{
	if (x_start > x_finish || y_start > y_finish)
	{
		return false;
	} else if (x_finish >= LCD_WIDTH || y_finish >= LCD_HEIGHT){
		return false;
	}

	lcd_io_write_command(CMD_COLUMN_ADDR_SET);
	lcd_io_write_data(x_start >> 8);
	lcd_io_write_data(x_start & 0xFF);
	lcd_io_write_data(x_finish >> 8);
	lcd_io_write_data(x_finish & 0xFF);

	lcd_io_write_command(CMD_ROW_ADDR_SET);
	lcd_io_write_data(y_start >> 8);
	lcd_io_write_data(y_start & 0xFF);
	lcd_io_write_data(y_finish >> 8);
	lcd_io_write_data(y_finish & 0xFF);

	lcd_io_write_command(CMD_MEMORY_WRITE);
	return true;
}

void lcd_write_pixels(const uint16_t *pixels, uint32_t nr_of_pixels)
{
	lcd_io_write_pixels(pixels, nr_of_pixels);
}

void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	if (width == 0 || height == 0 || x >= LCD_WIDTH || y >= LCD_HEIGHT)
	{
		return;
	}

	if (x + width > LCD_WIDTH)
	{
		width = LCD_WIDTH - x;
	}
	if (y + height > LCD_HEIGHT)
	{
		height = LCD_HEIGHT - y;
	}

	lcd_set_window(x, y, x + width - 1, y + height - 1);
	lcd_io_fill_color(color, (uint32_t)width * height);
}
