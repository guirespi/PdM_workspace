/*
 * api_debounce.h
 *
 *  Created on: Jul 11, 2024
 *      Author: guirespi
 */

#ifndef API_API_DEBOUNCE_INC_API_DEBOUNCE_H_
#define API_API_DEBOUNCE_INC_API_DEBOUNCE_H_

typedef enum
{
	BUTTON_UP = 0, /* BUtton is not pressed or is UP */
	BUTTON_FAILLING, /* Button transitional state to be DOWN */
	BUTTON_DOWN, /* Button is pressed or DOWN */
	BUTTON_RAISING, /* Button transitional state to be UP */
}debounceState_t;

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
/**
 * @brief Get the current FSM state for this API.
 *
 * @return Debounce state.
 */
debounceState_t debounceFSM_get_state(void);

#endif /* API_API_DEBOUNCE_INC_API_DEBOUNCE_H_ */
