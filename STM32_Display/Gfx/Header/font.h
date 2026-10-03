/*
 * font.h
 *
 *  Created on: 3 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_FONT_H_
#define HEADER_FONT_H_

#include <stdint.h>

#define FONT_WIDTH 5
#define FONT_HEIGHT 7
#define FONT_CELL_WIDTH (FONT_WIDTH + 1)
#define FONT_CELL_HEIGHT (FONT_HEIGHT + 1)

#define FONT_FIRST_CHAR ' '
#define FONT_LAST_CHAR 'Z'

extern const uint8_t font_5x7[FONT_LAST_CHAR - FONT_FIRST_CHAR + 1][FONT_WIDTH];

#endif /* HEADER_FONT_H_ */
