/*
 * app.h
 *
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_APP_H_
#define HEADER_APP_H_

#define APP_MAX_PHOTOS 8
#define APP_PHOTO_TIME_MS 2000
#define APP_SIMULATION_SECONDS 5

typedef enum {
	APP_PHOTOS,
	APP_START,
	APP_SIMULATION,
	APP_RESULTS
} AppState_t;

void APP_Init();
void APP_Run();

#endif /* HEADER_APP_H_ */
