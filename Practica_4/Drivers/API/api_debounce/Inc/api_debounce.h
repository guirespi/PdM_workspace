/*
 * api_debounce.h
 *
 *  Created on: Jul 11, 2024
 *      Author: guirespi
 */

#ifndef API_API_DEBOUNCE_INC_API_DEBOUNCE_H_
#define API_API_DEBOUNCE_INC_API_DEBOUNCE_H_

#include "api_delay.h"

/**
 * @brief Start finite state machine (FSM) for debounce the BUTTON_USER gpio.
 *
 */
void debounceFSM_init(void);
/**
 * @brief Update the state of the FSM for debounce the BUTTON_USER gpio.
 *
 */
void debounceFSM_update(void);
/**
 * @brief Read if the button was pressed recently.
 *
 * @return true: The button was pressed recently. false: The button was not pressed recently.
 */
bool_t readKey(void);

#endif /* API_API_DEBOUNCE_INC_API_DEBOUNCE_H_ */
