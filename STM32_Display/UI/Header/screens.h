/*
 * screens.h
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_SCREENS_H_
#define HEADER_SCREENS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	SCREEN_RISK_NORMAL,
	SCREEN_RISK_HYPER,
	SCREEN_RISK_HYPO
} ScreenRisk_t;

void screen_start_draw();
void screen_simulation_draw();
void screen_simulation_update(uint8_t second_lef);
void screen_results_draw(uint16_t sys, uint16_t dia, uint16_t bpm, uint8_t risk_percent, ScreenRisk_t risk);
bool screen_start_button_pressed();
bool screen_restart_button_pressed();


#endif /* HEADER_SCREENS_H_ */
