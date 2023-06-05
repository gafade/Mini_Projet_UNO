/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct {
	uint16_t playerID;
	uint16_t couleur;
	uint16_t valeur;
	uint16_t prochain;//normal 1,si passe tour =2, inversion -1
	uint16_t uno;//juste si on a un UN0, au cas ou/msg erreur

	// A FINIR
}Message;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BP2_Pin GPIO_PIN_4
#define BP2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOI
#define BP3_Pin GPIO_PIN_7
#define BP3_GPIO_Port GPIOG
#define BP1_Pin GPIO_PIN_6
#define BP1_GPIO_Port GPIOG
#define POTD_Pin GPIO_PIN_10
#define POTD_GPIO_Port GPIOF
#define POTG_Pin GPIO_PIN_0
#define POTG_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
