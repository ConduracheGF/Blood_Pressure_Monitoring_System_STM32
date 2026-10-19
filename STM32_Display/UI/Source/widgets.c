/*
 * widgets.c
 *
 *  Created on: 4 Oct 2026
 *      Author: Condurache Gabriel
 */

#include "UI/Header/widgets.h"
#include "UI/Header/layout.h"
#include "Gfx/Header/gfx.h"
#include "Utils/Header/lcd.h"

#define BUTTON_TEXT_SCALE 3
#define VALUE_BOX_BORDER 3
#define VALUE_BOX_TITLE_SCALE 2
#define VALUE_BOX_VALUE_SCALE 4

// Se centreaza textul pe orizontala in zona data, la inaltimea y
void widget_draw_text_centered(const Rect_t *area, uint16_t y, const char *text, uint16_t color, uint16_t bg_color, uint8_t scale)
{
	uint16_t textWidth = gfx_text_width(text, scale) - scale;
	uint16_t x = area->x;

	if (textWidth < area->width)
	{
		x += (area->width - textWidth) / 2;
	}
	gfx_draw_text(x, y, text, color, bg_color, scale);
}

void widget_draw_button(const Button_t *button)
{
	lcd_fill_rect(button->area.x, button->area.y, button->area.width, button->area.height, button->color);

	uint16_t textY = button->area.y + (button->area.height - gfx_text_height(BUTTON_TEXT_SCALE)) / 2;
	widget_draw_text_centered(&button->area, textY, button->label, UI_COLOR_BACKGROUND, button->color, BUTTON_TEXT_SCALE);
}

void widget_draw_value_box(const Rect_t *area, const char *title, const char *value, uint16_t color)
{
	gfx_draw_border(area->x, area->y, area->width, area->height, VALUE_BOX_BORDER, color);

	widget_draw_text_centered(area, area->y + 14, title, UI_COLOR_TEXT, UI_COLOR_BACKGROUND, VALUE_BOX_TITLE_SCALE);

	uint16_t valueY = area->y + (area->height - gfx_text_height(VALUE_BOX_VALUE_SCALE)) / 2 + 12;
	widget_draw_text_centered(area, valueY, value, color, UI_COLOR_BACKGROUND, VALUE_BOX_VALUE_SCALE);
}

bool widget_contains(const Rect_t *area, uint16_t x, uint16_t y)
{
	return (x >= area->x && x < area->x + area->width && y >= area->y && y < area->y + area->height);
}
