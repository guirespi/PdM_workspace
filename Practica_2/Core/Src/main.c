/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx_nucleo_144.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
	tick_t startTime; /* Start time in milliseconds since MCU boot */
	tick_t duration; /* Delay duration in milliseconds */
	bool_t running; /* Delay is running */
} delay_t;

typedef struct {
	bool init; /* Sets if the object is initialized by the api function */
	bool cfg; /* Sets if the object is configured */
	Led_TypeDef led_target; /* Led to be used */
	uint8_t cycles_nbr; /* Number of cycles active */
	uint8_t duty_cycle; /* Duty cycle in percentage*/
	uint32_t cycle; /* Cycle duration or period in milliseconds */
} led_pwm_t;

typedef struct {
	uint32_t cycle;
	uint8_t cycles_nbr;
	uint8_t duty_cycle;
} led_pwm_cfg_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define DECLARE_LED_PWM_PTR_CFG(cycle, cycles_nbr, duty_cycle) (&((led_pwm_cfg_t ){cycle,cycles_nbr,duty_cycle}))

#define LED_PWM_COMMON_DUTY_CYCLE	(50)
#define LED_PWM_COMMON_CYCLE_NBR	(5)
#define LED_PWM_1S_CYCLE	(1000)
#define LED_PWM_200MS_CYCLE	(200)
#define LED_PWM_100MS_CYCLE	(100)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

/* Array terminated in NULL as guardian */
static led_pwm_cfg_t *led_cfg_array[] = {
		DECLARE_LED_PWM_PTR_CFG(LED_PWM_1S_CYCLE, 		LED_PWM_COMMON_CYCLE_NBR, 	LED_PWM_COMMON_DUTY_CYCLE),
		DECLARE_LED_PWM_PTR_CFG(LED_PWM_200MS_CYCLE, 	LED_PWM_COMMON_CYCLE_NBR, 	LED_PWM_COMMON_DUTY_CYCLE),
		DECLARE_LED_PWM_PTR_CFG(LED_PWM_100MS_CYCLE, 	LED_PWM_COMMON_CYCLE_NBR,	LED_PWM_COMMON_DUTY_CYCLE),
		NULL };

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void delayInit(delay_t *delay, tick_t duration);
bool_t delayRead(delay_t *delay);
void delayWrite(delay_t *delay, tick_t duration);
void led_pwm_init(led_pwm_t *led, Led_TypeDef led_enum);
void led_pwm_set_cfg(led_pwm_t *led, led_pwm_cfg_t *led_pwm_cfg);
void led_pwm_start(led_pwm_t *led);
void led_pwm_start_cfg_array(led_pwm_t *led, led_pwm_cfg_t **led_pwm_cfgs);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Non-blocking delay implementation */

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

/* Led-pwm implementation */

void led_pwm_init(led_pwm_t *led, Led_TypeDef led_enum) {
	if (!led)
		return;

	led->led_target = led_enum;
	led->init = true;
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

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();
	/* USER CODE BEGIN SysInit */
	BSP_LED_Init(LED1);
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	/* USER CODE BEGIN 2 */
	led_pwm_t led1 = { 0 };
	led_pwm_init(&led1, LED1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		led_pwm_start_cfg_array(&led1, led_cfg_array);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
