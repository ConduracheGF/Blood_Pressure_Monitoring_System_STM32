/*
 * vitals.h
 *
 *	Pentru simularea valorilor corespunzatoare generate aleatoriu
 *
 *  Created on: 18 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_VITALS_H_
#define HEADER_VITALS_H_

#include <stdint.h>
#include <stdbool.h>

// SYS > DIA minim 20
#define VITALS_MIN_DIFFERENCE 20

#define HYPER_SYS_NORMAL 120
#define HYPER_SYS_EXTREME 180
#define HYPER_DIA_NORMAL 80
#define HYPER_DIA_EXTREME 110

#define HYPO_SYS_NORMAL 100
#define HYPO_SYS_EXTREME 70
#define HYPO_DIA_NORMAL 65
#define HYPO_DIA_EXTREME 45

#define VITALS_SYS_MIN 80
#define VITALS_SYS_MAX 180
#define VITALS_DIA_MIN 50
#define VITALS_DIA_MAX 110
#define VITALS_BPM_MIN 50
#define VITALS_BPM_MAX 110

#define VITALS_RISK_THRESHOLD 30

typedef enum {
	NORMAL,
	HYPER,
	HYPO
} RiskType_t;

typedef struct {
	bool risk;
	RiskType_t type;
	uint16_t risk_percent;
} TensiuneType_t;

typedef struct {
	uint16_t sys;
	uint16_t dia;
	uint16_t bpm;
	TensiuneType_t tensiune;
} Vitals_t;

void Vitals_Seed(uint32_t seed);
void Vitals_Generate(Vitals_t *vitals);

#endif /* HEADER_VITALS_H_ */
