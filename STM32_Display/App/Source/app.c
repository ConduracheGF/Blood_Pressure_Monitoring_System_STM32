/*
 * app.c
 *	LOGO -> PROJECT -> START -> MEASURE -> RESULTS -> MENU
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "App/Header/app.h"
#include "App/Header/vitals.h"
#include "UI/Header/screens.h"
#include "Utils/Header/cardFS.h"
#include "Utils/Header/lcd.h"
#include "IO/Header/sys_io.h"

static AppState_t state;
static uint32_t shownAt = 0;
static uint32_t measureStartedAt = 0;
static uint8_t lastProgress = 0;
static Vitals_t vitals;

static void Enter_Logo()
{
	Screen_Logo_Draw();
	shownAt = SYS_IO_Millis();
	state = APP_LOGO;
}

static void Enter_Project()
{
	Screen_Project_Draw();
	shownAt = SYS_IO_Millis();
	state = APP_PROJECT;
}

static void Enter_Start()
{
	Screen_Start_Draw();
	state = APP_START;
}

static void Enter_Menu()
{
	Screen_Menu_Draw();
	state = APP_MENU;
}

static void Enter_Results()
{
	ScreenRisk_t _risk = SCREEN_RISK_NORMAL;
	if (vitals.tensiune.type == HYPER)
	{
		_risk = SCREEN_RISK_HYPER;
	}
	else if (vitals.tensiune.type == HYPO)
	{
		_risk = SCREEN_RISK_HYPO;
	}

	Screen_Results_Draw(vitals.sys, vitals.dia, vitals.bpm, vitals.tensiune.riskPercent, _risk);
	state = APP_RESULTS;
}

static void Enter_Measure()
{
	Screen_Measure_Draw();
	Vitals_Seed(SYS_IO_Millis());
	Vitals_Generate(&vitals);

	Screen_Measure_Begin();
	measureStartedAt = SYS_IO_Millis();
	lastProgress = 255;
	state = APP_MEASURE;
}

static uint16_t Cuff_Pressure(uint32_t elapsedMS)
{
	uint32_t _peak = vitals.sys + APP_CUFF_OVERSHOOT;
	uint32_t _inflateMS = APP_MEASURE_TIME_MS + APP_INFLATE_PERCENT / 100;

	if (elapsedMS < _inflateMS)
	{
		return _peak  * elapsedMS / _inflateMS;
	}
	return _peak * (APP_MEASURE_TIME_MS - elapsedMS);
}

static void Update_Measure()
{
	uint32_t _elapsed = SYS_IO_Millis() - measureStartedAt;

	if (_elapsed >= APP_MEASURE_TIME_MS)
	{
		Enter_Results();
		return;
	}

	uint8_t _progress = _elapsed * 100 / APP_MEASURE_TIME_MS;
	if (_progress != lastProgress)
	{
		Screen_Measure_Update(_progress, Cuff_Pressure(_elapsed));
		lastProgress = _progress;
	}
}

void APP_Init()
{
	LCD_Init();
	FS_Mount();
	Enter_Logo();
}

void APP_Run()
{
	switch (state)
	{
		case APP_LOGO:
			if (SYS_IO_Millis() - shownAt >= APP_LOGO_TIME_MS)
			{
					Enter_Project();
			}
			break;
		case APP_PROJECT:
			if (SYS_IO_Millis() - shownAt >= APP_PROJECT_TIME_MS)
			{
				Enter_Start();
			}
			break;
		case APP_START:
			if (Screen_Start_Pressed())
			{
				Enter_Measure();
			}
			break;
		case APP_MEASURE:
			Update_Measure();
			break;
		case APP_RESULTS:
			if (Screen_Back_Pressed())
			{
				Enter_Menu();
			}
			break;
		case APP_MENU:
			if (Screen_Restart_Pressed())
			{
				Enter_Measure();
			}
			else if (Screen_View_Pressed())
			{
				Enter_Results();
			}
			break;
	}
}
