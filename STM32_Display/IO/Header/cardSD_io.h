/*
 * cardSD_io.h
 *
 *  Created on: 6 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_CARDSD_IO_H_
#define HEADER_CARDSD_IO_H_

#include <stdint.h>

typedef enum {
	SD_IO_SPEED_INIT, // 390 kHz pentru initializare
	SD_IO_SPEED_FAST // 12.5 MHz pentru transfer
} SdIoSpeed_t;

void SD_IO_Init();
void SD_IO_Select();
void SD_IO_Deselect();
uint8_t SD_IO_Transfer_Byte(uint8_t byte);
void SD_IO_Receive_Bytes(uint8_t *buffer, uint16_t length);
void SD_IO_Set_Speed(SdIoSpeed_t speed);

#endif /* HEADER_CARDSD_IO_H_ */
