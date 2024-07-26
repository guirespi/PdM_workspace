/*
 * api_debounce.c
 *
 *  Created on: Jul 11, 2024
 *      Author: guirespi
 */
#include "api_debounce.h"
#include <stm32f4xx_nucleo_144.h>

#define DEBOUNCE_TIME_DELAY (40)

static debounceState_t debounce_state =  BUTTON_UP;
static bool_t button_pressed = false;
static delay_t debounce_delay = {0};

void debounceFSM_init(void)
{
	delayInit(&debounce_delay, DEBOUNCE_TIME_DELAY);
	debounce_state = BUTTON_UP;
	button_pressed = false;
}

void debounceFSM_update(void)
{
	switch(debounce_state)
	{
		case BUTTON_UP:
		{
			if(BSP_PB_GetState(BUTTON_USER))
			{
				debounce_state = BUTTON_FAILLING;
				delayInit(&debounce_delay, DEBOUNCE_TIME_DELAY);
			}
			break;
		}
		case BUTTON_FAILLING:
		{
			if(delayRead(&debounce_delay))
			{
				if(!BSP_PB_GetState(BUTTON_USER))
				{
					debounce_state = BUTTON_UP;
				}
				else
				{
					button_pressed = true;
					debounce_state = BUTTON_DOWN;
				}
			}
			break;
		}
		case BUTTON_DOWN:
		{
			if(!BSP_PB_GetState(BUTTON_USER))
			{
				debounce_state = BUTTON_RAISING;
				delayInit(&debounce_delay, DEBOUNCE_TIME_DELAY);
			}
			break;
		}
		case BUTTON_RAISING:
		{
			if(delayRead(&debounce_delay))
			{
				if(BSP_PB_GetState(BUTTON_USER))
				{
					debounce_state = BUTTON_DOWN;
				}
				else
				{
					button_pressed = false;
					debounce_state = BUTTON_UP;
				}
			}
			break;
		}
	}
}

bool_t readKey(void)
{
	bool_t rt = button_pressed;
	if(button_pressed == true)
		button_pressed = false;
	return rt;
}

debounceState_t debounceFSM_get_state(void)
{
	return debounce_state;
}

