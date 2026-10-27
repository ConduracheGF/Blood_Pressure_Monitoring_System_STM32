/*
 * app.h
 *
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_APP_H_
#define HEADER_APP_H_

#define APP_LOGO_TIME_MS 3000
#define APP_PROJECT_TIME_MS  3000
#define APP_MEASURE_TIME_MS  10000
#define APP_INFLATE_PERCENT 40
#define APP_CUFF_OVERSHOOT 30

typedef enum {
	APP_LOGO,
	APP_PROJECT,
	APP_START,
	APP_MEASURE,
	APP_RESULTS,
	APP_MENU
} AppState_t;

void APP_Init();
void APP_Run();

#endif /* HEADER_APP_H_ */
