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
#define TOUCH_STATE_DATA_READY_FLAG 7

typedef enum {
	TOUCH_STATE_IDLE,
	TOUCH_STATE_PREP_X,
	TOUCH_STATE_WAIT_X,
	TOUCH_STATE_PREP_Y,
	TOUCH_STATE_WAIT_Y,
	TOUCH_STATE_PREP_Z,
	TOUCH_STATE_WAIT_Z,
	TOUCH_STATE_DATA_READY
} TouchState_t;

extern volatile TouchState_t touchState;

uint16_t Touch_IO_Read_X();
uint16_t Touch_IO_Read_Y();
uint16_t Touch_IO_Read_Pressure();
void Touch_IO_Process();

#endif /* HEADER_TOUCHSCREEN_IO_H_ */
