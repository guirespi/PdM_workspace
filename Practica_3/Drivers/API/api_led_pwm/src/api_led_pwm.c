/*
 * api_led_pwm.c
 *
 *  Created on: Jul 4, 2024
 *      Author: guirespi
 */
#include <string.h>
#include "api_led_pwm.h"
#include <stm32f4xx_nucleo_144.h>

/**
 * @brief Toggle the led state field of a PWM led.
 *
 * @param led PWM led object reference.
 */
static void inline led_pwm_change_state(led_pwm_t * led);
/**
 * @brief Operate a LED PWM based in its state.
 *
 * @param led PWM led object reference.
 */
static void led_pwm_op(led_pwm_t * led);
/**
 * @brief Poll a previously started non blocking operation of a PWM led.
 *
 * @param led PWM led object valid pointer;
 */
static void led_pwm_non_blockin_poll(led_pwm_t * led);

static void inline led_pwm_change_state(led_pwm_t * led)
{
	led->led_pwm_op.led_state = (led->led_pwm_op.led_state + 1)%led_state_max;
}

static void led_pwm_op(led_pwm_t * led)
{
	if(led->led_pwm_op.led_state == led_state_off)
		BSP_LED_Off(led->led_target);
	if(led->led_pwm_op.led_state == led_state_on)
		BSP_LED_On(led->led_target);
}

static void led_pwm_non_blockin_poll(led_pwm_t * led)
{
	if(delayIsRunning(&led->led_pwm_op.delay_ctrl) == false)
	{
		/* We finish an off cycle */
		if(led->led_pwm_op.led_state == led_state_off)
		{
			delayWrite(&led->led_pwm_op.delay_ctrl, led->led_pwm_op.time_on);
			if(led->led_pwm_op.cycles_ctrl > 0)
				led->led_pwm_op.cycles_ctrl--;
		}
		/* We finish an on cycle */
		if(led->led_pwm_op.led_state == led_state_on)
		{
			delayWrite(&led->led_pwm_op.delay_ctrl, led->led_pwm_op.time_off);
		}

		if(led->led_pwm_op.cycles_ctrl == 0)
		{
			/* We finish all the cycles */
			led->led_pwm_op.is_op = false;
			return;
		}

		/* Change to next state */
		led_pwm_change_state(led);
		/* Operate led */
		led_pwm_op(led);
		/* Read delay */
	}
	/* Read delay */
	delayRead(&led->led_pwm_op.delay_ctrl);
}

void led_pwm_init(led_pwm_t *led, led_pwm_enum led_enum) {
	if (!led)
		return;

	led->led_target = led_enum;
	led->init = true;
	led->cfg = false;
	memset(&led->led_pwm_op, 0,sizeof(led->led_pwm_op));
}

void led_pwm_set_cfg(led_pwm_t *led, led_pwm_cfg_t *led_pwm_cfg) {
	if (!led)
		return;
	if (!led->init)
		return;
	if (!led_pwm_cfg)
		return;
	if (led_pwm_cfg->duty_cycle > 100)
		return;

	led->cycle = led_pwm_cfg->cycle;
	led->duty_cycle = led_pwm_cfg->duty_cycle;
	led->cycles_nbr = led_pwm_cfg->cycles_nbr;
	led->cfg = true;
}

void led_pwm_start(led_pwm_t *led) {
	if (!led)
		return;
	if (!led->init || !led->cfg)
		return;

	const uint32_t time_on = (uint32_t) (led->cycle
			* ((float) led->duty_cycle / 100));
	const uint32_t time_off = led->cycle - time_on;
	uint8_t cycles_nbr = led->cycles_nbr;

	while (cycles_nbr--) {
		BSP_LED_On(led->led_target);
		delayTask(time_on);
		BSP_LED_Off(led->led_target);
		delayTask(time_off);
	}
}

void led_pwm_start_non_blocking(led_pwm_t * led)
{
	if(!led)
		return;
	if(!led->init || !led->cfg)
		return;

	if(led->led_pwm_op.is_op == false)
	{
		/* Configuration of operation structure */
		led->led_pwm_op.time_on = (uint32_t) (led->cycle*((float) led->duty_cycle / 100));
		led->led_pwm_op.time_off = led->cycle - led->led_pwm_op.time_on;
		led->led_pwm_op.cycles_ctrl = led->cycles_nbr;
		led->led_pwm_op.led_state = led_state_on;

		/* Start first cycle: On state */
		delayInit(&led->led_pwm_op.delay_ctrl, led->led_pwm_op.time_on);
		/* Operate led */
		led_pwm_op(led);
		/* Read delay for first cycle */
		delayRead(&led->led_pwm_op.delay_ctrl);
		/* Operation status set to true */
		led->led_pwm_op.is_op = true;
	}
}

bool_t led_pwm_read_non_blocking(led_pwm_t * led)
{
	if(!led) return false;
	if(!led->init || !led->cfg)
		return false;

	bool_t rt =  led->led_pwm_op.is_op;

	/* If it is operating, keep polling the operation */
	if(rt == true)
		led_pwm_non_blockin_poll(led);

	return rt;
}

void led_pwm_start_cfg_array(led_pwm_t *led, led_pwm_cfg_t **led_pwm_cfgs) {
	if (!led)
		return;
	if (!led->init)
		return;
	if (led_pwm_cfgs == NULL)
		return;

	while (*led_pwm_cfgs) {
		led_pwm_set_cfg(led, *led_pwm_cfgs);
		led_pwm_start(led);
		led_pwm_cfgs++;
	}
}

