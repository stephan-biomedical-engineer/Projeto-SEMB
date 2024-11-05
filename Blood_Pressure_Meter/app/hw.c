/*
 * hw.c
 *
 *  Created on: Oct 26, 2024
 *      Author: stephan
 */
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "app.h"

extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef *hi2c;
volatile char msg[20];
volatile uint16_t rawValue;
volatile float value;
const float OFFSET_VOLTAGE = -2.5 / 1000.0; // Offset em Volts
const float SPAN_VOLTAGE = 42.0 / 1000.0;   // Span em Volts
const float MAX_PRESSURE_KPA = 50.0;         // Pressão máxima em kPa
const float MIN_PRESSURE_KPA = -50.0;        // Pressão mínima em kPa
const float KPA_TO_MMHG = 7.50062;


float analogRead(void)
{
    if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
    {
        rawValue = HAL_ADC_GetValue(&hadc1);
        float value = ((float)rawValue / 4095) * 3300;
        return value;
    }
    else
    {
        return -1.0;
    }
}

bool buttonState(void)
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void rotateMotor(void)
{
	HAL_GPIO_WritePin(GPIOA, 8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, 9, GPIO_PIN_RESET);
}

void stopMotor(void)
{
	HAL_GPIO_WritePin(GPIOA, 8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, 9, GPIO_PIN_RESET);
}

void valveActivate(void)
{
	HAL_GPIO_WritePin(GPIOB, 15, GPIO_PIN_SET);
}

void valveDeactivate(void)
{
	HAL_GPIO_WritePin(GPIOB, 15, GPIO_PIN_RESET);

}

float computePressure(float pressureVoltage)
{
	float pressureKPA = ((pressureVoltage - OFFSET_VOLTAGE) / SPAN_VOLTAGE) * (MAX_PRESSURE_KPA - MIN_PRESSURE_KPA) + MIN_PRESSURE_KPA;
	float pressuremmHg = pressureKPA * KPA_TO_MMHG;
	return pressuremmHg;
}



