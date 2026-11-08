/*
 * app.c
 *	LOGO -> PROJECT -> START -> MEASURE -> RESULTS -> MENU
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "main.h"
#include "App/Header/app.h"
#include "App/Header/vitals.h"
#include "UI/Header/screens.h"
#include "Utils/Header/cardFS.h"
#include "Utils/Header/lcd.h"
#include "Utils/Header/touchscreen.h"
#include "IO/Header/sys_io.h"
#include "IO/Header/touchscreen_io.h"

static AppState_t state;
static uint32_t appTimerMS = 0;
static uint32_t measureTimerMS = 0;
static uint8_t lastProgress = 0;
static Vitals_t vitals;

static void Enter_Logo()
{
	Screen_Logo_Draw();
	state = APP_LOGO;
}

static void Enter_Project()
{
	Screen_Project_Draw();
	state = APP_PROJECT;
}

static void Enter_Start()
{
	Screen_Start_Draw();
	Touch_Flush();
	state = APP_START;
}

static void Enter_Menu()
{
	Screen_Menu_Draw();
	Touch_Flush();
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
	Touch_Flush();
	state = APP_RESULTS;
}

static void Enter_Measure()
{
	Screen_Measure_Draw();
	Vitals_Seed(SYS_IO_Millis());
	Vitals_Generate(&vitals);

	Screen_Measure_Begin();
	measureTimerMS = 0;
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
	if (measureTimerMS >= APP_MEASURE_TIME_MS)
	{
		Enter_Results();
		return;
	}

	uint8_t _progress = measureTimerMS * 100 / APP_MEASURE_TIME_MS;
	if (_progress != lastProgress)
	{
		Screen_Measure_Update(_progress, Cuff_Pressure(measureTimerMS));
		lastProgress = _progress;
	}
}

void APP_Init()
{
	LCD_Init();
	FS_Mount();

	LCD_Lock();
	Enter_Logo();
	LCD_Unlock();
}

void APP_Run()
{
	// State Machine din touch => asincron
	Touch_IO_Process();

	// Initiem conversia = IDLE & LCD nu deseneaza
	if (flagFor10ms && touchState == TOUCH_STATE_IDLE && !LCD_IsLocked()) {
		flagFor10ms = false;
		touchState = TOUCH_STATE_PREP_Z;
	}

	// Eveniment la 1 ms
	if (flagFor1ms){
		flagFor1ms = false;
		switch (state)
		{
			case APP_LOGO:
				appTimerMS++;
				if (appTimerMS >= APP_LOGO_TIME_MS)
				{
					LCD_Lock();
					Enter_Project();
					LCD_Unlock();
					appTimerMS = 0;
				}
				break;
			case APP_PROJECT:
				appTimerMS++;
				if (appTimerMS >= APP_PROJECT_TIME_MS)
				{
					LCD_Lock();
					Enter_Start();
					LCD_Unlock();
					appTimerMS = 0;
				}
				break;
			case APP_START:
				if (Screen_Start_Pressed())
				{
					LCD_Lock();
					Enter_Measure();
					LCD_Unlock();
				}
				break;
			case APP_MEASURE:
				measureTimerMS++;
				if (measureTimerMS % 100 == 0)
				{
					LCD_Lock();
					Update_Measure();
					LCD_Unlock();
				}
				break;
			case APP_RESULTS:
				if (Screen_Back_Pressed())
				{
					LCD_Lock();
					Enter_Menu();
					LCD_Unlock();
				}
				break;
			case APP_MENU:
				if (Screen_Restart_Pressed())
				{
					LCD_Lock();
					Enter_Measure();
					LCD_Unlock();
				}
				else if (Screen_View_Pressed())
				{
					LCD_Lock();
					Enter_Results();
					LCD_Unlock();
				}
				break;
		}
	}
	__WFI();
}
