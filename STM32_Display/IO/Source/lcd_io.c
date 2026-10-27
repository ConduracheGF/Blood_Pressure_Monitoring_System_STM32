/*
 * lcd_io.c
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "main.h"
#include "IO/Header/lcd_io.h"

/*
 * Control Signals:
 * 		RD = Read Strobe
 * 		WR = Write Strobe
 * 		RS = Register Select
 * 			low = command
 * 			high = data
 * 		CS = Chip Select
 * 			low = lcd-ul asculta buss-ul
 * 		RST = Reset
 * 			low = activ
*/
#define RD_HIGH() HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET)
#define WR_LOW() HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)
#define WR_HIGH() HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define RS_LOW() HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)
#define RS_HIGH() HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define CS_LOW() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define RST_LOW() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define RST_HIGH() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

static const uint16_t dataPins[8] = {
	LCD_D0_Pin,
	LCD_D1_Pin,
	LCD_D2_Pin,
	LCD_D3_Pin,
	LCD_D4_Pin,
	LCD_D5_Pin,
	LCD_D6_Pin,
	LCD_D7_Pin
};

static GPIO_TypeDef * const dataPorts[8] = {
	LCD_D0_GPIO_Port,
	LCD_D1_GPIO_Port,
	LCD_D2_GPIO_Port,
	LCD_D3_GPIO_Port,
	LCD_D4_GPIO_Port,
	LCD_D5_GPIO_Port,
	LCD_D6_GPIO_Port,
	LCD_D7_GPIO_Port
};

// Serial Paralel 8 bit writing protocol dedicat pentru modulul de display folosit
// Scriem bit-cu-bit cate un octet in data si il trimitem prin semnalul de WR
static void Bus_Write_Byte(uint8_t octet)
{
	for (uint8_t bit = 0; bit < 8; bit++)
	{
		// Pregatim pe rand fiecare pin corespunzator
		GPIO_PinState bitSetat = (octet & (1 << bit)) ? GPIO_PIN_SET: GPIO_PIN_RESET;
		HAL_GPIO_WritePin(dataPorts[bit], dataPins[bit], bitSetat);
	}
	WR_LOW();
	WR_HIGH();
}

void LCD_IO_Init()
{
	RD_HIGH();
	WR_HIGH();
	CS_HIGH();
	RS_HIGH();
	RST_HIGH();
}

void LCD_IO_Reset()
{
	RST_HIGH(); HAL_Delay(5);
	RST_LOW(); HAL_Delay(20);
	RST_HIGH(); HAL_Delay(150);
}

void LCD_IO_Write_Command(uint8_t cmd)
{
	CS_LOW();
	RS_LOW();
	Bus_Write_Byte(cmd);
	CS_HIGH();
}

void LCD_IO_Write_Data(uint8_t data)
{
	CS_LOW();
	RS_HIGH();
	Bus_Write_Byte(data);
	CS_HIGH();
}

void LCD_IO_Write_Pixels(const uint16_t *pixels, uint32_t nrOfPixels)
{
	CS_LOW();
	RS_HIGH();
	while (nrOfPixels--)
	{
		uint16_t pixel = *pixels++;
		Bus_Write_Byte(pixel >> 8);
		Bus_Write_Byte(pixel & 0xFF);
	}
	CS_HIGH();
}

void LCD_IO_Fill_Color(uint16_t color, uint32_t nrOfPixels)
{
	CS_LOW();
	RS_HIGH();
	while(nrOfPixels--)
	{
		Bus_Write_Byte(color >> 8);
		Bus_Write_Byte(color & 0xFF);
	}
	CS_HIGH();
}

// Test pentru toate valorile posibile: se trimite pe rand valoarea pe bus, urmand fie validata asemanarea
uint16_t LCD_IO_Test_Data_Bus()
{
	uint16_t errors = 0;
	for (uint16_t valoareTest = 0; valoareTest < 256; valoareTest++)
	{
		Bus_Write_Byte(valoareTest);
		uint8_t readBack = 0;
		for (int bit = 0; bit < 8; bit++)
		{
			if (HAL_GPIO_ReadPin(dataPorts[bit], dataPins[bit]) == GPIO_PIN_SET)
			{
				readBack |= (1 << bit);
			}
		}

		if (readBack != valoareTest)
		{
			errors++;
		}
	}
	return errors;
}
