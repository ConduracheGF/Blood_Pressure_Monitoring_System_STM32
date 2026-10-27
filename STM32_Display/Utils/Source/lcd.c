/*
 * lcd.c
 *	Fisier ce contine corpurile de definitie ale functionalitatilor de baza ale display-ului.
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "Utils/Header/lcd.h"
#include "IO/Header/sys_io.h"
#include "IO/Header/lcd_io.h"

static void Write_Command(uint8_t cmd, const uint8_t *parameters, uint8_t nrOfParameters)
{
	LCD_IO_Write_Command(cmd);
	for (uint8_t _index = 0; _index < nrOfParameters; _index++)
	{
		LCD_IO_Write_Data(parameters[_index]);
	}
}

void LCD_Init()
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

	LCD_IO_Init();
	LCD_IO_Reset();

	LCD_IO_Write_Command(CMD_SLEEP_OUT);
	SYS_IO_Delay_MS(120);

	Write_Command(CMD_PIXEL_FORMAT_16BIT, (const uint8_t[]){PIXEL_FORMAT_16BIT}, 1);
	Write_Command(CMD_POWER_CTRL_3, (const uint8_t[]){0x44}, 1);
	Write_Command(CMD_GAMMA_POSITIVE, gamma_positive, 15);
	Write_Command(CMD_GAMMA_NEGATIVE, gamma_negative, 15);
	Write_Command(CMD_GAMMA_DIGITAL, gamma_negative, 15);
	Write_Command(CMD_MEMORY_ACCESS, (const uint8_t[]){MEMORY_ACCESS_PORTRAIT_BGR}, 1);

	LCD_IO_Write_Command(CMD_DISPLAY_ON);
	SYS_IO_Delay_MS(25);
}

/*
 *  Scrie in memorie in zona selectata de display din RAM
 *  Daca fereastra este invalida sau este afara din ecran
 *  => false = send nothing
*/
bool LCD_Set_Window(uint16_t xStart, uint16_t yStart, uint16_t xFinish, uint16_t yFinish)
{
	if (xStart > xFinish || yStart > yFinish)
	{
		return false;
	} else if (xFinish >= LCD_WIDTH || yFinish >= LCD_HEIGHT){
		return false;
	}

	LCD_IO_Write_Command(CMD_COLUMN_ADDR_SET);
	LCD_IO_Write_Data(xStart >> 8);
	LCD_IO_Write_Data(xStart & 0xFF);
	LCD_IO_Write_Data(xFinish >> 8);
	LCD_IO_Write_Data(xFinish & 0xFF);

	LCD_IO_Write_Command(CMD_ROW_ADDR_SET);
	LCD_IO_Write_Data(yStart >> 8);
	LCD_IO_Write_Data(yStart & 0xFF);
	LCD_IO_Write_Data(yFinish >> 8);
	LCD_IO_Write_Data(yFinish & 0xFF);

	LCD_IO_Write_Command(CMD_MEMORY_WRITE);
	return true;
}

void LCD_Write_Pixels(const uint16_t *pixels, uint32_t nrOfPixels)
{
	LCD_IO_Write_Pixels(pixels, nrOfPixels);
}

void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
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

	LCD_Set_Window(x, y, x + width - 1, y + height - 1);
	LCD_IO_Fill_Color(color, (uint32_t)width * height);
}
