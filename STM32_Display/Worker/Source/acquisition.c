/*
 * acquisition.c
 *
 *  Created on: 8 Nov 2026
 *      Author: Condurache Gabriel
 */
#include "Worker/Header/acquisition.h"
#include "Worker/Header/vitals.h"
#include "IO/Header/sys_io.h"

static AcquisitionState_t acqState = ACQ_STATE_IDLE;
static uint32_t acqTimerMS = 0;
static Vitals_t calculatedVitals;

void Acquisition_Init()
{
	acqState = ACQ_STATE_IDLE;
	acqTimerMS = 0;
}

void Acquisition_Start()
{
	SystemData_t *_sysData = System_Get_State();
	Vitals_Seed(SYS_IO_Millis());
	acqState = ACQ_STATE_INFLATING;
	acqTimerMS = 0;

	_sysData->resultsReady = false;
	_sysData->isMeasuring = true;
}

void Acquisition_Run()
{
	SystemData_t *_sysData = System_Get_State();

	switch (acqState)
	{
		case ACQ_STATE_IDLE:
			_sysData->isMeasuring = false;
			_sysData->resultsReady = false;
			break;
		case ACQ_STATE_INFLATING:
			_sysData->isMeasuring = true;
			acqTimerMS++;

			if (acqTimerMS <= 4000)
			{
				_sysData->cuffPressure = (acqTimerMS * 180) / 4000;
				_sysData->progressPercent = (acqTimerMS * 50) / 4000;
			} else {
				acqState = ACQ_STATE_MEASURING;
			}
			break;
		case ACQ_STATE_MEASURING:
			acqTimerMS++;

			if (acqTimerMS <= 10000)
			{
				_sysData->cuffPressure = 180 - ((acqTimerMS - 4000) * 140) / 6000;
				_sysData->progressPercent = 50 + ((acqTimerMS - 4000) * 50) / 6000;
			} else {
				acqState = ACQ_STATE_COMPUTING;
			}
			break;
		case ACQ_STATE_COMPUTING:
			Vitals_Generate(&calculatedVitals);
			_sysData->finalResults = calculatedVitals;
			_sysData->resultsReady = true;
			_sysData->isMeasuring = false;
			acqState = ACQ_STATE_FINISHED;
			break;
		case ACQ_STATE_FINISHED:
			/*
			 *  Datele sunt preluate
			 *  Asteptam starea de IDLE
			 */
			break;
	}
}

AcquisitionState_t Acquisition_Get_State()
{
	return acqState;
}
