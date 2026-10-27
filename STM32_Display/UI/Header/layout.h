/*
 * layout.h
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_LAYOUT_H_
#define HEADER_LAYOUT_H_

#include "Utils/Header/lcd.h"

// Poze card memorie
#define IMAGE_LOGO "LOGO.BMP"
#define IMAGE_PROJECT "PROJECT.BMP"
#define IMAGE_START "START.BMP"
#define IMAGE_RESULTS "RESULTS.BMP"
#define IMAGE_MENU "MENU.BMP"
#define IMAGE_MEASURE "MEASURE.BMP"

/*
 * Posibil temporar
 * 1 = se deseneaza peste poze zonele de touch
 * 0 = functionalitate oprita
 */
#define UI_DEBUG_ZONE 1

// Culori de layout
#define UI_COLOR_BACKGROUND CULOARE_NEGRU
#define UI_COLOR_TEXT CULOARE_ALB
#define UI_COLOR_VALUE CULOARE_VERDE
#define UI_COLOR_OVERLAY CULOARE_ALB
#define UI_COLOR_FIELD CULOARE_NEGRU
#define UI_COLOR_BAR_EMPTY CULOARE_GRI
#define UI_COLOR_YELLOW CULOARE_GALBEN

// Zone de atingere {x, y, latime, inaltime}
#define LAYOUT_ZONE_START {50, 170, 220, 120}
#define LAYOUT_ZONE_BACK {20, 205, 280, 70}
#define LAYOUT_ZONE_RESTART {60, 120, 200, 85}
#define LAYOUT_ZONE_VIEW {60, 295, 200, 90}

// Zona cu bara de progres
#define LAYOUT_BAR {20, 250, 280, 30}
#define LAYOUT_PRESSURE_FIELD {60, 220, 200, 28}

// Poza cu rezultate
#define LAYOUT_RESULT_SYS_X 60
#define LAYOUT_RESULT_SYS_X100 45
#define LAYOUT_RESULT_SYS_Y 125
#define LAYOUT_RESULT_DIA_X 215
#define LAYOUT_RESULT_DIA_X100 200
#define LAYOUT_RESULT_DIA_Y 125
#define LAYOUT_RESULT_BPM_X 60
#define LAYOUT_RESULT_BPM_X100 45
#define LAYOUT_RESULT_BPM_Y 380
#define LAYOUT_RESULT_RISK_X 200
#define LAYOUT_RESULT_RISK_X100 190
#define LAYOUT_RESULT_RISK_Y 380
#define LAYOUT_RESULT_SCALE 4
#define LAYOUT_RESULT_RISK_SCALE 3

#endif /* HEADER_LAYOUT_H_ */
