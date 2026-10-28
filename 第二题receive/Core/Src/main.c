/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "can.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**此部分由AI生成 START**/

// 解析出来的变量
volatile uint16_t val_uint16;
volatile int16_t  val_int16_1;
volatile int16_t  val_int16_2;
volatile uint8_t  val_uint8;
volatile float    angle;    //映射后的角度 0~360°
volatile uint8_t can_rx_flag = 0;  //接收标记


 //CAN接收FIFO0中断回调
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef RxHeader;
  uint8_t rx_data[8];  //8字节DLC=8
  if(hcan->Instance == CAN1)
  {
    //读取收到的一帧CAN报文
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rx_data) == HAL_OK)
    {
      //只处理标准帧，DLC=8
      if(RxHeader.IDE == CAN_ID_STD && RxHeader.DLC == 8)
      {
       /* DATA[0], DATA[1]：uint16_t，大端 */
        val_uint16 = ((uint16_t)rx_data[0] << 8)
                | (uint16_t)rx_data[1];
        
        /* DATA[2], DATA[3]：int16_t，大端 */
        uint16_t raw1 = ((uint16_t)rx_data[2] << 8)
                    | (uint16_t)rx_data[3];
        
        val_int16_1 = (int16_t)raw1;
        
        /* DATA[4], DATA[5]：int16_t，大端 */
        uint16_t raw2 = ((uint16_t)rx_data[4] << 8)
                    | (uint16_t)rx_data[5];
        
        val_int16_2 = (int16_t)raw2;
        
        /* DATA[6]：uint8_t */
        val_uint8 = rx_data[6];
          
          if (val_uint16 > 8191)
        {
            val_uint16 = 8191;
        }

        // 数值映射 0~8191 →0~360°
        angle = (float)val_uint16 * 360.0f / 8191.0f;

        can_rx_flag = 1; //置接收标记，交给主循环OLED刷新
      }
    }
  }
}
/**此部分由AI生成 END**/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  
  OLED_Init();
  OLED_Clear();
  HAL_TIM_Base_Start_IT(&htim2);
  OLED_ShowString(1, 1, "CAN Ready");
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    
     
      /**此部分由AI生成 START**/

   // 收到CAN报文标记
    if(can_rx_flag == 1)
    {
      can_rx_flag = 0;
      
      OLED_Clear();
//      //第1行
//      OLED_ShowString(1,1,"CAN RX DATA");
      //第2行 角度
      OLED_ShowString(2,1,"Angle:");
      OLED_ShowNum(2,8,(uint16_t)angle,3);
      OLED_ShowString(2,12,"deg");
      //第3行 int16_1
      OLED_ShowString(3,1,"i16_1:");
      OLED_ShowSignedNum(3,7,val_int16_1,5);
      //第4行 int16_2
      OLED_ShowString(4,1,"i16_2:");
      OLED_ShowSignedNum(4,7,val_int16_2,5);
      //第5行 uint8
      OLED_ShowString(5,1,"u8:");
      OLED_ShowNum(5,4,val_uint8,3);
    }
      /**此部分由AI生成 END**/
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

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
#ifdef USE_FULL_ASSERT
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
