/*
 * cardFS.h
 *
 *  Created on: 10 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_CARDFS_H_
#define HEADER_CARDFS_H_

#include <stdint.h>

#define FS_MAX_NAME_LENGTH 13

typedef enum {
	FS_NORMAL,
	FS_ERROR_MOUNT
} FsStatus_t;

FsStatus_t FS_Mount();
uint8_t FS_List_Files(const char *extension, char names[][FS_MAX_NAME_LENGTH], uint8_t max_files);
uint8_t FS_Get_Last_Error(); // Se obtine codul FRESULT din FatFS cu ultima eroare

#endif /* HEADER_CARDFS_H_ */
