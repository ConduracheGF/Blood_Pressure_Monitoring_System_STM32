/*
 * sys_io.c
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "main.h"
#include "IO/Header/sys_io.h"

volatile bool flagFor1ms = false;
volatile bool flagFor10ms = false;
volatile bool flagFor100ms = false;

void HAL_SYSTICK_Callback()
{
	static uint8_t _count10ms = 0;
	static uint8_t _count100ms = 0;

	flagFor1ms = true;

	if (++_count10ms >= 10) {
		flagFor10ms = true;
		_count10ms = 0;
	}

	if (++_count100ms >= 100) {
		flagFor100ms = true;
		_count100ms = 0;
	}
}

uint32_t SYS_IO_Millis()
{
	return HAL_GetTick();
}

void SYS_IO_Delay_MS(uint32_t ms)
{
	HAL_Delay(ms);
}
