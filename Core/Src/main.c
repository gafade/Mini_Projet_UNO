/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "can.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "rng.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdio.h"
#include "table.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t caractere_recu;
CAN_HandleTypeDef CanHandle;
CAN_TxHeaderTypeDef TxHeader1;
CAN_TxHeaderTypeDef TxHeader2;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData1[4];
uint8_t TxData2[8];
uint8_t RxData[4];
uint32_t TxMailbox;

extern int ID;
//int ID=1; // est la MEME valeur que celui dans freertos.c



// à modifier l'ID
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char text[50]={};
	char text2[50]={};
	static TS_StateTypeDef  TS_State;
	uint32_t potl,potr,joystick_h, joystick_v;
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

	//bus CAN
	uint32_t truc_can;
	CAN_FilterTypeDef sFilterConfig;


//	/*CODE TP1*/
//	char msg[50]={};
//	char recep[50] ;
//	char retour[50]={};
//
//	uint8_t time[3]={};
//	uint8_t date[3]={};
//
//	uint16_t pos_v=130;
//	uint16_t pos_h=240;
//
//	int16_t vit_v=0;
//	int16_t vit_h=0;
//
//
//	//        TP2 variables
//	RTC_TimeTypeDef time1, time2;
//	RTC_DateTypeDef date1, date2;
	uint32_t i,millisecond1,millisecond2,value;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC3_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM12_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_CAN1_Init();
  MX_ADC1_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS+ BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4);
	BSP_LCD_DisplayOn();


	//layer 0 : table background,UNO,pioche,cartes adversaires
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_RED);
	BSP_LCD_DrawBitmap(0,0,(uint8_t*)_actable);


	//layer 1 : carte dessous,playing flag,UNO blink
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(00);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_SetBackColor(00);

	//IL NY A QUE 2 COUCHES / ON DOIT JUSTE
	//pour carte dessus/dessous, afficher l'une puis l'autre sur une meme tache


	//layer 2 : carte dessus,nb joueurs, nb cartes,WIN/LOSE/BEGIN/CHARGEMENT/CONNEXION




	//begin touchscreen
	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	//pour le TP3 la liaision UART interruptions

	//HAL_UART_Receive_IT(&huart1,&caractere_recu,1);

	/* Commencer le bus CAN
	 *
	 *
	 */
	/*##-2- Configure the CAN Filter ###########################################*/
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x000; //ON ECOUTE TOUT
	sFilterConfig.FilterIdLow = 0x000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
		/* Filter configuration Error */
		Error_Handler();
	}

	//IL FAUT QUAND MEME ACTIVER LA RECEPTION
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);



	/*##-3- Start the CAN peripheral ###########################################*/
	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
		/* Start Error */
		Error_Handler();
	}
	TxHeader1.StdId = 0x410;//importance du MESSAGE
	TxHeader1.ExtId = 0x01; //peu importe, on est en ID 11 bits (CAN_ID_STD)
	TxHeader1.RTR = CAN_RTR_DATA;
	TxHeader1.IDE = CAN_ID_STD;
	TxHeader1.DLC = 4;//data length
	TxHeader1.TransmitGlobalTime = DISABLE;

	//on a pas beson d'envoyer 2 choses à la fois pas de Tx2Header


  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



	while (1)
	{
		//   RECEVOIR
		if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData)
				== HAL_OK) {
			//HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);

			sprintf(text, "ID %3X data ", (unsigned int)RxHeader.StdId);

			for (i = 0; i < RxHeader.DLC; i++) {
				sprintf(text2, "%2X ", RxData[i]);
				strcat(text, text2);
			}

			BSP_LCD_DisplayStringAt(10, 255, (uint8_t*) text, LEFT_MODE);
		}

		//     ENVOYER
		TxData1[0] = (2000) >> 8; //valeur de 0 à 3000 en 0.1 km/h (avec offset de -2000)
		TxData1[1] = (2000) & 0xFF; //valeur de 0 à 3000 en 0.1 km/h (avec offset de -2000)
		TxData1[2] = (20000) >> 8; // valeur de -12000 à 12000 en tour/min (avec offset de -20000)
		TxData1[3] = (20000) ;
		HAL_CAN_AddTxMessage(&hcan1, &TxHeader1, TxData1, &truc_can);


		TxData2[0] = 2500; //valeur de 0 à 3000 en 0.1 km/h (avec offset de -2000)
		HAL_CAN_AddTxMessage(&hcan1, &TxHeader2, TxData2, &truc_can);






    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[4];

	char test[50]={};

	extern osMessageQId myQueue02Handle;
	//extern osMessageQId MessagerieBatterieHandle;

	Message coup={1,0,0,0};
	//alumé LED

	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData);
	//if (RxHeader.StdId == 0x410 ) {//égal un autre joueur != notre
		coup.playerID=RxData[0];
		coup.couleur=RxData[1];
		coup.valeur=RxData[2];
		coup.prochain=RxData[3];
		sprintf(test,"x%d",(char) coup.prochain);
		BSP_LCD_DisplayStringAt(-10,70,(uint8_t*) test ,CENTER_MODE);
		xQueueSendFromISR(myQueue02Handle, &coup, NULL);
	//}
	//if (RxHeader.StdId == 0x310) {//à notre id, on ignore
	//	NiveauBatterie = RxData[0];
	//	xQueueSendFromISR(myQueue02Handle, &, NULL);
	//}
}




/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
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
