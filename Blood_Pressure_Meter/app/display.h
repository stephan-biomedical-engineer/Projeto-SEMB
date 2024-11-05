/*
 * display.h
 *
 *  Created on: Oct 30, 2024
 *      Author: stephan
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stm32f4xx_hal.h"

#define LCD_ADDR 0x40 // Endereço I2C com base no datasheet do módulo PCF8574
#define LCD_ENABLE_PIN GPIO_PIN_5 // Pino Enable do módulo I²C
#define LCD_ENABLE_GPIO GPIOB // GPIO para o pino Enable

void LCD_Init(I2C_HandleTypeDef *hi2c);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_WriteString(char *str);

#endif /* DISPLAY_H_ */
