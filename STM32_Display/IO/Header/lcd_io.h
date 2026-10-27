/*
 * lcd_io.h
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_LCD_IO_H_
#define HEADER_LCD_IO_H_

#include <stdint.h>

void LCD_IO_Init();
void LCD_IO_Reset();
void LCD_IO_Write_Data(uint8_t data);
void LCD_IO_Write_Command(uint8_t cmd);
void LCD_IO_Write_Pixels(const uint16_t *pixels, uint32_t nrOfPixels);
void LCD_IO_Fill_Color(uint16_t color, uint32_t nrOfPixels);
uint16_t LCD_IO_Test_Data_Bus();

#endif /* HEADER_LCD_IO_H_ */
