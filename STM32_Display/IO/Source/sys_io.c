/*
 * sys_io.c
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "main.h"
#include "IO/Header/sys_io.h"

uint32_t sys_io_millis()
{
	return HAL_GetTick();
}

void sys_io_delay_ms(uint32_t ms)
{
	HAL_Delay(ms);
}
