/*
 * uart_app.h
 *
 * Created on: Apr 1, 2026
 * Author: Antarikchya
 */
#ifndef UART_APP_H
#define UART_APP_H

#include "main.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern int hsFLAG;

// --- OLD FUNCTION ---
// void UartApp_Handshake();

// --- NEW FUNCTION ---
void UartApp_Handshake_IT_Start();
void new_hs_operation();
extern void myDebug(const char *fmt, ...);

#endif /* UART_APP_H */
