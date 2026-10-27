/*
 * gfx.c
 *
 *  Created on: 3 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <string.h>
#include <stdbool.h>
#include "Gfx/Header/gfx.h"
#include "Gfx/Header/font.h"
#include "Utils/Header/lcd.h"

static uint16_t cellBuffer[(FONT_CELL_WIDTH * GFX_MAX_SCALE) * (FONT_CELL_HEIGHT * GFX_MAX_SCALE)];

void GFX_Clear(uint16_t color)
{
	LCD_Fill_Rect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

// Contur dreptunghiular, grosimea se masoara spre interior
void GFX_Draw_Border(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color)
{
	if (thickness == 0 || width < 2 * thickness || height < 2 * thickness)
	{
		return;
	}

	LCD_Fill_Rect(x, y, width, thickness, color); // sus
	LCD_Fill_Rect(x, y + height - thickness, width, thickness, color); // jos
	LCD_Fill_Rect(x, y, thickness, height, color); // stanga
	LCD_Fill_Rect(x + width - thickness, y, thickness, height, color); // dreapta
}

void GFX_Draw_Char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bgColor, uint8_t scale)
{
	if (scale == 0 || scale > GFX_MAX_SCALE)
	{
		return;
	}

	if (c >= 'a' && c <= 'z')
	{
		c -= 32; // Mentinem totul pe MAJUSCULE
	}

	if (c < FONT_FIRST_CHAR  || c > FONT_LAST_CHAR)
	{
		c = '?';
	}

	const uint8_t *_glyph = font[c - FONT_FIRST_CHAR];
	uint16_t _cellWidth = FONT_CELL_WIDTH * scale;
	uint16_t _cellHeight = FONT_CELL_HEIGHT * scale;

	for (uint16_t row = 0; row < _cellHeight; row++)
	{
		for (uint16_t col = 0; col < _cellWidth; col++)
		{
			uint16_t _fontCol = col / scale; // coloana 6 din font devine spatiul de distantiere a literelor
			uint8_t _fontRow = row / scale; // randul 8 din font devine spatiul de sub litera
			bool _pixelOn = (_fontCol < FONT_WIDTH) && (_fontRow < FONT_HEIGHT) && (_glyph[_fontCol] & (1 << _fontRow));
			cellBuffer[row * _cellWidth + col] = _pixelOn ? color : bgColor;
		}
	}

	if (LCD_Set_Window(x, y, x + _cellWidth - 1, y + _cellHeight - 1))
	{
		LCD_Write_Pixels(cellBuffer, (uint32_t)_cellWidth * _cellHeight);
	}
}

void GFX_Draw_Text(uint16_t x, uint16_t y, const char *text, uint16_t color, uint16_t bg_color, uint8_t scale)
{
	while(*text)
	{
		GFX_Draw_Char(x, y, *text++, color , bg_color, scale);
		x += FONT_CELL_WIDTH * scale;
	}
}

uint16_t GFX_Text_Width(const char *text, uint8_t scale)
{
	return strlen(text) * FONT_CELL_WIDTH * scale;
}

uint16_t GFX_Text_Height(uint8_t scale)
{
	return FONT_HEIGHT * scale;
}

void GFX_Draw_Text_Overlay(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t scale)
{
	while (*text)
	{
		char _caracter = *text++;
		if (_caracter >= 'a' && _caracter <= 'z')
		{
			_caracter -= 32;
		}
		if (_caracter < FONT_FIRST_CHAR || _caracter > FONT_LAST_CHAR)
		{
			_caracter = '?';
		}

		const uint8_t *_glyph = font[_caracter - FONT_FIRST_CHAR];
		for (uint8_t col = 0; col < FONT_WIDTH; col++)
		{
			for (uint8_t row = 0; row < FONT_HEIGHT; row++)
			{
				if (_glyph[col] & (1 << row))
				{
					LCD_Fill_Rect(x + col * scale, y + row * scale, scale, scale, color);
				}
			}
		}
		x += FONT_CELL_WIDTH * scale;
	}
}
