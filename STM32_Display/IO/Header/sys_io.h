/*
 * sys_io.h
 *
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_SYS_IO_H_
#define HEADER_SYS_IO_H_

#include <stdint.h>
#include <stdbool.h>

extern volatile bool flagFor1ms;
extern volatile bool flagFor10ms;
extern volatile bool flagFor100ms;

uint32_t SYS_IO_Millis();
void SYS_IO_Delay_MS(uint32_t ms);

#endif /* HEADER_SYS_IO_H_ */
