/*
 * app.c
 *
 *  Created on: 19 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "App/Header/app.h"
#include "App/Header/vitals.h"
#include "UI/Header/screens.h"
#include "Gfx/Header/bmp.h"
#include "Gfx/Header/gfx.h"
#include "Utils/Header/cardFS.h"
#include "Utils/Header/lcd.h"
#include "IO/Header/sys_io.h"

static AppState_t state;
static char photo_names[APP_MAX_PHOTOS][FS_MAX_NAME_LENGTH];
static uint8_t photo_count = 0;
static uint8_t photo_index = 0;
static uint8_t photo_shown_at = 0;
static uint8_t simulation_started_at = 0;
static uint8_t seconds_shown = 0;
static Vitals_t vitals;

static void Enter_Start()
{
	screen_start_draw();
	state = APP_START;
}

static void Show_Photo()
{
	gfx_clear(CULOARE_NEGRU);
	BmpStatus_t _status = BMP_Draw_File(photo_names[photo_index], 0, 0);
	photo_shown_at = sys_io_millis();

	if (_status != BMP_NORMAL)
	{
		photo_shown_at -= APP_PHOTO_TIME_MS;
	}
}

static void Enter_Photos()
{
	photo_index = 0;
	if (photo_count == 0)
	{
		Enter_Start();
		return;
	}

	Show_Photo();
	state = APP_PHOTOS;
}

static void Enter_Results()
{
	Vitals_Generate(&vitals);

	ScreenRisk_t _risk = SCREEN_RISK_NORMAL;
	if (vitals.tensiune.type == HYPER)
	{
		_risk = SCREEN_RISK_HYPER;
	}
	else if (vitals.tensiune.type == HYPO)
	{
		_risk = SCREEN_RISK_HYPO;
	}

	screen_results_draw(vitals.sys, vitals.dia, vitals.bpm, vitals.tensiune.risk_percent, _risk);
	state = APP_RESULTS;
}

static void Enter_Simulation()
{
	Vitals_Seed(sys_io_millis());
	screen_simulation_draw();
	simulation_started_at = sys_io_millis();
	seconds_shown = 0;
	state = APP_SIMULATION;
}

static void Update_Simulation()
{
	uint32_t _elapsed_seconds = (sys_io_millis() - simulation_started_at) / 1000;
	if (_elapsed_seconds >= APP_SIMULATION_SECONDS)
	{
		Enter_Results();
		return;
	}

	uint8_t _seconds_left = APP_SIMULATION_SECONDS - _elapsed_seconds;
	if (_seconds_left != seconds_shown)
	{
		screen_simulation_update(_seconds_left);
		seconds_shown = _seconds_left;
	}
}

void APP_Init()
{
	lcd_init();
	if (FS_Mount() == FS_NORMAL)
	{
		photo_count = FS_List_Files(".BMP", photo_names, APP_MAX_PHOTOS);
	}

	Enter_Photos();
}

void APP_Run()
{
	switch (state)
	{
		case APP_PHOTOS:
			if (sys_io_millis() - photo_shown_at >= APP_PHOTO_TIME_MS)
			{
				photo_index++;
				if (photo_index >= photo_count)
				{
					Enter_Start();
				}
				else
				{
					Show_Photo();
				}
			}
			break;
		case APP_START:
			if (screen_start_button_pressed())
			{
				Enter_Simulation();
			}
			break;
		case APP_SIMULATION:
			Update_Simulation();
			break;
		case APP_RESULTS:
			if (screen_restart_button_pressed())
			{
				Enter_Photos();
			}
			break;
	}
}
