/*
 * cardSD_io.c
 *
 *  Created on: 6 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "main.h"
#include <string.h>
#include "IO/Header/cardSD_io.h"

#define SPI_TIMEOUT_MS 100

extern SPI_HandleTypeDef hspi1;

void SD_IO_Init()
{
	SD_IO_Deselect();
}

void SD_IO_Select()
{
	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
}

void SD_IO_Deselect()
{
	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}

// Se trimite octetul pe SPI si returneaza octetul primit
uint8_t SD_IO_Transfer_Byte(uint8_t byte)
{
	uint8_t received = 0xFF;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &received, 1, SPI_TIMEOUT_MS);
	return received;
}

/*
 * Se citesc length octeti
 * Pe toata durata citirii: MOSI = 0xFF
 * Se umple buffer-ul cu 0xFF
 * Bufferul va fi folosit si pentru receive si pentru transmit
 * */
void SD_IO_Receive_Bytes(uint8_t *buffer, uint16_t length)
{
	memset(buffer, 0xFF, length);
	HAL_SPI_TransmitReceive(&hspi1, buffer, buffer, length, SPI_TIMEOUT_MS);
}

/*
 * Se seteaza frecventa de clock a hardware-ului
 * Se dezactiveaza SPI-ul ca sa putem modifica registrii protejati
 * Acum se poate adauga noua configuratie spi si se initializeaza SPI-ul dupa noua configuratie
 * */
void SD_IO_Set_Speed(SdIoSpeed_t speed)
{
	if (speed == SD_IO_SPEED_FAST)
	{
		hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4; // SPI_BAUDRATEPRESCALER_8
	}
	else
	{
		hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	}

	HAL_SPI_DeInit(&hspi1);
	HAL_SPI_Init(&hspi1);
}
