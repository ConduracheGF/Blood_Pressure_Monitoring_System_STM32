/*
 * cardSD.h
 *
 *  Created on: 7 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_CARDSD_H_
#define HEADER_CARDSD_H_

#include <stdint.h>

#define SD_BLOCK_SIZE 512

/*
 * SD_NORMAL = Functionare normala a sistemului de card
 * SD_ERROR_NO_CARD = Cardul nu raspunde la reset
 * SD_ERROR_INIT = Cardul raspunde, dar initializarea are fail
 * SD_ERROR_READ = TIMEOUT / Raspuns invalid la citire
 * */
typedef enum {
	SD_NORMAL,
	SD_ERROR_NO_CARD,
	SD_ERROR_INIT,
	SD_ERROR_READ
} SdStatus_t;

SdStatus_t SD_Init();
SdStatus_t SD_Read_Block(uint32_t block, uint8_t *buffer);
SdStatus_t SD_Read_Blocks(uint32_t block, uint8_t *buffer, uint32_t count);

#endif /* HEADER_CARDSD_H_ */
