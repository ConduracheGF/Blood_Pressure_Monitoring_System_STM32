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

void widget_draw_text_centered(const Rect_t *area, uint16_t y, const char *text, uint16_t color, uint16_t bg_color, uint8_t scale);
void widget_draw_button(const Button_t *button);
void widget_draw_value_box(const Rect_t *area, const char *title, const char *value, uint16_t color);
bool widget_contains(const Rect_t *area, uint16_t x, uint16_t y);

#endif /* HEADER_WIDGETS_H_ */
