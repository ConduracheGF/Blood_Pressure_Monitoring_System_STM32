/*
 * cardFS.c
 *
 *  Created on: 10 Oct 2026
 *      Author: Condurache Gabriel
 */

#include <string.h>
#include <stdbool.h>
#include "ff.h"
#include "Utils/Header/cardFS.h"

static FATFS file_system;
static uint8_t last_error = 0;

/*
 * Se citeste sectorul de boot
 * Se verifica FAT ul si intoarce statusul
 */
FsStatus_t FS_Mount()
{
	FRESULT _result = f_mount(&file_system, "0:", 1);
	if (_result != FR_OK)
	{
		last_error = _result;
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

uint8_t FS_List_Files(const char *extension, char names[][FS_MAX_NAME_LENGTH], uint8_t max_files)
{
	DIR _directory;
	FILINFO _info;
	uint8_t _count = 0;
	size_t _extension_length = strlen(extension);

	FRESULT _result = f_opendir(&_directory, "0:/");
	if (_result != FR_OK)
	{
		last_error = _result;
		return 0;
	}

	while (_count < max_files && f_readdir(&_directory, &_info) == FR_OK && _info.fname[0] != 0)
	{
		size_t _name_length = strlen(_info.fname);
		bool _is_file = !(_info.fattrib & (AM_DIR | AM_HID | AM_SYS));

		if (_is_file && _name_length > _extension_length && strcmp(_info.fname + _name_length - _extension_length, extension) == 0)
		{
			strncpy(names[_count], _info.fname, FS_MAX_NAME_LENGTH - 1);
			names[_count][FS_MAX_NAME_LENGTH - 1] = '\0';
			_count++;
		}
	}

	f_closedir(&_directory);
	return _count;
}

uint8_t FS_Get_Last_Error()
{
	return last_error;
}
