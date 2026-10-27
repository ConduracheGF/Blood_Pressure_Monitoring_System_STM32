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
#define BMP_MAX_BYTES_PER_PIXEL 4

static uint8_t rowBuffer[LCD_WIDTH * BMP_MAX_BYTES_PER_PIXEL]; // Randul din fisier contine B,G,R
static uint16_t pixelBuffer[LCD_WIDTH];

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
	uint16_t _bytesRead;

	if (FS_Open(name) != FS_NORMAL){
		return BMP_ERROR_OPEN;
	}

	if (FS_Read(_header, BMP_HEADER_SIZE, &_bytesRead) != FS_NORMAL || _bytesRead != BMP_HEADER_SIZE) {
		FS_Close();
		return BMP_ERROR_READ;
	}

	uint32_t _dataOffset = Read32(&_header[10]);
	int32_t _width = (int32_t) Read32(&_header[18]);
	int32_t _height = (int32_t) Read32(&_header[22]);
	uint16_t _bitiPerPixel = Read16(&_header[28]);
	uint32_t _compression = Read32(&_header[30]);

	bool _bottomUp = (_height > 0);

	if (_height < 0) {
		_height = -_height;
	}

	bool _supportedBytes = (_bitiPerPixel == 24 && _compression == 0) ||
							(_bitiPerPixel == 32 && (_compression == 0 || _compression == 3));

	if (Read16(&_header[0]) != BMP_SIGNATURE || !_supportedBytes ||  _width <= 0 || _height == 0) {
		FS_Close();
		return BMP_ERROR_FORMAT;
	}

	if (x + _width > LCD_WIDTH || y + _height > LCD_HEIGHT) {
		FS_Close();
		return BMP_ERROR_SIZE;
	}

	int32_t _bytesPerPixel = _bitiPerPixel / 8;
	uint32_t _rowSize = (uint32_t)((_width * _bytesPerPixel + 3) & ~3);

	if (FS_Seek(_dataOffset) != FS_NORMAL) {
		FS_Close();
		return BMP_ERROR_READ;
	}

	for (int32_t row = 0; row < _height; row++){
		if (FS_Read(rowBuffer, _rowSize, &_bytesRead) != FS_NORMAL || _bytesRead < (_width * _bytesPerPixel)) {
			FS_Close();
			return BMP_ERROR_READ;
		}

		// Conversie RGB565
		for (int32_t column = 0; column < _width; column++) {
			uint8_t _blue = rowBuffer[column * _bytesPerPixel];
			uint8_t _green = rowBuffer[column * _bytesPerPixel + 1];
			uint8_t _red = rowBuffer[column * _bytesPerPixel + 2];
			pixelBuffer[column] = ((_red & 0x0F8) << 8) | ((_green & 0xFC) << 3) | (_blue >> 3);
		}

		uint16_t _screenRow = _bottomUp ? (y + _height - 1 - row) : (y + row);
		LCD_Set_Window(x, _screenRow, x + _width - 1, _screenRow);
		LCD_Write_Pixels(pixelBuffer, _width);
	}

	FS_Close();
	return BMP_NORMAL;
}
