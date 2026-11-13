/*
 * app.c
 *
 *  Created on: 8 Nov 2026
 *      Author: Condurache Gabriel
 */
#include "main.h"
#include "Worker/Header/acquisition.h"
#include "Worker/Header/display.h"
#include "Worker/Header/vitals.h"

void APP_Init()
{
	Acquisition_Init();
	Display_Init();
}

void APP_Run()
{
	Acquisition_Run();
	Display_Run();
}
