/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "cmsis_os.h"
#include "adc.h"
//#include "dac.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "rtc.h"
//#include "spi.h" pour bus CAN
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdio.h"
#include "rtc.h"
//#include "HorombeRGB565.h"
#include "BLEU.h"
#include "ROUGE.h"
#include "JAUNE.h"
#include "Vert.h"








/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct game{
	uint8_t nombre[5];//pioche,J1 J2 J3 J4//19*4+8*4-7*4=72
	int8_t cartes[8][2];//carte centre, main1,2,3,..7:[8,2]:2 rouge [0,0] si vide
	uint8_t game_state[3];//flag_actuel,sens_rot,timer_uno
}state={ {72,7,7,7,7},
	{ {8,2}  ,{5,10},{6,11},{7,11},{6,10},{8,12},{8,7},{7,12} },
	{1,1,0}};



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

//



int ID=1;//La variable à changer pour décider de la position sur la table
static TS_StateTypeDef  TS_State;//pour toucher écran exprès délai



//
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId tache1Handle;
osThreadId tache3Handle;
osThreadId tache4Handle;
osMessageQId myQueue02Handle;
osMutexId Mutex_layerHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void affichage_1(void const * argument);
void update_state(void const * argument);
void touch_confirm(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of Mutex_layer */
  osMutexDef(Mutex_layer);
  Mutex_layerHandle = osMutexCreate(osMutex(Mutex_layer));

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */


  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue02 */
  osMessageQDef(myQueue02, 32, Message);
  myQueue02Handle = osMessageCreate(osMessageQ(myQueue02), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of tache1 */
  osThreadDef(tache1, affichage_1, osPriorityAboveNormal, 0, 1024);
  tache1Handle = osThreadCreate(osThread(tache1), NULL);

  /* definition and creation of tache3 */
  osThreadDef(tache3, update_state, osPriorityHigh, 0, 1024);
  tache3Handle = osThreadCreate(osThread(tache3), NULL);

  /* definition and creation of tache4 */
  osThreadDef(tache4, touch_confirm, osPriorityAboveNormal, 0, 128);
  tache4Handle = osThreadCreate(osThread(tache4), NULL);

  /* USER CODE BEGIN RTOS_THREADS */











  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for(;;)
	{
		//HAL_GPIO_WritePin(LED13_GPIO_Port,LED13_Pin,1);
		//HAL_GPIO_WritePin(LED13_GPIO_Port,LED13_Pin,0);
		osDelay(100);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_affichage_1 */
/**
 * @brief Function implementing the tache1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_affichage_1 */
void affichage_1(void const * argument)
{
  /* USER CODE BEGIN affichage_1 */

	//Au final non? on va laisser Tout en background,
	//on affiche pas la carte dessous et on laisse tach2 faire

	extern int ID;


	TickType_t LastCall=xTaskGetTickCount();
	TickType_t value=1000;//environ 33 pour 30FPS


	int nb_cartes=7;
	char text[50]={};//agit comme un buffer pour les numeros cartes en main
	char inter[50]={};
	char j_1[50]={};
	char j_2[50]={};
	char j_3[50]={};
	char centre[50]={};
	char pioche[50]={};
	char msg[50]={};
	int X_main=50;
	int Y_main=300;

	//variables globales state : etat de la table


	//choisir le font du BSP LCD taille police et couleur





	/* Infinite loop */



	for(;;)
	{
		//on affiche ici le blink du UNO,le playing flag, et la carte du dessous


		xSemaphoreTake(Mutex_layerHandle,10);


		BSP_LCD_SelectLayer(1);


		BSP_LCD_Clear(00);





		//les 7 cartes en main utilisen la var global/tableau game_state.hand
		//avec color et number (et le nombre de cartes en main/une carte de stop valeur 0?)

		sprintf(text,"        ");//bon nb d'espaces??
		sprintf(inter," ");
		for(int i=0;i<8;i++){//[0,0] si pas de cartes

			//carte au centre, de X... et de Y...
			if (i==0){
				X_main=200;
				Y_main=100;
			}else{
				X_main=50;
				Y_main=250;
				//sprintf(&text+strlen(text),"%s    %d  ",text,(char)state.cartes[i][1]);//le bon nombre espaces

				if(state.cartes[i][1]<=9 && state.cartes[i][1]>=0){//nombre
				sprintf(inter,"     %d",(char)state.cartes[i][1]);}
				if(state.cartes[i][1]==12){//changement sens
				sprintf(inter,"     %%");}
				if(state.cartes[i][1]==10){//pioche 2 cartes
				sprintf(inter,"     !");}
				if(state.cartes[i][1]==11){
				sprintf(inter,"     #");}

				//UNE VALEUR DE 1 POUR AIT PIOCHE UNE CARTE
				//le mettre sur l'écran
				strcat(text,(char*) inter);
			}


			if (state.cartes[i][0]==8){//red
				BSP_LCD_DrawBitmap(X_main+i*40,Y_main,(uint8_t*) _acROUGE);
			}
			if (state.cartes[i][0]==7){//blue
				BSP_LCD_DrawBitmap(X_main+i*40,Y_main,(uint8_t*) _acBLEU);
			}
			if (state.cartes[i][0]==6){//green
				BSP_LCD_DrawBitmap(X_main+i*40,Y_main,(uint8_t*) _acVERT);
			}
			if (state.cartes[i][0]==5){//Yelllow
				BSP_LCD_DrawBitmap(X_main+i*40,Y_main,(uint8_t*) _acJAUNE);
			}

		}
		BSP_LCD_DisplayStringAtLine(20,(uint8_t*)text);//aligné avec les trous des images



		//Afficher le nombre de cartes en main des autres joueurs
		//pas immédiatemetn important? on relache le mutex?

		xSemaphoreGive(Mutex_layerHandle);
		xSemaphoreTake(Mutex_layerHandle,10);

		//game_state.hand_size chaque joueur donne périodiquement
		//le nombre de cartes en main disponibles

		//il faut TROUVER ALIGNEMENT NB LIGNES
		//sur des lignes différentes pour ne pas overlap?
		//aussi le nombre de cartes dans la pioche

		//i correspond à chaque joueur, de 1 à 4
		//donc grace à NOTRE numéro joueur, on en déduit pos relative
		for(int i=0;i<4;i++){
			if(i-ID==0){}//rien faire, c'est nous, mais on l'enverra
			if((i-ID==1) || (i-ID==-3)){//position relative à gauche/suivant

				sprintf(j_1,"         x%d",(char)state.nombre[i+1]);
				BSP_LCD_DisplayStringAtLine(4,(uint8_t*) j_1);//LEFT MODE sous entendu
			}
			if((i-ID)== 2 || (i-ID)== -2){//position relative en face + ou - 2/ ou c'est nous
				sprintf(j_2,"x%d",(char)state.nombre[i+1]);
				BSP_LCD_DisplayStringAt(0,1,(uint8_t*)j_2,CENTER_MODE);//les Y en pixel desormais
			}
			if((i-ID)==-1 || (i-ID==3)){//position relative à Droite/precedent
				sprintf(j_3,"x%d",(char)state.nombre[i+1]);
				BSP_LCD_DisplayStringAt(50,50,(uint8_t*)j_3,RIGHT_MODE);
			}

			//Affichage de la carte au centre et de la pioche
			sprintf(pioche,"x%d",(char)state.nombre[0]);
			BSP_LCD_DisplayStringAt(25,110,(char*) pioche,RIGHT_MODE);

			sprintf(centre,"x   %d",(char)state.cartes[0][1]);
			BSP_LCD_DisplayStringAt(-10,90,(char*) centre,CENTER_MODE);



			//Affichage du flag jeu en cours en fonction de state.game_state[0] qui joue

			sprintf(msg,"%d",(char)state.game_state[0]);
			BSP_LCD_DisplayStringAt(-10,50,(char*) msg,CENTER_MODE);

			xSemaphoreGive(Mutex_layerHandle);
		}




		BSP_LCD_SetBackColor(00);
		xSemaphoreGive(Mutex_layerHandle);
		vTaskDelayUntil(&LastCall,value);
	}
  /* USER CODE END affichage_1 */
}

/* USER CODE BEGIN Header_update_state */
/**
 * @brief Function implementing the tache3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_update_state */
void update_state(void const * argument)
{
  /* USER CODE BEGIN update_state */

	TickType_t LastCall=xTaskGetTickCount();
	TickType_t value=300;//environ 33 pour 30FPS


	char msg[50]={};
	char tour[50]={};

	Message recu;//on recoit qui a joué quelle carte
	//PAS une var globale mais juste le meme nom/strucuture



	/* Infinite loop */
	for(;;)
	{
		if(xQueueReceive(myQueue02Handle, &recu, 10)==pdPASS){//10ms au pire, plus rapide que les clics sur écran

		//TEST
		sprintf(msg,"on se crie %d dessus %d",(char)recu.couleur,(char)recu.playerID);
		BSP_LCD_DisplayStringAtLine(15,(char*) msg);


		//on modifie alors le game state

		state.nombre[recu.playerID]--;//commencent à 1 donc bien l'indice

		if(recu.couleur!=0){//couleur 0 est le ppas de cartes : [0,0] pour etre sur
		state.cartes[0][0]=recu.couleur;// efface la carte au centre si il n'y a rien
		state.cartes[0][1]=recu.valeur;
		}


		if(recu.prochain==-1){
			state.game_state[1]=-1;
		}
		else{state.game_state[1]= 1;}
		//MODULO 2 CAR 2 JOUEURS
		state.game_state[0]=(int)(state.game_state[0]+(int)state.game_state[1]*recu.prochain)%2+1;//pour revenir à 1 si "4 " dit +1

		sprintf(tour,"Au tour de :  %d",(char) state.game_state[0]);
		BSP_LCD_DisplayStringAt(-10,200,(uint8_t*) tour,CENTER_MODE);
	}//erreur probable car types
		else{


			sprintf(msg,"YA rien ");
			BSP_LCD_DisplayStringAtLine(15,(uint8_t*) msg);
		}
		vTaskDelayUntil(&LastCall,value);
	}

  /* USER CODE END update_state */
}

/* USER CODE BEGIN Header_touch_confirm */
/**
 * @brief Function implementing the tache4 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_touch_confirm */
void touch_confirm(void const * argument)
{
  /* USER CODE BEGIN touch_confirm */
	//on ne va pas tout le temps vérifier si le joueur appuie
	//ce qui implique une tache à grande période qui verifie
	//SI est est reveillée si on touche l'écran et sa pos
	TickType_t LastCall=xTaskGetTickCount();
	TickType_t value=300;
	extern TS_StateTypeDef TS_State;
	//au 1/3 de seconde près

	Message action;//un coup joué
	extern struct game state;
	extern int ID;
	uint8_t TxData1[4];
	//uint8_t TxData2[8];//pas de TX2
	//uint8_t RxData[8];//reception en tant qu'interruption
	uint32_t TxMailbox;
	uint32_t truc_can;
	extern CAN_HandleTypeDef hcan1;
	extern CAN_TxHeaderTypeDef TxHeader1;

	//copie colle de MCU
	TxHeader1.StdId = 0x410;
	TxHeader1.ExtId = 0x01; //peu importe, on est en ID 11 bits (CAN_ID_STD)
	TxHeader1.RTR = CAN_RTR_DATA;
	TxHeader1.IDE = CAN_ID_STD;
	TxHeader1.DLC = 4;
	TxHeader1.TransmitGlobalTime = DISABLE;


	int pos_carte=0;
	/* Infinite loop */
	for(;;)
	{
		pos_carte=1;

		if (state.game_state[0]==ID){//ID, notre propre numero de joueur

			BSP_TS_GetState(&TS_State);//si c'est à notre tour,
			//on regarde si on appuie sur écran et on envoie la position des cartes
			if(TS_State.touchDetected){
				//BSP_LCD_FillCircle(TS_State.touchX[0],TS_State.touchY[0],4);
				if(TS_State.touchY[0]>230){//le Ymain mais avec une marge
					for(int i=0;i<7;i++){//i=0 est la carte au centre donc décalage
						if(TS_State.touchX[0]>50+(i+1)*40 && TS_State.touchX[0]<50+(i+1+1)*40){
							action.couleur=state.cartes[i+1][0];//on check sur quelle carte on a appuyé et
							action.valeur=state.cartes[i+1][1];//on stocke ses parametres
							pos_carte=i+1;
						}
					}
					//on check si carte valide

					//pas 00
					//fonctionne sur le tas


				//on checck si le action est valide avant de l'envoyer
				if(state.cartes[0][0]==action.couleur || state.cartes[0][1] ==action.valeur ){


				//maintenant on a touché une carte légale
				//on enleve de la main
				state.cartes[pos_carte][0]=0;
				state.cartes[pos_carte][1]=0;

				//on prépare le reste de la chaine à envoyer
				action.playerID=ID;

				if (action.valeur<0){//carte sens inverse
				action.prochain=-1;}
				if (action.valeur==10){
					action.prochain=1;//+1 normal
				}
				if(action.valeur==11){
					action.prochain=2;//+2passe tour
				}


				//+1 si normal;+2 si passe *(-1) si changement de sense

				//valeur intermediaire si coup illegal? action.couleur=
				// ENVOYER SUR LE BUS CAN
				TxData1[0] = action.playerID;
				TxData1[1] = action.couleur;
				TxData1[2] = action.valeur;
				TxData1[3] = action.prochain;
				HAL_CAN_AddTxMessage(&hcan1, &TxHeader1, TxData1, &truc_can);


				//on change state en fonction de action, on reprend le code de update_state
				// -------------------------------------------------------------------------------

				//on a encore dans le if carte legale



				if(action.couleur!=0){//couleur 0 est le pas de cartes/pioche? : [0,0] pour etre sur
						state.cartes[0][0]=action.couleur;// efface la carte au centre si il n'y a rien
						state.cartes[0][1]=action.valeur;
						}
				else{
					//A 2 joueur, le joueur a son nb de carte qui augmente de 1


				}

				//pas besoin de changer son propre nb carte en main
				//state.nombre[action.playerID]--;


				if(action.prochain==-1){
					state.game_state[1]=-1;
				}
				else{state.game_state[1]= 1;}
				//MODULO 2 CAR 2 JOUEURS
				state.game_state[0]=(int)(state.game_state[0]+(int)state.game_state[1]*action.prochain)%2+1;//pour revenir à 1 si "4 " dit +1
				// -------------------------------------------------------------------------------




				//on a modifier les autres,
				//on change NOTRE state apres le }else ei





				}
				else if(TS_State.touchY[0]>100 && TS_State.touchY[0]<200){
					//piochage de cartes
					if(TS_State.touchX[0]>300 && TS_State.touchX[0]<450){
						//on a correctement appuyé et on envoie

						//on cherche la carte en main vide
						for(int i=0;i<7;i++){//7 cartes maxi, sinon on envoie pioche de 0 cartes
							if (state.cartes[i+1][0]==0 && state.cartes[i+1][1]==0){//vide
								pos_carte=i+1;
								break;
							}
							}

						//on génère une carte àleéatoiremetn à la position pos_carte


						//UNE VALEUR DE 1 POUR AIT PIOCHE UNE CARTE

						//on envoie sur le CAN on a pioché 1 carte
						action.playerID=ID;
						action.couleur=0;//pioche
						action.valeur=1;//on a pioché 1 carte puis fin de notre tour
						action.prochain=1;//sens normal

						TxData1[0] = action.playerID;
						TxData1[1] = action.couleur;
						TxData1[2] = action.valeur;
						TxData1[3] = action.prochain;
						HAL_CAN_AddTxMessage(&hcan1, &TxHeader1, TxData1, &truc_can);


					}
				}

			}
		}
		}

		vTaskDelayUntil(&LastCall,value);
	}
  /* USER CODE END touch_confirm */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

