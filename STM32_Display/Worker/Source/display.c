/*
 * app.c
 *	LOGO -> PROJECT -> START -> MEASURE -> RESULTS -> MENU
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <Worker/Header/display.h>
#include "main.h"
#include "Worker/Header/vitals.h"
#include "UI/Header/screens.h"
#include "Utils/Header/cardFS.h"
#include "Utils/Header/lcd.h"
#include "Utils/Header/touchscreen.h"
#include "IO/Header/sys_io.h"
#include "IO/Header/touchscreen_io.h"
#include "Worker/Header/acquisition.h"

static DisplayState_t displayState;
static uint32_t displayTimerMS = 0;
static uint8_t lastProgress = 0;

static void Enter_Logo()
{
	Screen_Logo_Draw();
	displayState = DISPLAY_LOGO;
}

static void Enter_Project()
{
	Screen_Project_Draw();
	displayState = DISPLAY_PROJECT;
}

static void Enter_Start()
{
	Screen_Start_Draw();
	Touch_Flush();
	displayState = DISPLAY_START;
}

static void Enter_Menu()
{
	Screen_Menu_Draw();
	Touch_Flush();
	displayState = DISPLAY_MENU;
}

static void Enter_Results()
{
	SystemData_t *_data = System_Get_State();
	ScreenRisk_t _risk = SCREEN_RISK_NORMAL;

	if (_data->finalResults.tensiune.type == HYPER)
	{
		_risk = SCREEN_RISK_HYPER;
	}
	else if (_data->finalResults.tensiune.type == HYPO)
	{
		_risk = SCREEN_RISK_HYPO;
	}

	Screen_Results_Draw(
			_data->finalResults.sys,
			_data->finalResults.dia,
			_data->finalResults.bpm,
			_data->finalResults.tensiune.riskPercent,
			_risk
	);
	Touch_Flush();
	displayState = DISPLAY_RESULTS;
}

static void Enter_Measure()
{
	Screen_Measure_Draw();
	Acquisition_Start();

	Screen_Measure_Begin();
	lastProgress = 255;
	displayState = DISPLAY_MEASURE;
}

static void Update_Measure()
{
	SystemData_t *_data = System_Get_State();

	if (_data->resultsReady)
	{
		Enter_Results();
		return;
	}

	if (_data->progressPercent != lastProgress)
	{
		Screen_Measure_Update(_data->progressPercent, _data->cuffPressure);
		lastProgress = _data->progressPercent;
	}
}

void Display_Init()
{
	LCD_Init();
	FS_Mount();

	LCD_Lock();
	Enter_Logo();
	LCD_Unlock();
}

void Display_Run()
{
	// displayState Machine din touch => asincron
	Touch_IO_Process();

	// Initiem conversia = IDLE & LCD nu deseneaza
	if (flagFor10ms && touchState == TOUCH_STATE_IDLE && !LCD_IsLocked()) {
		flagFor10ms = false;
		touchState = TOUCH_STATE_PREP_Z;
	}

	// Eveniment la 1 ms
	if (flagFor1ms){
		flagFor1ms = false;
		switch (displayState)
		{
			case DISPLAY_LOGO:
				displayTimerMS++;
				if (displayTimerMS >= DISPLAY_LOGO_TIME_MS)
				{
					LCD_Lock();
					Enter_Project();
					LCD_Unlock();
					displayTimerMS = 0;
				}
				break;
			case DISPLAY_PROJECT:
				displayTimerMS++;
				if (displayTimerMS >= DISPLAY_PROJECT_TIME_MS)
				{
					LCD_Lock();
					Enter_Start();
					LCD_Unlock();
					displayTimerMS = 0;
				}
				break;
			case DISPLAY_START:
				if (Screen_Start_Pressed())
				{
					LCD_Lock();
					Enter_Measure();
					LCD_Unlock();
				}
				break;
			case DISPLAY_MEASURE:
				LCD_Lock();
				Update_Measure();
				LCD_Unlock();
				break;
			case DISPLAY_RESULTS:
				if (Screen_Back_Pressed())
				{
					LCD_Lock();
					Enter_Menu();
					LCD_Unlock();
				}
				break;
			case DISPLAY_MENU:
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
