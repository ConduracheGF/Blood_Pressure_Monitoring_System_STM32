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
void Widget_Draw_Text_Centered(const Rect_t *area, uint16_t y, const char *text, uint16_t color, uint16_t bgColor, uint8_t scale)
{
	uint16_t textWidth = GFX_Text_Width(text, scale) - scale;
	uint16_t x = area->x;

	if (textWidth < area->width)
	{
		x += (area->width - textWidth) / 2;
	}
	GFX_Draw_Text(x, y, text, color, bgColor, scale);
}

void WidgetDrawButton(const Button_t *button)
{
	LCD_Fill_Rect(button->area.x, button->area.y, button->area.width, button->area.height, button->color);

	uint16_t textY = button->area.y + (button->area.height - GFX_Text_Height(BUTTON_TEXT_SCALE)) / 2;
	Widget_Draw_Text_Centered(&button->area, textY, button->label, UI_COLOR_BACKGROUND, button->color, BUTTON_TEXT_SCALE);
}

void Widget_Draw_Value_Box(const Rect_t *area, const char *title, const char *value, uint16_t color)
{
	GFX_Draw_Border(area->x, area->y, area->width, area->height, VALUE_BOX_BORDER, color);

	Widget_Draw_Text_Centered(area, area->y + 14, title, UI_COLOR_TEXT, UI_COLOR_BACKGROUND, VALUE_BOX_TITLE_SCALE);

	uint16_t valueY = area->y + (area->height - GFX_Text_Height(VALUE_BOX_VALUE_SCALE)) / 2 + 12;
	Widget_Draw_Text_Centered(area, valueY, value, color, UI_COLOR_BACKGROUND, VALUE_BOX_VALUE_SCALE);
}

bool Widget_Contains(const Rect_t *area, uint16_t x, uint16_t y)
{
	return (x >= area->x && x < area->x + area->width && y >= area->y && y < area->y + area->height);
}
