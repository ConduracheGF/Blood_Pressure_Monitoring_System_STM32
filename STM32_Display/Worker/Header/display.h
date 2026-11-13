/*
 * DISPLAY.h
 *
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_DISPLAY_H_
#define HEADER_DISPLAY_H_

#define DISPLAY_LOGO_TIME_MS 3000
#define DISPLAY_PROJECT_TIME_MS  3000
#define DISPLAY_MEASURE_TIME_MS  10000
#define DISPLAY_INFLATE_PERCENT 40
#define DISPLAY_CUFF_OVERSHOOT 30

typedef enum {
	DISPLAY_LOGO,
	DISPLAY_PROJECT,
	DISPLAY_START,
	DISPLAY_MEASURE,
	DISPLAY_RESULTS,
	DISPLAY_MENU
} DisplayState_t;

void Display_Init();
void Display_Run();

#endif /* HEADER_DISPLAY_H_ */
