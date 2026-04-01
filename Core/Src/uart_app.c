/*
 * uart_app.c
 *
 *  Created on: Apr 1, 2026
 *      Author: Antarikchya
 */
#include "uart_app.h"
#include <string.h>

void UartApp_Poll(UART_HandleTypeDef *huart_rx, UART_HandleTypeDef *huart_tx)
{
    uint8_t rx_buffer[5];

    if (HAL_UART_Receive(huart_rx, rx_buffer, 5, HAL_MAX_DELAY) == HAL_OK)
    {

        if (strncmp((char*)rx_buffer, "Hello", 5) == 0)
        {

            char *msg = "\r\nHello received from FTDI module!\r\n";


            HAL_UART_Transmit(huart_tx, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        }
    }
}
