/*
 * api_uart.c
 *
 *  Created on: Jul 25, 2024
 *      Author: guirespi
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "api_uart.h"
#include <stm32f4xx_nucleo_144.h>

#define API_UART_BAUDRATE	(115200)
#define API_UART_SEND_LINE_BUFFER (128)
#define API_UART_DEFAULT_WRITE_TIMEOUT (100)

static UART_HandleTypeDef UartHandle;

bool uartInit(void)
{
	bool rt = false;

	UartHandle.Instance = USART3;

	UartHandle.Init.BaudRate = API_UART_BAUDRATE;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&UartHandle) == HAL_OK)
		rt = true;

	return rt;
}

void uartPrintConfig(void)
{
	uartSendLine("UART configuration");
	uartSendLine("%20s: %10u", "Baudrate",UartHandle.Init.BaudRate);
	uartSendLine("%20s: %10s (Including parity bit)", "Word length", UartHandle.Init.WordLength == UART_WORDLENGTH_8B?"8 bit":"9 bit");
	uartSendLine("%20s: %10s", "Stop bits", UartHandle.Init.StopBits == UART_STOPBITS_1?"1 bit":"2 bit");
	uartSendLine("%20s: %10s", "Parity",UartHandle.Init.Parity == UART_PARITY_NONE? "none":(UartHandle.Init.Parity == UART_PARITY_EVEN?"even":"odd"));
}

int uartSendLine(char * format, ...)
{
	char buffer[API_UART_SEND_LINE_BUFFER] = {0};
	va_list args;

	va_start(args, format);
	vsprintf(buffer, format, args);
	strcat(buffer, "\r\n");
	va_end(args);

	return  uartSendString((uint8_t *) buffer);
}

int uartSendString(uint8_t * pstring)
{
	if(pstring == NULL) return -1;

	char * _temp = (char *) pstring;
	uint16_t _temp_length =  (uint16_t)strlen(_temp);
	if(_temp_length == 0) return -1;

	return HAL_UART_Transmit(&UartHandle, (uint8_t *)_temp, _temp_length, API_UART_DEFAULT_WRITE_TIMEOUT);

}

int uartSendStringSize(uint8_t * pstring, uint16_t size)
{
	if(pstring == NULL || size == 0) return -1;

	return HAL_UART_Transmit(&UartHandle, pstring, size, API_UART_DEFAULT_WRITE_TIMEOUT);
}

int uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
	if(pstring == NULL || size == 0) return -1;

	return HAL_UART_Receive(&UartHandle, pstring, size, 0);
}
