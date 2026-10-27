/*
 * cardFS.c
 *
 *  Created on: 10 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "ff.h"
#include "Utils/Header/cardFS.h"

static FATFS fileSystem;
static FIL file;
static uint8_t lastError = 0;

/*
 * Se citeste sectorul de boot
 * Se verifica FAT ul si intoarce statusul
 */
FsStatus_t FS_Mount()
{
	FRESULT _result = f_mount(&fileSystem, "0:", 1);
	if (_result != FR_OK)
	{
		lastError = _result;
		return FS_ERROR_MOUNT;
	}
	return FS_NORMAL;
}

/*
 * Se salveaza fisierele cu radacina .BMP
 * Se intoarce numarul de fisiere gasite
 *
 * ! Pentru master: codul e configurat pentru nume cu majuscule !
 */

uint8_t FS_List_Files(const char *extension, char names[][FS_MAX_NAME_LENGTH], uint8_t maxFiles)
{
	DIR _directory;
	FILINFO _info;
	uint8_t _count = 0;
	size_t _extensionLength = strlen(extension);

	FRESULT _result = f_opendir(&_directory, "0:/");
	if (_result != FR_OK)
	{
		lastError = _result;
		return 0;
	}

	while (_count < maxFiles && f_readdir(&_directory, &_info) == FR_OK && _info.fname[0] != 0)
	{
		size_t _nameLength = strlen(_info.fname);
		bool _isFile = !(_info.fattrib & (AM_DIR | AM_HID | AM_SYS));

		if (_isFile && _nameLength > _extensionLength && strcmp(_info.fname + _nameLength - _extensionLength, extension) == 0)
		{
			strncpy(names[_count], _info.fname, FS_MAX_NAME_LENGTH - 1);
			names[_count][FS_MAX_NAME_LENGTH - 1] = '\0';
			_count++;
		}
	}

	f_closedir(&_directory);
	return _count;
}

// Se deschide fisierul din radacina cardului: blocul 0 de memorie
FsStatus_t FS_Open(const char *name)
{
	char _path[FS_MAX_NAME_LENGTH + 4];
	snprintf(_path, sizeof(_path), "0:/%s", name);

	FRESULT _result = f_open(&file, _path, FA_READ);
	if (_result != FR_OK) {
		lastError = _result;
		return FS_ERROR_OPEN;
	}
	return FS_NORMAL;
}

FsStatus_t FS_Read(void *buffer, uint16_t length, uint16_t *bytes_read)
{
	UINT _readCount = 0;
	FRESULT _result = f_read(&file, buffer, length, &_readCount);
	*bytes_read = _readCount;

	if (_result != FR_OK) {
		lastError = _result;
		return FS_ERROR_READ;
	}
	return FS_NORMAL;
}

FsStatus_t FS_Seek(uint32_t offset)
{
	FRESULT _result = f_lseek(&file, offset);

	if (_result != FR_OK) {
		lastError = _result;
		return FS_ERROR_READ;
	}
	return FS_NORMAL;
}

void FS_Close()
{
	f_close(&file);
}

uint8_t FS_Get_Last_Error()
{
	return lastError;
}
