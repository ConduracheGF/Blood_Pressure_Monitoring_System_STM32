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
#include "Utils/Header/lcd.h"
#include "IO/Header/touchscreen_io.h"

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

volatile TouchState_t touchState = TOUCH_STATE_IDLE;
volatile uint16_t adcValX = 0, adcValY = 0, adcValZ = 0;
volatile bool adcConversionDone = false;

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


// ISR de finalizare a conversiei ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1)
	{
		adcConversionDone = true;
	}
}

void Touch_IO_Process()
{
	if (LCD_IsLocked())	return;

	ADC_ChannelConfTypeDef config = {0};
	config.Rank = 1;
	config.SamplingTime = ADC_SAMPLETIME_144CYCLES;

	switch(touchState) {
		case TOUCH_STATE_IDLE:
		case TOUCH_STATE_DATA_READY:
			break;
		/*
		 * XP = LOW
		 * YM = HIGH
		 * Se masoara XM si YP => FARA touch: XM = 0 ; YP = maxim => presiune = 0
		 * 						  CU touch: la contact diferenta scade, iar presiunea devine mare
		 */
		case TOUCH_STATE_PREP_Z:
			Pin_Output(&xp, GPIO_PIN_RESET);
			Pin_Output(&ym, GPIO_PIN_SET);
			Pin_Analog(&xm);
			Pin_Analog(&yp);

			config.Channel = TOUCH_XM_ADC_CHANNEL;
			HAL_ADC_ConfigChannel(&hadc1, &config);
			adcConversionDone = false;
			HAL_ADC_Start_IT(&hadc1);
			touchState = TOUCH_STATE_WAIT_Z;
			break;
		case TOUCH_STATE_WAIT_Z:
			if (adcConversionDone){
				adcValZ = HAL_ADC_GetValue(&hadc1);
				touchState = TOUCH_STATE_PREP_X;
			}
			break;
		/*
		 * Se aplica tensiune pe folia X cu XP HIGH si XM LOW
		 * Se masoara tensiunea pe YP pentru a afla pozitia X
		 */
		case TOUCH_STATE_PREP_X:
			Pin_Output(&xp, GPIO_PIN_SET);
			Pin_Output(&xm, GPIO_PIN_RESET);
			Pin_Floating(&ym);
			Pin_Analog(&yp);

			config.Channel = TOUCH_YP_ADC_CHANNEL;
			HAL_ADC_ConfigChannel(&hadc1, &config);

			adcConversionDone = false;
			HAL_ADC_Start_IT(&hadc1);
			touchState = TOUCH_STATE_WAIT_X;
			break;
		case TOUCH_STATE_WAIT_X:
			if (adcConversionDone) {
				adcValX = HAL_ADC_GetValue(&hadc1);
				touchState = TOUCH_STATE_PREP_Y;
			}
			break;

		/*
		 * Se aplica tensiune pe folia Y cu YP HIGH si YM LOW
		 * Se masoara tensiunea pe XM pentru a afla pozitia Y
		 */
		case TOUCH_STATE_PREP_Y:
			Pin_Output(&yp, GPIO_PIN_SET);
			Pin_Output(&ym, GPIO_PIN_RESET);
			Pin_Floating(&xp);
			Pin_Analog(&xm);

			config.Channel = TOUCH_XM_ADC_CHANNEL;
			HAL_ADC_ConfigChannel(&hadc1, &config);

			adcConversionDone = false;
			HAL_ADC_Start_IT(&hadc1);
			touchState = TOUCH_STATE_WAIT_Y;
			break;
		case TOUCH_STATE_WAIT_Y:
			if (adcConversionDone){
				adcValY = HAL_ADC_GetValue(&hadc1);
				Restore_Lcd_Pins();
				touchState = TOUCH_STATE_DATA_READY;
			}
			break;
		default:
			break;
	}
}

uint16_t Touch_IO_Read_X()
{
	return adcValX;
}

uint16_t Touch_IO_Read_Y()
{
	return adcValY;
}

// Calcul de presiune estimativ
uint16_t Touch_IO_Read_Pressure()
{
	if (adcValZ < 4050)
		return (TOUCH_IO_ADC_MAX - adcValZ);
	return 0;
}
