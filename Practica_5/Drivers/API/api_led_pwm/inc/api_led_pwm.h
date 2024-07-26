/*
 * api_led_pwm.h
 *
 *  Created on: Jul 4, 2024
 *      Author: guirespi
 */

#ifndef API_API_LED_PWM_INC_API_LED_PWM_H_
#define API_API_LED_PWM_INC_API_LED_PWM_H_

#include "api_delay.h"

#define LED_PWM_DONT_STOP (0xff)

typedef int led_pwm_enum;

#define DECLARE_LED_PWM_PTR_CFG(cycle, cycles_nbr, duty_cycle) (&((led_pwm_cfg_t ){cycle,cycles_nbr,duty_cycle}))

typedef enum __attribute__((packed))
{
	led_state_off = 0, /* Led is off */
	led_state_on, /* Led is on */
	led_state_max /* Guardian of led states */
}led_state_t;

typedef struct {
	bool_t is_op; /* A non blocking operation is running? */
	led_state_t led_state; /* Last led state executed in the non blocking operation */
	delay_t delay_ctrl; /* Delay object to control the delay of the PWM leds */
	uint8_t cycles_ctrl; /* Number of cycles left to end the non blocking operation */
	tick_t time_on; /* Time the leds stays on */
	tick_t time_off; /* Time the leds stays off */
} led_pwm_operation_t;

typedef struct {
	bool init; /* Sets if the object is initialized by the api function */
	bool cfg; /* Sets if the object is configured */
	led_pwm_enum  led_target; /* Led to be used */
	uint8_t cycles_nbr; /* Number of cycles active */
	uint8_t duty_cycle; /* Duty cycle in percentage*/
	tick_t 	cycle; /* Cycle duration or period in milliseconds */
	led_pwm_operation_t led_pwm_op; /* Structure used for non-blocking and poll type operation */
} led_pwm_t;

typedef struct {
	uint32_t cycle; /* Cycle duration or period in milliseconds */
	uint8_t cycles_nbr /* Number of cycles the led must complete. If its value is FF, it would never stop */;
	uint8_t duty_cycle /* Duty cycle */;
} led_pwm_cfg_t;

/**
 * @brief Init a led pwm object-
 *
 * @param led led pwm object valid pointer.
 * @param led_enum led enum of the board used.
 */
void led_pwm_init(led_pwm_t *led, led_pwm_enum led_enum);
/**
 * @brief Set a PWM config to a previous initialized led pwm object.
 *
 * @param led led pwm object valid pointer.
 * @param led_pwm_cfg led pwm configuration valid pointer.
 */
void led_pwm_set_cfg(led_pwm_t *led, led_pwm_cfg_t *led_pwm_cfg);
/**
 * @brief Start a previous initialized and configurated led pwm object. This functions blocks the program flow.
 *
 * @param led led pwm object valid pointer.
 */
void led_pwm_start(led_pwm_t *led);
/**
 * @brief Start an array of led pwm configs.
 *
 * @param led led pwm object valid pointer.
 * @param led_pwm_cfgs Array of led pwm configs valid pointers. The array must be NULL terminated. This functions blocks the program flow.
 */
void led_pwm_start_cfg_array(led_pwm_t *led, led_pwm_cfg_t **led_pwm_cfgs);
/**
 * @brief Start non blocking operation for a previously configured PWM led.
 *
 * @param led PWM led object valid pointer.
 */
void led_pwm_start_non_blocking(led_pwm_t * led);
/**
 * @brief Stop a non blocking operation for a previously configured PWM led.
 *
 * @param PWM led object valid pointer;
 */
void led_pwm_stop_non_blocking(led_pwm_t *);
/**
 * @brief Read the status of a non blocking PWM led operation previously started.
 *
 * @note Internally, this function updates the status of the non blocking operation.
 *
 * @param led PWM led object valid pointer.
 * @return true: The non blocking operation's still going. false: The non blocking operation finish.
 */
bool_t led_pwm_read_non_blocking(led_pwm_t * led);
#endif /* API_API_LED_PWM_INC_API_LED_PWM_H_ */
