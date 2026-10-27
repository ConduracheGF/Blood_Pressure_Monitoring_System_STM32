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

// Ecrane din poze
void Screen_Logo_Draw();
void Screen_Project_Draw();
void Screen_Start_Draw();
void Screen_Menu_Draw();
void Screen_Measure_Draw();
void Screen_Results_Draw(uint16_t sys, uint16_t dia, uint16_t bpm, uint8_t riskPercent, ScreenRisk_t risk);

// Masurare cu bara colorata si presiunea din manseta
void Screen_Measure_Begin();
void Screen_Measure_Update(uint8_t progressPercent, uint16_t pressure);

// Zonele de atingere
bool Screen_Start_Pressed();
bool Screen_Restart_Pressed();
bool Screen_Back_Pressed();
bool Screen_View_Pressed();

#endif /* HEADER_SCREENS_H_ */
