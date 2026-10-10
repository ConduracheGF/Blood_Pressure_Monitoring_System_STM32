/*
 * cardSD.c
 *
 *  Created on: 7 Oct 2026
 *      Author: Condurache Gabriel
 */


#include <stdbool.h>
#include "Utils/Header/cardSD.h"
#include "IO/Header/cardSD_io.h"
#include "IO/Header/sys_io.h"

// Comenzi SD pentru SPI
#define SD_CMD_GO_IDLE_STATE 0
#define SD_CMD_SEND_IF_COND 8
#define SD_CMD_SET_BLOCKLEN 16
#define SD_CMD_READ_SINGLE_BLOCK 17
#define SD_CMD_APP_CMD 55
#define SD_CMD_READ_OCR 58
#define SD_ACMD_SD_SEND_OP_COND 41

#define SD_R1_IDLE 0x01
#define SD_R1_READY 0x00
#define SD_R1_NO_RESPONSE 0xFF
#define SD_DATA_TOKEN_START 0xFE

#define SD_INIT_TIMEOUT_MS 1000
#define SD_READ_TOKEN_TIMEOUT_MS 200
#define SD_READY_TIMEOUT_MS 500

static bool card_ready = false;

/*
 * Cardul trebuie sa elibereze MISO
 * Se mai trimite un semnal pentru clock
 */
static void Deselect_Card()
{
	SD_IO_Deselect();
	SD_IO_Transfer_Byte(0xFF);
}

static bool Wait_Ready()
{
	uint32_t start = sys_io_millis();
	while ((sys_io_millis() - start) < SD_READY_TIMEOUT_MS)
	{
		if (SD_IO_Transfer_Byte(0xFF) == 0xFF)
		{
			return true;
		}
	}
	return false;
}

static bool Select_Card()
{
	SD_IO_Select();
	SD_IO_Transfer_Byte(0xFF);
	if (Wait_Ready())
	{
		return true;
	}
	Deselect_Card();
	return false;
}

/*
 * Se trimite o comanda pe 6 bytes
 * Se returneaza raspunsul R1
 * Se mentine cardul selectat
 */
static uint8_t Send_Command(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	Deselect_Card();
	if (!Select_Card())
	{
		return SD_R1_NO_RESPONSE;
	}

	SD_IO_Transfer_Byte(0x40 | cmd);
	SD_IO_Transfer_Byte(arg >> 24);
	SD_IO_Transfer_Byte(arg >> 16);
	SD_IO_Transfer_Byte(arg >> 8);
	SD_IO_Transfer_Byte(arg);
	SD_IO_Transfer_Byte(crc);

	uint8_t response = SD_R1_NO_RESPONSE;
	for (uint8_t count = 0; count < 10 && response == SD_R1_NO_RESPONSE; count++)
	{
		response = SD_IO_Transfer_Byte(0xFF);
	}

	return response;
}

// Raspunsurile R3/R7 au 4 octeti dupa R1
static void Read_Response_Tail(uint8_t *tail)
{
	for (uint8_t index = 0; index < 4; index++)
	{
		tail[index] = SD_IO_Transfer_Byte(0xFF);
	}
}

SdStatus_t SD_Init()
{
	uint8_t _r1;
	uint8_t _tail[4];
	card_ready = false;
	SD_IO_Init();
	SD_IO_Set_Speed(SD_IO_SPEED_INIT);

	/*
	 * CS = HIGH pentru 74 de cicluri
	 * cardul trebuie sa intre in modul de SPI
	 */
	for (uint8_t count = 0; count < 10; count++)
	{
		SD_IO_Transfer_Byte(0xFF);
	}

	/*
	 * Comanda de reset
	 * Cardul ramane in IDLE
	 */
	_r1 = SD_R1_NO_RESPONSE;
	for (uint8_t attempt = 0; attempt < 5 && _r1 != SD_R1_IDLE; attempt++)
	{
		_r1 = Send_Command(SD_CMD_GO_IDLE_STATE, 0, 0x95);
	}
	if(_r1 != SD_R1_IDLE)
	{
		Deselect_Card();
		return SD_ERROR_NO_CARD;
	}

	/*
	 * Comanda de verificare a tensiunii
	 * Cardul SDHC va raspunde cu IDLE si se va repeta 0x000001AA
	 * Orice alt raspuns va spune ca este un card fara suport
	 */
	_r1 = Send_Command(SD_CMD_SEND_IF_COND, 0x000001AA, 0x87);
	if (_r1 != SD_R1_IDLE)
	{
		Deselect_Card();
		return SD_ERROR_INIT;
	}

	Read_Response_Tail(_tail);
	if (_tail[2] != 0x01 || _tail[3] != 0xAA)
	{
		Deselect_Card();
		return SD_ERROR_INIT;
	}

	/*
	 * Comanda de initializare pentru SDHC
	 */
	bool _within_time;
	uint32_t _start = sys_io_millis();

	do
	{
		Send_Command(SD_CMD_APP_CMD, 0, 0x01);
		_r1 = Send_Command(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0x01);
		_within_time = ((sys_io_millis() - _start) < SD_INIT_TIMEOUT_MS);
	} while (_r1 != SD_R1_READY && _within_time);

	if (_r1 != SD_R1_READY)
	{
		Deselect_Card();
		return SD_ERROR_INIT;
	}

	/*
	 * Se citeste bitul CCS din OCR
	 * Se verifica functia de SDHC a cardului
	 */

	if (Send_Command(SD_CMD_READ_OCR, 0, 0x01) != SD_R1_READY)
	{
		Deselect_Card();
		return SD_ERROR_INIT;
	}

	Read_Response_Tail(_tail);
	if (!(_tail[0] & 0x40))
	{
		Deselect_Card();
		return SD_ERROR_INIT;
	}

	Deselect_Card();
	SD_IO_Set_Speed(SD_IO_SPEED_FAST);
	card_ready = true;
	return SD_NORMAL;
}

// Citire de blocuri pe 512 octeti
SdStatus_t SD_Read_Block(uint32_t block, uint8_t *buffer)
{
	if (!card_ready)
	{
		return SD_ERROR_NO_CARD;
	}

	if (Send_Command(SD_CMD_READ_SINGLE_BLOCK, block, 0x01) != SD_R1_READY)
	{
		Deselect_Card();
		return SD_ERROR_READ;
	}

	uint8_t _token = 0xFF;
	uint32_t _start = sys_io_millis();

	while (_token == 0xFF && (sys_io_millis() - _start) < SD_READ_TOKEN_TIMEOUT_MS)
	{
		_token = SD_IO_Transfer_Byte(0xFF);
	}

	while (_token != SD_DATA_TOKEN_START)
	{
		Deselect_Card();
		return SD_ERROR_READ;
	}

	SD_IO_Receive_Bytes(buffer, SD_BLOCK_SIZE);
	SD_IO_Transfer_Byte(0xFF);
	SD_IO_Transfer_Byte(0xFF);
	Deselect_Card();

	return SD_NORMAL;
}
