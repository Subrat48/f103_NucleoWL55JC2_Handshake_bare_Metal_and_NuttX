/*
 * uart_app.c
 *
 * Created on: Apr 1, 2026
 * Author: Antarikchya
 */
#include "uart_app.h"
#include <string.h>
#include <stdio.h>
#define HS_CMD_LEN 4
const uint8_t expected_hs_cmd[HS_CMD_LEN] = { 0xaa, 0xbb, 0xcc, 0xdd };

uint8_t rx_buffer[HS_CMD_LEN] = { 0 };

/*

 OLD CODE

 void UartApp_Handshake() {
 uint8_t rx_buffer[HS_CMD_LEN] = { 0 };

 if (HAL_UART_Receive(&huart1, rx_buffer, HS_CMD_LEN, HAL_MAX_DELAY)
 == HAL_OK) {

 // for(int i = 0; i < sizeof(rx_buffer); i++){
 // 	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
 // }

 if (memcmp(rx_buffer, expected_hs_cmd, HS_CMD_LEN) == 0) {
 HAL_UART_Transmit(&huart1, (uint8_t*) expected_hs_cmd, HS_CMD_LEN,
 HAL_MAX_DELAY);

 char *msg = "\r\n[NUCLEO] Handshake requested and acknowledged!\r\n";
 HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
 HAL_MAX_DELAY);
 hsFLAG = 1;
 } else {
 char *msg = "\r\n[NUCLEO] Unknown bytes received from Bluepill.\r\n";
 HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
 HAL_MAX_DELAY);
 hsFLAG = 0;
 }
 }
 }
 */

void UartApp_Handshake_IT_Start(void) {
	HAL_UART_Receive_IT(&huart1, rx_buffer, HS_CMD_LEN);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {

		char debug_msg[100];
		sprintf(debug_msg,
				"\r\n[NUCLEO] Received Command: %02X %02X %02X %02X\r\n",
				rx_buffer[0], rx_buffer[1], rx_buffer[2], rx_buffer[3]);
		HAL_UART_Transmit(&huart2, (uint8_t*) debug_msg, strlen(debug_msg),
		HAL_MAX_DELAY);

		if (memcmp(rx_buffer, expected_hs_cmd, HS_CMD_LEN) == 0) {

			HAL_UART_Transmit(&huart1, (uint8_t*) expected_hs_cmd, HS_CMD_LEN,
			HAL_MAX_DELAY);

			char *msg =
					"[NUCLEO] The command has been received! Handshake Successful!\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
			HAL_MAX_DELAY);

			hsFLAG = 1;
		} else {
			char *msg = "[NUCLEO] Unknown command received. Retrying...\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
			HAL_MAX_DELAY);

			hsFLAG = 0;

			HAL_UART_Receive_IT(&huart1, rx_buffer, HS_CMD_LEN);
		}
	}
}

void new_hs_operation() {
	myDebug("\nwait to rx:\n");
	if (HAL_UART_Receive(&huart1, rx_buffer, sizeof(rx_buffer), 7000)
			== HAL_OK) {
		for (int i = 0; i < sizeof(rx_buffer); i++) {
			myDebug("%02x", rx_buffer[i]);
		}

		if (memcmp(rx_buffer, expected_hs_cmd, HS_CMD_LEN) == 0) {

			myDebug("HS rx correct");

			HAL_UART_Transmit(&huart1, (uint8_t*) expected_hs_cmd, HS_CMD_LEN,
			HAL_MAX_DELAY);
			hsFLAG = 1;
		} else {
			myDebug("\nHS rx in-correct");
			hsFLAG = 0;
		}
	} else {
		new_hs_operation();
	}
}
