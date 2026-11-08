/*
 * touchscreen.c
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */
#include "IO/Header/touchscreen_io.h"
#include "Utils/Header/touchscreen.h"
#include "Utils/Header/lcd.h"

static TouchPoint_t lastPoint;
static bool lastPointValid = false;

static int32_t Map(int32_t raw, int32_t rawA, int32_t rawB, int32_t screenA, int32_t screenB)
{
	return screenA + (raw - rawA) * (screenB - screenA) / (rawB - rawA);
}

static int32_t Clamp(int32_t value, int32_t maxim)
{
	if (value < 0)
	{
		return 0;
	}
	if (value > maxim)
	{
		return maxim;
	}
	return value;
}

bool Touch_Get_Point(TouchPoint_t *point)
{
	if (touchState == TOUCH_STATE_DATA_READY)
	{
		lastPointValid = false;

		if (Touch_IO_Read_Pressure() >= TOUCH_PRESSURE_THRESHOLD) {
			int32_t _rawX = Touch_IO_Read_X();
			int32_t _rawY = Touch_IO_Read_Y();

			int32_t _x = Map(_rawX, TOUCH_CAL_RAW_X_LEFT, TOUCH_CAL_RAW_X_RIGHT, TOUCH_CAL_SCREEN_X_LEFT, TOUCH_CAL_SCREEN_X_RIGHT);
			int32_t _y = Map(_rawY, TOUCH_CAL_RAW_Y_TOP, TOUCH_CAL_RAW_Y_BOTTOM, TOUCH_CAL_SCREEN_Y_TOP, TOUCH_CAL_SCREEN_Y_BOTTOM);

			lastPoint.x = Clamp(_x, LCD_WIDTH - 1);
			lastPoint.y = Clamp(_y, LCD_HEIGHT - 1);
			lastPointValid = true;
		}
		touchState = TOUCH_STATE_IDLE;
	}

	if (lastPointValid)
	{
		*point = lastPoint;
	}

	return lastPointValid;
}

void Touch_Flush() {
	lastPointValid = false;
	if (touchState == TOUCH_STATE_DATA_READY) {
		touchState = TOUCH_STATE_IDLE;
	}
}
