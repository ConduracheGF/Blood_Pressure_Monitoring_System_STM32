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

static uint16_t cell_buffer[(FONT_CELL_WIDTH * GFX_MAX_SCALE) * (FONT_CELL_HEIGHT * GFX_MAX_SCALE)];

void gfx_clear(uint16_t color)
{
	lcd_fill_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

// Contur dreptunghiular, grosimea se masoara spre interior
void gfx_draw_border(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color)
{
	if (thickness == 0 || width < 2 * thickness || height < 2 * thickness)
	{
		return;
	}

	lcd_fill_rect(x, y, width, thickness, color); // sus
	lcd_fill_rect(x, y + height - thickness, width, thickness, color); // jos
	lcd_fill_rect(x, y, thickness, height, color); // stanga
	lcd_fill_rect(x + width - thickness, y, thickness, height, color); // dreapta
}

void gfx_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t scale)
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

	const uint8_t *glyph = font_5x7[c - FONT_FIRST_CHAR];
	uint16_t cellWidth = FONT_CELL_WIDTH * scale;
	uint16_t cellHeight = FONT_CELL_HEIGHT * scale;

	for (uint16_t row = 0; row < cellHeight; row++)
	{
		for (uint16_t col = 0; col < cellWidth; col++)
		{
			uint16_t fontCol = col / scale; // coloana 6 din font devine spatiul de distantiere a literelor
			uint8_t fontRow = row / scale; // randul 8 din font devine spatiul de sub litera
			bool pixelOn = (fontCol < FONT_WIDTH) && (fontRow < FONT_HEIGHT) && (glyph[fontCol] & (1 << fontRow));
			cell_buffer[row * cellWidth + col] = pixelOn ? color : bg_color;
		}
	}

	if (lcd_set_window(x, y, x + cellWidth - 1, y + cellHeight - 1))
	{
		lcd_write_pixels(cell_buffer, (uint32_t)cellWidth * cellHeight);
	}
}

void gfx_draw_text(uint16_t x, uint16_t y, const char *text, uint16_t color, uint16_t bg_color, uint8_t scale)
{
	while(*text)
	{
		gfx_draw_char(x, y, *text++, color , bg_color, scale);
		x += FONT_CELL_WIDTH * scale;
	}
}

uint16_t gfx_text_width(const char *text, uint8_t scale)
{
	return strlen(text) * FONT_CELL_WIDTH * scale;
}

uint16_t gfx_text_height(uint8_t scale)
{
	return FONT_HEIGHT * scale;
}
