/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define BTN0_Pin GPIO_PIN_0
#define BTN0_GPIO_Port GPIOC
#define BTN1_Pin GPIO_PIN_1
#define BTN1_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_2
#define BTN2_GPIO_Port GPIOC
#define BTN3_Pin GPIO_PIN_3
#define BTN3_GPIO_Port GPIOC
#define SERVO_Pin GPIO_PIN_1
#define SERVO_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_6
#define BUZZER_GPIO_Port GPIOA
#define ROW1_KEYPAD_Pin GPIO_PIN_4
#define ROW1_KEYPAD_GPIO_Port GPIOC
#define ROW2_KEYPAD_Pin GPIO_PIN_5
#define ROW2_KEYPAD_GPIO_Port GPIOC
#define LCD_1602_RS_Pin GPIO_PIN_0
#define LCD_1602_RS_GPIO_Port GPIOB
#define LCD_1602_EN_Pin GPIO_PIN_1
#define LCD_1602_EN_GPIO_Port GPIOB
#define LCD_1602_D3_Pin GPIO_PIN_2
#define LCD_1602_D3_GPIO_Port GPIOB
#define CLK_74HC595_Pin GPIO_PIN_10
#define CLK_74HC595_GPIO_Port GPIOB
#define LATCH_74HC595_Pin GPIO_PIN_13
#define LATCH_74HC595_GPIO_Port GPIOB
#define SER_74HC595_Pin GPIO_PIN_15
#define SER_74HC595_GPIO_Port GPIOB
#define ROW3_KEYPAD_Pin GPIO_PIN_6
#define ROW3_KEYPAD_GPIO_Port GPIOC
#define ROW4_KEYPAD_Pin GPIO_PIN_7
#define ROW4_KEYPAD_GPIO_Port GPIOC
#define COL1_KEYPAD_Pin GPIO_PIN_8
#define COL1_KEYPAD_GPIO_Port GPIOC
#define COL2_KEYPAD_Pin GPIO_PIN_9
#define COL2_KEYPAD_GPIO_Port GPIOC
#define CE_DS1302_Pin GPIO_PIN_10
#define CE_DS1302_GPIO_Port GPIOA
#define IO_DS1302_Pin GPIO_PIN_11
#define IO_DS1302_GPIO_Port GPIOA
#define CLK_DS1302_Pin GPIO_PIN_12
#define CLK_DS1302_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define COL3_KEYPAD_Pin GPIO_PIN_10
#define COL3_KEYPAD_GPIO_Port GPIOC
#define COL4_KEYPAD_Pin GPIO_PIN_11
#define COL4_KEYPAD_GPIO_Port GPIOC
#define LCD_1602_D4_Pin GPIO_PIN_3
#define LCD_1602_D4_GPIO_Port GPIOB
#define LCD_1602_D5_Pin GPIO_PIN_4
#define LCD_1602_D5_GPIO_Port GPIOB
#define LCD_1602_D6_Pin GPIO_PIN_5
#define LCD_1602_D6_GPIO_Port GPIOB
#define LCD_1602_D7_Pin GPIO_PIN_6
#define LCD_1602_D7_GPIO_Port GPIOB
#define BMP_SCL_Pin GPIO_PIN_8
#define BMP_SCL_GPIO_Port GPIOB
#define BMP_SDA_Pin GPIO_PIN_9
#define BMP_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef struct print_option
{
	uint8_t p_rtc;  // 0 1
	uint8_t p_led;
} t_print;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
