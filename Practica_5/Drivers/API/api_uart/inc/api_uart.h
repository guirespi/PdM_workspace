/*
 * api_uart.h
 *
 *  Created on: Jul 25, 2024
 *      Author: guirespi
 */

#ifndef API_API_UART_INC_API_UART_H_
#define API_API_UART_INC_API_UART_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize UART.
 *
 * @return true: Initialize ok. false: Error.
 */
bool uartInit(void);
/**
 * @brief Print previous initialized UART configuration.
 *
 */
void uartPrintConfig(void);
/**
 * @brief Send a line through UART.
 *
 * @note '\r\n' is append into format.
 *
 * @param format Line to print.
 * @return
 * 			- HAL_OK if no error.
 */
int uartSendLine(char * format, ...);
/**
 * @brief Send string through UART.
 *
 * @param pstring String to send.
 * @return
 * 			- HAL_OK if no error.
 */
int uartSendString(uint8_t * pstring);
/**
 * @brief Send string until size through UART.
 *
 * @param pstring String to send.
 * @param size Limit to send.
 * @return
 * 			- HAL_OK if no error.
 */
int uartSendStringSize(uint8_t * pstring, uint16_t size);
/**
 * @brief Receive string until size through UART.
 *
 * @param pstring Buffer to receive string.
 * @param size Size to be received.
 * @return
 * 			- HAL_OK if no error.
 */
int uartReceiveStringSize(uint8_t * pstring, uint16_t size);

#endif /* API_API_UART_INC_API_UART_H_ */
