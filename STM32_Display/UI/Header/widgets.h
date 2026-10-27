/*
 * widgets.h
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#ifndef HEADER_WIDGETS_H_
#define HEADER_WIDGETS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
} Rect_t;

typedef struct {
	Rect_t area;
	const char *label;
	uint16_t color;
} Button_t;

void Widget_Draw_Text_Centered(const Rect_t *area, uint16_t y, const char *text, uint16_t color, uint16_t bgColor, uint8_t scale);
void Widget_Draw_Button(const Button_t *button);
void Widget_Draw_Value_Box(const Rect_t *area, const char *title, const char *value, uint16_t color);
bool Widget_Contains(const Rect_t *area, uint16_t x, uint16_t y);

#endif /* HEADER_WIDGETS_H_ */
