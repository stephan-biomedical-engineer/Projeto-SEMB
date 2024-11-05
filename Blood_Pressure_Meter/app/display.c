/*
 * display.c
 *
 *  Created on: Oct 30, 2024
 *      Author: stephan
 */

// display.c
#include "display.h"

// Handle I2C
I2C_HandleTypeDef *hi2c_LCD;

void LCD_SendByte(uint8_t data, uint8_t mode)
{
    // Prepare os dados para envio
    uint8_t upperNibble = (data & 0xF0) | mode; // Máscara para o nibble alto
    uint8_t lowerNibble = ((data << 4) & 0xF0) | mode; // Máscara para o nibble baixo

    // Habilita o pino Enable
    HAL_GPIO_WritePin(LCD_ENABLE_GPIO, LCD_ENABLE_PIN, GPIO_PIN_SET);
    HAL_I2C_Master_Transmit(hi2c_LCD, LCD_ADDR, &upperNibble, 1, HAL_MAX_DELAY); // Envia o nibble alto
    HAL_GPIO_WritePin(LCD_ENABLE_GPIO, LCD_ENABLE_PIN, GPIO_PIN_RESET);

    // Habilita o pino Enable
    HAL_GPIO_WritePin(LCD_ENABLE_GPIO, LCD_ENABLE_PIN, GPIO_PIN_SET);
    HAL_I2C_Master_Transmit(hi2c_LCD, LCD_ADDR, &lowerNibble, 1, HAL_MAX_DELAY); // Envia o nibble baixo
    HAL_GPIO_WritePin(LCD_ENABLE_GPIO, LCD_ENABLE_PIN, GPIO_PIN_RESET);

    HAL_Delay(1); // Delay para garantir que o comando seja processado
}

void LCD_Init(I2C_HandleTypeDef *hi2c)
{
    hi2c_LCD = hi2c; // Armazena o handle I2C

    // Inicializa o LCD
    HAL_Delay(20); // Espera 20ms após a alimentação
    LCD_SendByte(0x33, 0); // Inicializa o modo de 4 bits
    LCD_SendByte(0x32, 0); // Define modo de 4 bits
    LCD_SendByte(0x28, 0); // Configura 2 linhas e 5x8 matrix
    LCD_SendByte(0x0C, 0); // Liga o display e desativa o cursor
    LCD_SendByte(0x06, 0); // Define o modo de incremento do cursor
    LCD_Clear(); // Limpa o LCD
}

void LCD_Clear(void)
{
    LCD_SendByte(0x01, 0); // Comando para limpar o LCD
    HAL_Delay(2); // Espera 2ms para que o comando seja processado
}

void LCD_SetCursor(uint8_t col, uint8_t row)
{
    uint8_t address;

    // Calcula o endereço do cursor baseado na linha e coluna
    if (row == 0)
        address = 0x80 + col; // Endereço da linha 1
    else if (row == 1)
        address = 0xC0 + col; // Endereço da linha 2
    else
        return; // Linha inválida

    LCD_SendByte(address, 0); // Envia o endereço do cursor
}

void LCD_WriteString(char *str)
{
    while (*str)
    {
        LCD_SendByte(*str++, 1); // Envia cada caractere em modo de dados
    }
}
