/*
 * api_delay.h
 *
 *  Created on: Jul 4, 2024
 *      Author: guirespi
 */

#ifndef API_API_DELAY_INC_API_DELAY_H_
#define API_API_DELAY_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
	tick_t startTime; /* Start time in milliseconds since MCU boot */
	tick_t duration; /* Delay duration in milliseconds */
	bool_t running; /* Delay is running */
} delay_t;

/**
 * @brief Init a delay object.
 *
 * @param delay Delay object valid pointer.
 * @param duration Duration in milliseconds for the delay.
 */
void delayInit(delay_t *delay, tick_t duration);
/**
 * @brief Read if delay object is completed. This functions also runs a delay if
 * it was not running and successive calls checks if the configured delay is done.
 *
 * @param delay Delay object valid and previous initialized pointer.
 * @return bool_t true: Delay already accomplished. false: Delay not accomplished yet.
 */
bool_t delayRead(delay_t *delay);
/**
 * @brief Change a Delay's object duration.
 *
 * @param delay Delay object valid pointer.
 * @param duration New duration in milliseconds.
 */
void delayWrite(delay_t *delay, tick_t duration);
/**
 * @brief Start a delay. This function returns after the delay is over.
 *
 * @param duration Duration of delay.
 */
void delayTask(tick_t duration);
/**
 * @brief Return is the delay is running.
 *
 * @param delay Delay object valid pointer.
 * @return	true: Delay is running. false: Delay is not running.
 */
bool_t delayIsRunning(delay_t * delay);

#endif /* API_API_DELAY_INC_API_DELAY_H_ */
