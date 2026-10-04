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

void screen_start_draw();
void screen_simulation_draw();
void screen_simulation_update(uint8_t second_lef);
void screen_results_draw(uint16_t sys, uint16_t dia, uint16_t bpm, uint8_t risk_percent, bool hypertension);

#endif /* HEADER_SCREENS_H_ */
