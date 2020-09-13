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
  uint32_t flagsX;
  uint8_t count = 0;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  while(1)
  {
	flagsX = osThreadFlagsWait(0x0002U, osFlagsWaitAny, osWaitForever);
	if(flagsX == 0x0002U)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

  }

  osThreadTerminate(NULL);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	osStatus_t status;
	uint32_t flags;
	if(GPIO_Pin == GPIO_PIN_13)
	{
		flags = osThreadFlagsSet(blinkyTaskHandle, 0x0002U);
	}
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
