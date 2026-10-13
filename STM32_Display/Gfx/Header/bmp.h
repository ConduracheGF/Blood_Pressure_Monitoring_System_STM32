/*
 * bmp.h
 *
 *  Created on: 13 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_BMP_H_
#define HEADER_BMP_H_

#include <stdint.h>

/*
 * BMP_NORMAL = Functionare normala a fisierului
 * BMP_ERROR_OPEN = Fisierul nu s-a putut deschide
 * BMP_ERROR_FORMAT = Formatul fisierului nu este BMP pe 24 biti necomprimat
 * BMP_ERROR_SIZE = Dimensiunea pozei nu incape pe ecran
 * BMP_ERROR_READ = Eroare la citirea fisierului
 */
typedef enum {
	BMP_NORMAL,
	BMP_ERROR_OPEN,
	BMP_ERROR_FORMAT,
	BMP_ERROR_SIZE,
	BMP_ERROR_READ
} BmpStatus_t;

BmpStatus_t BMP_Draw_File(const char *name, uint16_t x, uint16_t y);

#endif /* HEADER_BMP_H_ */
