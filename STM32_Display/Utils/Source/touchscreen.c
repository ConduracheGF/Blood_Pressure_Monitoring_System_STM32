/*
 * touchscreen.c
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */
#include "IO/Header/touchscreen_io.h"
#include "Utils/Header/touchscreen.h"
#include "Utils/Header/lcd.h"

static int32_t Map(int32_t raw, int32_t raw_a, int32_t raw_b, int32_t screen_a, int32_t screen_b)
{
	return screen_a + (raw - raw_a) * (screen_b - screen_a) / (raw_b - raw_a);
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
	if (Touch_IO_Read_Pressure() < TOUCH_PRESSURE_THRESHOLD)
	{
		return false;
	}

	int32_t _raw_x = Touch_IO_Read_X();
	int32_t _raw_y = Touch_IO_Read_Y();

	if (Touch_IO_Read_Pressure() < TOUCH_PRESSURE_THRESHOLD)
	{
		return false;
	}

	int32_t _x = Map(_raw_x, TOUCH_CAL_RAW_X_LEFT, TOUCH_CAL_RAW_X_RIGHT, TOUCH_CAL_SCREEN_X_LEFT, TOUCH_CAL_SCREEN_X_RIGHT);
	int32_t _y = Map(_raw_y, TOUCH_CAL_RAW_Y_TOP, TOUCH_CAL_RAW_Y_BOTTOM, TOUCH_CAL_SCREEN_Y_TOP, TOUCH_CAL_SCREEN_Y_BOTTOM);

	point->x = Clamp(_x, LCD_WIDTH - 1);
	point->y = Clamp(_y, LCD_HEIGHT - 1);
	return true;
}
