/*
 * app.c
 *
 *  Created on: Oct 25, 2024
 *      Author: stephan
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "app.h"
#include "main.h"
#include "hw.h"
#include "display.h"

extern ADC_HandleTypeDef hadc1;
volatile float sensor;
volatile float pression;
bool begin = false;
bool flag = true;

void app_init(void)
{
	HAL_ADC_Start(&hadc1);
	stopMotor();
	valveDeactivate();
}

void app_loop(void)
{
    begin = buttonState();

    if (begin && flag)
    {
        // Exibe "Iniciando..." por 5 segundos
        LCD_Clear();
        LCD_SetCursor(0, 0);
        LCD_WriteString("Iniciando...");
        HAL_Delay(5000);

        // Encher o manguito
        rotateMotor();

        while (true)
        {
            sensor = analogRead(); // Lê o valor do sensor
            pression = computePressure(sensor); // Converte para mmHg

            // Exibe a pressão atual no display
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_WriteString("Pressao: ");
            char pressureStr[10];
            snprintf(pressureStr, sizeof(pressureStr), "%.2f", pression);
            LCD_WriteString(pressureStr);
            HAL_Delay(100); // Atualiza o display a cada 100 ms

            if (pression >= 170) // Limite de pressão
            {
                stopMotor(); // Para a bomba

                // Exibe a pressão final e aguarda 10 segundos
                LCD_Clear();
                LCD_SetCursor(0, 0);
                LCD_WriteString("Pressao final:");
                LCD_SetCursor(0, 1);
                LCD_WriteString(pressureStr);
                HAL_Delay(10000); // Espera 10 segundos

                // Ativa o buzzer (três toques)
                for (int i = 0; i < 3; i++)
                {
                    HAL_GPIO_WritePin(GPIOB, 14, GPIO_PIN_SET);
                    HAL_Delay(200);
                    HAL_GPIO_WritePin(GPIOB, 14, GPIO_PIN_RESET);
                    HAL_Delay(200);
                }

                // Ativa e desativa a válvula solenoide para esvaziar o manguito
                valveActivate();
                HAL_Delay(5000); // Aguarda 5 segundos para esvaziar
                valveDeactivate();

                flag = false;
                break; // Sai do loop de enchimento
            }
        }
    }
}
