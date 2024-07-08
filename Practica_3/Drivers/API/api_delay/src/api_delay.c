/*
 * api_delay.c
 *
 *  Created on: Jul 4, 2024
 *      Author: guirespi
 */
#include <stdio.h>
#include "api_delay.h"
#include "stm32f4xx_hal.h"

void delayInit(delay_t *delay, tick_t duration) {
	if (delay == NULL)
		return;
	delay->running = false;
	delay->duration = duration;
}

bool_t delayRead(delay_t *delay) {
	if (delay == NULL)
		return false;

	bool_t rt = false;

	if (delay->running == false) {
		delay->startTime = HAL_GetTick();
		delay->running = true;
	} else {
		rt = (HAL_GetTick() - delay->startTime) > delay->duration;
		if (rt == true)
			delay->running = false;
	}
	return rt;
}

void delayWrite(delay_t *delay, tick_t duration) {
	if (delay == NULL)
		return;

	if (delay->running == true)
		delay->running = false;
	delay->startTime = 0;
	delay->duration = duration;
}

void delayTask(tick_t duration) {
	delay_t delay = { 0 };
	delayInit(&delay, duration);
	while (!delayRead(&delay))
		;
}

bool_t delayIsRunning(delay_t * delay)
{
	bool_t rt =  delay->running;
	return rt;
}
