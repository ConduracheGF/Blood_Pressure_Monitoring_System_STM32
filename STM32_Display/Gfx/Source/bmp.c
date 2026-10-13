/*
 * bmp.c
 *	Structura BMP:
 *		ANTET (14 biti - fisier) + IMAGINE (40 octeti)
 *			octetul 0-1 = semnatura BM
 *			octetul 10 = locul de inceput a pixelilor in fisier
 *			octetul 18 = latime
 *			octetul 22 = inaltime
 *						 inaltime pozitiva = randurile sunt de jos in sus
 *			octetul 28 = biti pe pixel
 *			octetul 30 = compresie
 *						 valoarea 0 => necomprimat
 *		Fiecare rand e completat cu 0 pana la multiplu de 4 octeti
 *
 *  Created on: 13 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <stdbool.h>
#include "Gfx/Header/bmp.h"
#include "Utils/Header/lcd.h"
#include "Utils/Header/cardFS.h"

#define BMP_HEADER_SIZE 54
#define BMP_SIGNATURE 0x4D42
#define BMP_BYTES_PER_PIXEL 3

static uint8_t row_buffer[LCD_WIDTH * BMP_BYTES_PER_PIXEL]; // Randul din fisier contine B,G,R
static uint16_t pixel_buffer[LCD_WIDTH];

static uint16_t Read16(const uint8_t *data)
{
	return data[0] | (data[1] << 8);
}

static uint32_t Read32(const uint8_t *data)
{
	return data[0] | (data[1] << 8) | (data[2] << 16) | ((uint32_t) data[3] << 24);
}

BmpStatus_t BMP_Draw_File(const char *name, uint16_t x, uint16_t y)
{
	uint8_t _header[BMP_HEADER_SIZE];
	uint16_t _bytes_read;

	if (FS_Open(name) != FS_NORMAL){
		return BMP_ERROR_OPEN;
	}

	if (FS_Read(_header, BMP_HEADER_SIZE, &_bytes_read) != FS_NORMAL || _bytes_read != BMP_HEADER_SIZE) {
		FS_Close();
		return BMP_ERROR_READ;
	}

	uint32_t _data_offset = Read32(&_header[10]);
	int32_t _width = (int32_t) Read32(&_header[18]);
	int32_t _height = (int32_t) Read32(&_header[22]);
	uint16_t _biti_per_pixel = Read16(&_header[28]);
	uint32_t _compression = Read32(&_header[30]);

	bool _bottom_up = (_height > 0);

	if (_height < 0) {
		_height = -_height;
	}

	if (Read16(&_header[0]) != BMP_SIGNATURE || _biti_per_pixel != 24 || _compression != 0 || _width <= 0 || _height == 0) {
		FS_Close();
		return BMP_ERROR_FORMAT;
	}

	if (x + _width > LCD_WIDTH || y + _height > LCD_HEIGHT) {
		FS_Close();
		return BMP_ERROR_SIZE;
	}

	uint32_t _row_size = ((_width * BMP_BYTES_PER_PIXEL) + 3) & ~3u;

	if (FS_Seek(_data_offset) != FS_NORMAL) {
		FS_Close();
		return BMP_ERROR_READ;
	}

	for (int32_t row = 0; row < _height; row++){
		if (FS_Read(row_buffer, _row_size, &_bytes_read) != FS_NORMAL || _bytes_read < (_width * BMP_BYTES_PER_PIXEL)) {
			FS_Close();
			return BMP_ERROR_READ;
		}

		// Conversie RGB565
		for (int32_t column = 0; column < _width; column++) {
			uint8_t _blue = row_buffer[column * BMP_BYTES_PER_PIXEL];
			uint8_t _green = row_buffer[column * BMP_BYTES_PER_PIXEL + 1];
			uint8_t _red = row_buffer[column * BMP_BYTES_PER_PIXEL + 2];
			pixel_buffer[column] = ((_red & 0x0F8) << 8) | ((_green & 0xFC) << 3) | (_blue >> 3);
		}

		uint16_t _screen_row = _bottom_up ? (y + _height - 1 - row) : (y + row);
		lcd_set_window(x, _screen_row, x + _width - 1, _screen_row);
		lcd_write_pixels(pixel_buffer, _width);
	}

	FS_Close();
	return BMP_NORMAL;
}
