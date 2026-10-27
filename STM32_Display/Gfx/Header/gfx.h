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

void GFX_Clear(uint16_t color);
void GFX_Draw_Border(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t tickness, uint16_t color);
void GFX_Draw_Char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t scale);
void GFX_Draw_Text(uint16_t x, uint16_t y, const char *text, uint16_t color, uint16_t bg_color, uint8_t scale);
void GFX_Draw_Text_Overlay(uint16_t x, uint16_t y, const char *text, uint16_t color, uint8_t scale);
uint16_t GFX_Text_Width(const char *text, uint8_t scale);
uint16_t GFX_Text_Height(uint8_t scale);

#endif /* HEADER_GFX_H_ */
