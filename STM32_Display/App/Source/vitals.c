/*
 * vitals.c
 *
 *  Created on: 18 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <stdlib.h>
#include "App/Header/vitals.h"

static uint16_t Random_Between(uint16_t minim, uint16_t maxim)
{
	return minim + rand() % (maxim - minim + 1);
}

static uint8_t Percent(int32_t value, int32_t normalLimit, int32_t extremeLimit)
{
	int32_t _percent = (value - normalLimit) * 100 / (extremeLimit - normalLimit);
	if (_percent < 0)
	{
		return 0;
	}
	if (_percent > 100)
	{
		return 100;
	}
	return (uint8_t)_percent;
}

static uint8_t Max(uint8_t a, uint8_t b)
{
	return (a > b) ? a : b;
}

void Vitals_Seed(uint32_t seed)
{
	srand(seed);
}

void Vitals_Generate(Vitals_t *vitals)
{
	vitals->dia = Random_Between(VITALS_DIA_MIN, VITALS_DIA_MAX);
	vitals->sys = Random_Between(VITALS_SYS_MIN, VITALS_SYS_MAX);
	vitals->bpm = Random_Between(VITALS_BPM_MIN, VITALS_BPM_MAX);

	if (vitals->sys < vitals->dia + VITALS_MIN_DIFFERENCE)
	{
		vitals->sys = vitals->dia + VITALS_MIN_DIFFERENCE;
	}

	uint8_t _hyper = Max(Percent(vitals->sys, HYPER_SYS_NORMAL, HYPER_SYS_EXTREME),
						 Percent(vitals->dia, HYPER_DIA_NORMAL, HYPER_DIA_EXTREME));
	uint8_t _hypo = Max(Percent(vitals->sys, HYPO_SYS_NORMAL, HYPO_SYS_EXTREME),
						Percent(vitals->dia, HYPO_DIA_NORMAL, HYPO_DIA_EXTREME));

	vitals->tensiune.riskPercent = Max(_hyper, _hypo);

	if (vitals->tensiune.riskPercent > VITALS_RISK_THRESHOLD)
	{
		vitals->tensiune.risk = true;
		vitals->tensiune.type = (_hyper > _hypo) ? HYPER : HYPO;
	}
	else
	{
		vitals->tensiune.risk = false;
		vitals->tensiune.type = NORMAL;
	}
}
