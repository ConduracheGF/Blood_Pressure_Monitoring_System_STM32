/*
 * touchscree_io.h
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_TOUCHSCREEN_IO_H_
#define HEADER_TOUCHSCREEN_IO_H_

#include <stdint.h>
#include <stdbool.h>

#define TOUCH_IO_ADC_MAX 4095

uint16_t Touch_IO_Read_X();
uint16_t Touch_IO_Read_Y();
uint16_t Touch_IO_Read_Pressure();

#endif /* HEADER_TOUCHSCREEN_IO_H_ */
