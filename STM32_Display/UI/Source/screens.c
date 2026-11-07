/*
 * screens.c
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <stdio.h>
#include <stdbool.h>
#include "UI/Header/screens.h"
#include "UI/Header/widgets.h"
#include "UI/Header/layout.h"
#include "Gfx/Header/gfx.h"
#include "Gfx/Header/bmp.h"
#include "Utils/Header/touchscreen.h"
#include "Utils/Header/lcd.h"
#include "IO/Header/sys_io.h"

#define BAR_BORDER 2
#define BAR_CHUNK 4
#define PRESSURE_TEXT_SCALE 3

#if UI_DEBUG_ZONE
#define DEBUG_ZONE(zone) GFX_Draw_Border((zone)->x, (zone)->y, (zone)->width, (zone)->height, 2, CULOARE_ROSU)
#else
#define DEBUG_ZONE(zone)
#endif

static const Rect_t zoneStart = LAYOUT_ZONE_START;
static const Rect_t zoneBack = LAYOUT_ZONE_BACK;
static const Rect_t zoneRestart = LAYOUT_ZONE_RESTART;
static const Rect_t zoneView = LAYOUT_ZONE_VIEW;
static const Rect_t barArea = LAYOUT_BAR;
static const Rect_t pressureField = LAYOUT_PRESSURE_FIELD;

static uint16_t barFilledWidth = 0;

static void Draw_Image(const char *name)
{
	uint32_t _start = SYS_IO_Millis(); // TEMPORAR
	BmpStatus_t _status = BMP_Draw_File(name, 0, 0);
	uint32_t _elapsed = SYS_IO_Millis() - _start;	// TEMPORAR
	if (_status != BMP_NORMAL)
	{
		char _text[32];
		GFX_Clear(UI_COLOR_BACKGROUND);
		snprintf(_text, sizeof(_text), "%s BMP: %d", name, _status);
		GFX_Draw_Text(10, 10, _text, CULOARE_ROSU, UI_COLOR_BACKGROUND, 2);
	}
	char _time[16];	// TEMPORAR
	snprintf(_time, sizeof(_time), "%lu MS", _elapsed);	// TEMPORAR
	GFX_Draw_Text(5, LCD_HEIGHT - 20, _time, CULOARE_ALB, CULOARE_NEGRU, 2);	// TEMPORAR
}

static uint16_t Bar_Color(uint16_t positionPercent)
{
	if (positionPercent < 50)
	{
		return CULOARE_VERDE;
	}
	if (positionPercent < 80)
	{
		return UI_COLOR_YELLOW;
	}
	return CULOARE_ROSU;
}

static bool Zone_Pressed(const Rect_t *zone)
{
	TouchPoint_t _point;
	return Touch_Get_Point(&_point) && Widget_Contains(zone, _point.x, _point.y);
}

void Screen_Logo_Draw()
{
	Draw_Image(IMAGE_LOGO);
}

void Screen_Project_Draw()
{
	Draw_Image(IMAGE_PROJECT);
}

void Screen_Start_Draw()
{
	Draw_Image(IMAGE_START);
	DEBUG_ZONE(&zoneStart);
}

void Screen_Menu_Draw()
{
	Draw_Image(IMAGE_MENU);
	DEBUG_ZONE(&zoneRestart);
	DEBUG_ZONE(&zoneView);
}

void Screen_Measure_Draw()
{
	Draw_Image(IMAGE_MEASURE);
	DEBUG_ZONE(&barArea);
}

static bool compareThreshold(uint16_t prag)
{
	return (prag > 100) ? true : false;
}

void Screen_Results_Draw(uint16_t sys, uint16_t dia, uint16_t bpm, uint8_t riskPercent, ScreenRisk_t risk)
{
	char _text[24];
	const char *_riskTitle = "-";

	if (risk == SCREEN_RISK_HYPER)
	{
		_riskTitle = "H";
	}
	else if (risk == SCREEN_RISK_HYPO)
	{
		_riskTitle = "h";
	}

	Draw_Image(IMAGE_RESULTS);

	snprintf(_text, sizeof(_text), "%u", sys);
	if (compareThreshold(sys)){
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_SYS_X100, LAYOUT_RESULT_SYS_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	} else {
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_SYS_X, LAYOUT_RESULT_SYS_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	}

	snprintf(_text, sizeof(_text), "%u", dia);
	if (compareThreshold(dia))
	{
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_DIA_X100, LAYOUT_RESULT_DIA_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	} else {
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_DIA_X, LAYOUT_RESULT_DIA_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	}

	snprintf(_text, sizeof(_text), "%u", bpm);
	if (compareThreshold(bpm))
	{
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_BPM_X100, LAYOUT_RESULT_BPM_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	} else {
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_BPM_X, LAYOUT_RESULT_BPM_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	}

	snprintf(_text, sizeof(_text), "%u%%", riskPercent);
	if (compareThreshold(riskPercent))
	{
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_RISK_X100, LAYOUT_RESULT_RISK_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	} else {
		GFX_Draw_Text_Overlay(LAYOUT_RESULT_RISK_X, LAYOUT_RESULT_RISK_Y, _text, UI_COLOR_OVERLAY, LAYOUT_RESULT_SCALE);
	}

	DEBUG_ZONE(&zoneBack);
}

void Screen_Measure_Begin()
{
	GFX_Draw_Border(barArea.x, barArea.y, barArea.width, barArea.height, BAR_BORDER, UI_COLOR_TEXT);
	LCD_Fill_Rect(barArea.x + BAR_BORDER, barArea.y + BAR_BORDER, barArea.width - 2 * BAR_BORDER, barArea.height - 2 * BAR_BORDER, UI_COLOR_BAR_EMPTY);
	LCD_Fill_Rect(pressureField.x, pressureField.y, pressureField.width, pressureField.height, UI_COLOR_FIELD);
	barFilledWidth = 0;
}

void Screen_Measure_Update(uint8_t progressPercent, uint16_t pressure)
{
	char _text[16];
	uint16_t _innerWidth = barArea.width - 2 * BAR_BORDER;
	uint16_t _newWidth = (uint32_t)progressPercent * _innerWidth / 100;

	for (uint16_t offset = barFilledWidth; offset < _newWidth; offset += BAR_CHUNK)
	{
		uint16_t _width = _newWidth - offset;
		if (_width > BAR_CHUNK)
		{
			_width = BAR_CHUNK;
		}
		LCD_Fill_Rect(barArea.x + BAR_BORDER + offset, barArea.y + BAR_BORDER, _width,
				barArea.height - 2 * BAR_BORDER, Bar_Color(offset * 100 / _innerWidth));
	}
	if (_newWidth > barFilledWidth)
	{
		barFilledWidth = _newWidth;
	}

	snprintf(_text, sizeof(_text), "%3u MMGH", pressure);
	uint16_t _textY = pressureField.y + (pressureField.height - GFX_Text_Height(PRESSURE_TEXT_SCALE)) / 2;
	Widget_Draw_Text_Centered(&pressureField, _textY, _text, UI_COLOR_TEXT, UI_COLOR_FIELD, PRESSURE_TEXT_SCALE);
}

static bool Button_Pressed(const Button_t *button)
{
	TouchPoint_t _point;
	return Touch_Get_Point(&_point) && Widget_Contains(&button->area, _point.x, _point.y);
}

bool Screen_Start_Pressed()
{
	return Zone_Pressed(&zoneStart);
}

bool Screen_Back_Pressed()
{
	return Zone_Pressed(&zoneBack);
}

bool Screen_Restart_Pressed()
{
	return Zone_Pressed(&zoneRestart);
}

bool Screen_View_Pressed()
{
	return Zone_Pressed(&zoneView);
}
