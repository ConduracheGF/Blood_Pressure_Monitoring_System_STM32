/*
 * touchscreen_io.c
 * Pinii se trec in modul de touch
 * Touch rezistiv in 4 fire, legat la aceleasi linii cu LCD
 * 		XP = D8 (LCD_D0)
 * 		XM = A2 (LCD_RS)
 * 		YP = A3 (LCD_CS)
 * 		YM = D9 (LCD_D1)
 * 	Pe o folie se pune tensiune, pe cealalta se masoara ADC-ul
 * 	Dupa fiecare citire pinii vor fi trecuti pe gpio_output
 *  Created on: 2 Oct 2026
 *      Author: Condurache Gabriel
 */
#include "main.h"
#include "IO/Header/touchscreen_io.h"

#define TOUCH_ADC_SAMPLES 4
#define TOUCH_ADC_TIMEOUT_MS 10
#define TOUCH_XM_ADC_CHANNEL ADC_CHANNEL_4
#define TOUCH_YP_ADC_CHANNEL ADC_CHANNEL_8

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} TouchPin_t;

extern ADC_HandleTypeDef hadc1;

static const TouchPin_t xp = {LCD_D0_GPIO_Port, LCD_D0_Pin};
static const TouchPin_t xm = {LCD_RS_GPIO_Port, LCD_RS_Pin};
static const TouchPin_t yp = {LCD_CS_GPIO_Port, LCD_CS_Pin};
static const TouchPin_t ym = {LCD_D1_GPIO_Port, LCD_D1_Pin};

static void Pin_Output(const TouchPin_t *touch_pin, GPIO_PinState level)
{
	GPIO_InitTypeDef _init = {0};
	HAL_GPIO_WritePin(touch_pin->port, touch_pin->pin, level);
	_init.Pin = touch_pin->pin;
	_init.Mode = GPIO_MODE_OUTPUT_PP;
	_init.Pull = GPIO_NOPULL;
	_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(touch_pin->port, &_init);
}

/*
 * Pinul intra in stare de floating si nu influenteaza folia
 * Intrare fara pull
 */
static void Pin_Floating(const TouchPin_t *touch_pin)
{
	GPIO_InitTypeDef _init = {0};
	_init.Pin = touch_pin->pin;
	_init.Mode = GPIO_MODE_INPUT;
	_init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(touch_pin->port, &_init);
}

static void Pin_Analog(const TouchPin_t *touch_pin)
{
	GPIO_InitTypeDef _init = {0};
	_init.Pin = touch_pin->pin;
	_init.Mode = GPIO_MODE_ANALOG;
	_init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(touch_pin->port, &_init);
}

static void Restore_Lcd_Pins()
{
	Pin_Output(&xp, GPIO_PIN_SET);
	Pin_Output(&xm, GPIO_PIN_SET);
	Pin_Output(&yp, GPIO_PIN_SET);
	Pin_Output(&ym, GPIO_PIN_SET);
}

static uint16_t Read_Adc(uint32_t channel)
{
	ADC_ChannelConfTypeDef _config = {0};
	_config.Channel = channel;
	_config.Rank = 1;
	_config.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &_config);

	uint32_t _sum = 0;
	for (uint8_t sample = 0; sample <= TOUCH_ADC_SAMPLES; sample++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, TOUCH_ADC_TIMEOUT_MS);

		uint32_t _value = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);

		if (sample > 0)
		{
			_sum += _value;
		}
	}
	return (_sum / TOUCH_ADC_SAMPLES);
}

/*
 * Se aplica tensiune pe folia X cu XP HIGH si XM LOW
 * Se masoara tensiunea pe YP pentru a afla pozitia X
 */
uint16_t Touch_IO_Read_X()
{
	Pin_Output(&xp, GPIO_PIN_SET);
	Pin_Output(&xm, GPIO_PIN_RESET);
	Pin_Floating(&ym);
	Pin_Analog(&yp);
	HAL_Delay(1);

	uint16_t _value = Read_Adc(TOUCH_YP_ADC_CHANNEL);
	Restore_Lcd_Pins();
	return _value;
}

/*
 * Se aplica tensiune pe folia Y cu YP HIGH si YM LOW
 * Se masoara tensiunea pe XM pentru a afla pozitia Y
 */
uint16_t Touch_IO_Read_Y()
{
	Pin_Output(&yp, GPIO_PIN_SET);
	Pin_Output(&ym, GPIO_PIN_RESET);
	Pin_Floating(&xp);
	Pin_Analog(&xm);
	HAL_Delay(1);

	uint16_t _value = Read_Adc(TOUCH_XM_ADC_CHANNEL);
	Restore_Lcd_Pins();
	return _value;
}

/*
 * XP = LOW
 * YM = HIGH
 * Se masoara XM si YP => FARA touch: XM = 0 ; YP = maxim => presiune = 0
 * 						  CU touch: la contact diferenta scade, iar presiunea devine mare
 */
uint16_t Touch_IO_Read_Pressure()
{
	Pin_Output(&xp, GPIO_PIN_RESET);
	Pin_Output(&ym, GPIO_PIN_SET);
	Pin_Analog(&xm);
	Pin_Analog(&yp);
	HAL_Delay(1);

	int32_t _z1 = Read_Adc(TOUCH_XM_ADC_CHANNEL);
	int32_t _z2 = Read_Adc(TOUCH_YP_ADC_CHANNEL);
	Restore_Lcd_Pins();

	int32_t _pressure = TOUCH_IO_ADC_MAX - (_z2 - _z1);
	if (_pressure < 0)
	{
		_pressure = 0;
	}
	if (_pressure > TOUCH_IO_ADC_MAX)
	{
		_pressure = TOUCH_IO_ADC_MAX;
	}
	return (uint16_t)_pressure;
}
