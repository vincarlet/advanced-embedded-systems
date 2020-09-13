/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#define MSGQUEUE_OBJECTS 16                     // number of Message Queue Objects

typedef struct {                                // object data type
  uint8_t Idx;
} MSGQUEUE_OBJ_t;

osMessageQueueId_t mid_MsgQueue;                // message queue id

/* USER CODE END Variables */
/* Definitions for blinkyTask */
osThreadId_t blinkyTaskHandle;
const osThreadAttr_t blinkyTask_attributes = {
  .name = "blinkyTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBlinkyTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {

  mid_MsgQueue = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_OBJ_t), NULL);
  if (mid_MsgQueue == NULL) {
	; // Message Queue object not created, handle failure
  }
  /* Create the thread(s) */
  /* creation of blinkyTask */
  blinkyTaskHandle = osThreadNew(StartBlinkyTask, NULL, &blinkyTask_attributes);
}

/* USER CODE BEGIN Header_StartBlinkyTask */
/**
  * @brief  Function implementing the blinkyTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBlinkyTask */
void StartBlinkyTask(void *argument)
{
  /* USER CODE BEGIN StartBlinkyTask */
  /* Infinite loop */
  MSGQUEUE_OBJ_t msg;
  osStatus_t status;
  uint8_t count = 0;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  while(1)
  {
	status = osMessageQueueGet(mid_MsgQueue, &msg, NULL, 100);
	if(status == osOK)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
  }

  osThreadTerminate(NULL);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	MSGQUEUE_OBJ_t msg;
	osStatus_t status;
	if(GPIO_Pin == GPIO_PIN_13)
	{
		//Le funzioni del kernerl per la gestione dei thread non possono
		//essere chiamate nelle interruzioni
	    msg.Idx    = 1;
	    status = osMessageQueuePut(mid_MsgQueue, &msg, 0U, 0U);
	}
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
