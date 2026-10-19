/*
 * screens.c
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <stdio.h>
#include "UI/Header/screens.h"
#include "UI/Header/widgets.h"
#include "UI/Header/layout.h"
#include "Gfx/Header/gfx.h"
#include "Utils/Header/touchscreen.h"

static const Rect_t screen_area = LAYOUT_SCREEN;
static const Rect_t sys_box = LAYOUT_RESULTS_SYS_BOX;
static const Rect_t dia_box = LAYOUT_RESULTS_DIA_BOX;
static const Rect_t bpm_box = LAYOUT_RESULTS_BPM_BOX;
static const Rect_t risk_box = LAYOUT_RESULTS_RISK_BOX;

static const Button_t start_button = {LAYOUT_START_BUTTON, "START", UI_COLOR_BUTTON};
static const Button_t restart_button = {LAYOUT_RESULTS_RESTART_BUTTON, "RESTART", UI_COLOR_BUTTON};

void screen_start_draw()
{
	gfx_clear(UI_COLOR_BACKGROUND);
	widget_draw_text_centered(&screen_area, LAYOUT_START_TITLE_Y, "MASURARE TENSIUNE", UI_COLOR_TEXT, UI_COLOR_BACKGROUND, 2);
	widget_draw_button(&start_button);
}

void screen_simulation_draw()
{
	gfx_clear(UI_COLOR_BACKGROUND);
	widget_draw_text_centered(&screen_area, LAYOUT_SIMULATION_TITLE_Y, "SIMULARE", UI_COLOR_TEXT, UI_COLOR_BACKGROUND, 3);
}

// Functie de actualizare a numerelor afisate
void screen_simulation_update(uint8_t second_left)
{
	char text[8];
	snprintf(text, sizeof(text), "%2u", second_left);
	widget_draw_text_centered(&screen_area, LAYOUT_SIMULATION_COUNTER_Y, text, UI_COLOR_VALUE, UI_COLOR_BACKGROUND, 4);
}

void screen_results_draw(uint16_t sys, uint16_t dia, uint16_t bpm, uint8_t risk_percent, ScreenRisk_t risk)
{
	char text[8];
	const char *risk_title = "NORMAL";
	uint16_t risk_color = UI_COLOR_VALUE;

	if (risk == SCREEN_RISK_HYPER)
	{
		risk_title = "RISC HIPER";
		risk_color = UI_COLOR_RISK;
	}
	else if (risk == SCREEN_RISK_HYPO)
	{
		risk_title = "RISC HIPO";
		risk_color = UI_COLOR_RISK;
	}

	gfx_clear(UI_COLOR_BACKGROUND);
	widget_draw_text_centered(&screen_area, LAYOUT_RESULTS_TITLE_Y, "REZULTATE", UI_COLOR_TEXT, UI_COLOR_BACKGROUND, 3);

	snprintf(text, sizeof(text), "%u", sys);
	widget_draw_value_box(&sys_box, "SYS", text, UI_COLOR_VALUE);

	snprintf(text, sizeof(text), "%u", dia);
	widget_draw_value_box(&dia_box, "DIA", text, UI_COLOR_VALUE);

	snprintf(text, sizeof(text), "%u", bpm);
	widget_draw_value_box(&bpm_box, "BPM", text, UI_COLOR_VALUE);


	snprintf(text, sizeof(text), "%u%%", risk_percent);
	widget_draw_value_box(&risk_box, risk_title, text, risk_color);

	widget_draw_button(&restart_button);
}

static bool Button_Pressed(const Button_t *button)
{
	TouchPoint_t _point;
	return Touch_Get_Point(&_point) && widget_contains(&button->area, _point.x, _point.y);
}

bool screen_start_button_pressed()
{
	return Button_Pressed(&start_button);
}

bool screen_restart_button_pressed()
{
	return Button_Pressed(&restart_button);
}
