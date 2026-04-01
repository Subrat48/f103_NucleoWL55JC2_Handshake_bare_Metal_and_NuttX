/*
 * uart_app.h
 *
 *  Created on: Apr 1, 2026
 *      Author: Antarikchya
 */
#ifndef UART_APP_H
#define UART_APP_H

#include "main.h"

void UartApp_Poll(UART_HandleTypeDef *huart_rx, UART_HandleTypeDef *huart_tx);

#endif /* UART_APP_H */
