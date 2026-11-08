/*
 * touchscreen.h
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_TOUCHSCREEN_H_
#define HEADER_TOUCHSCREEN_H_

#include <stdint.h>
#include <stdbool.h>

#define TOUCH_PRESSURE_THRESHOLD 100

#define TOUCH_CAL_RAW_X_LEFT 3304
#define TOUCH_CAL_RAW_X_RIGHT 795
#define TOUCH_CAL_RAW_Y_TOP 528
#define TOUCH_CAL_RAW_Y_BOTTOM 3484

#define TOUCH_CAL_SCREEN_X_LEFT 30
#define TOUCH_CAL_SCREEN_X_RIGHT 289
#define TOUCH_CAL_SCREEN_Y_TOP 30
#define TOUCH_CAL_SCREEN_Y_BOTTOM 449

typedef struct {
	uint16_t x;
	uint16_t y;
} TouchPoint_t;

/*
 * Pentru schimbarile dintre ecrane
 * Arunca punctul retinut
 */
void Touch_Flush();
// Se verifica daca ecranul este apasat pe coordonatele corespunzatoare
bool Touch_Get_Point(TouchPoint_t *point);

#endif /* HEADER_TOUCHSCREEN_H_ */
