/*
 * layout.h
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_LAYOUT_H_
#define HEADER_LAYOUT_H_

#include "Utils/Header/lcd.h"

// Culori de layout
#define UI_COLOR_BACKGROUND CULOARE_NEGRU
#define UI_COLOR_TEXT CULOARE_ALB
#define UI_COLOR_BUTTON CULOARE_VERDE
#define UI_COLOR_VALUE CULOARE_VERDE
#define UI_COLOR_RISK CULOARE_ROSU

// Definire spatiu de lucru
#define LAYOUT_SCREEN {0, 0, LCD_WIDTH, LCD_HEIGHT}

// Ecran START
#define LAYOUT_START_TITLE_Y 120
#define LAYOUT_START_BUTTON {60, 260, 200, 80}

// Ecran Simulare
#define LAYOUT_SIMULATION_TITLE_Y 120
#define LAYOUT_SIMULATION_COUNTER_Y 220

// Ecran de rezultate
#define LAYOUT_RESULTS_TITLE_Y 25
#define LAYOUT_RESULTS_SYS_BOX {10, 80, 140, 140}
#define LAYOUT_RESULTS_DIA_BOX {170, 80, 140, 140}
#define LAYOUT_RESULTS_BPM_BOX {10, 240, 140, 140}
#define LAYOUT_RESULTS_RISK_BOX {170, 240, 140, 140}
#define LAYOUT_RESULTS_RESTART_BUTTON {60, 400, 200, 60}

#endif /* HEADER_LAYOUT_H_ */
