/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "can.h"

/* USER CODE BEGIN 0 */

/**此部分由AI生成 START**/

CAN_TxHeaderTypeDef TxHeader;
volatile int16_t dev_data[3] = {4096, 5678, 0x203};//此部分由AI生成但经过修改
volatile uint8_t dev_u8 = 0xAB;  //此部分由AI生成但经过修改
uint8_t tx_data[8] ; 
uint32_t tx_mailbox;
//发送成功标志，给OLED显示
volatile uint8_t can_send_ok_flag = 0;
volatile HAL_StatusTypeDef can_start_status;

/**此部分由AI生成 END**/
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 2;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_14TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  
  

  /* USER CODE BEGIN CAN_Init 2 */
  TxHeader.StdId = 0x200;
  TxHeader.ExtId = 0;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.DLC = 8;
  TxHeader.TransmitGlobalTime = DISABLE;
 
  can_start_status = HAL_CAN_Start(&hcan);
    if (can_start_status != HAL_OK)
    {
      Error_Handler();
    }
  
	
  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**此部分由AI生成 START**/

uint8_t CAN_Send_4dev(volatile int16_t *dev, uint8_t u8_val)
{
    tx_data[0] = ((uint16_t)dev[0] >> 8) & 0xFF;
    tx_data[1] = (uint16_t)dev[0] & 0xFF;

    tx_data[2] = ((uint16_t)dev[1] >> 8) & 0xFF;
    tx_data[3] = (uint16_t)dev[1] & 0xFF;

    tx_data[4] = ((uint16_t)dev[2] >> 8) & 0xFF;
    tx_data[5] = (uint16_t)dev[2] & 0xFF;

    tx_data[6] = u8_val;
    tx_data[7] = 0x00;

    if (HAL_CAN_GetState(&hcan) != HAL_CAN_STATE_READY &&
        HAL_CAN_GetState(&hcan) != HAL_CAN_STATE_LISTENING &&
        HAL_CAN_GetState(&hcan) != HAL_CAN_STATE_ERROR)
    {
        return 0;
    }

    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0)
    {
        return 0;
    }

    if (HAL_CAN_AddTxMessage(
            &hcan, &TxHeader, tx_data, &tx_mailbox) != HAL_OK)
    {
        return 0;
    }

    return 1;
}/**此部分由AI生成 END**/
/* USER CODE END 1 */

