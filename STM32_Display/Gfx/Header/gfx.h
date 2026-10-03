/*
 * gfx.h
 *
 *  Created on: 3 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_GFX_H_
#define HEADER_GFX_H_

#include <stdint.h>

#define GFX_MAX_SCALE 4

void gfx_clear(uint16_t color);
void gfx_draw_border(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t tickness, uint16_t color);
void gfx_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t scale);
void gfx_draw_text(uint16_t x, uint16_t y, const char *text, uint16_t color, uint16_t bg_color, uint8_t scale);
uint16_t gfx_text_width(const char *text, uint8_t scale);

#endif /* HEADER_GFX_H_ */
