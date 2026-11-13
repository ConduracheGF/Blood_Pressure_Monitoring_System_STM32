/*
 * acquisition.h
 *
 *  Created on: 8 Nov 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_ACQUISITION_H_
#define HEADER_ACQUISITION_H_

#include <stdint.h>
#include <stdbool.h>
#include "Worker/Header/vitals.h"

typedef enum {
	ACQ_STATE_IDLE, // Repaus
	ACQ_STATE_INFLATING, // Se umfla manseta si se porneste pompa
	ACQ_STATE_MEASURING, // Masurare valori si dezumflare controlata
	ACQ_STATE_COMPUTING, // Calcul valori
	ACQ_STATE_FINISHED // Finalizare
} AcquisitionState_t;

void Acquisition_Init();
void Acquisition_Start();
void Acquisition_Run();
AcquisitionState_t Acquisition_Get_State();

#endif /* HEADER_ACQUISITION_H_ */
