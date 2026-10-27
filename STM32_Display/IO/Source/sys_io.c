/*
 * sys_io.c
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "main.h"
#include "IO/Header/sys_io.h"

uint32_t SYS_IO_Millis()
{
	return HAL_GetTick();
}

void SYS_IO_Delay_MS(uint32_t ms)
{
	HAL_Delay(ms);
}
