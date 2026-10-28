#ifndef __OLED_H
#define __OLED_H
#include "stm32f1xx_hal.h"

// 引脚定义 和CubeMX标签对应 PB8 PB9
#define OLED_SCL_PORT		GPIOB
#define OLED_SCL_PIN		GPIO_PIN_8
#define OLED_SDA_PORT		GPIOB
#define OLED_SDA_PIN		GPIO_PIN_9

#define OLED_W_SCL(x)		HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define OLED_W_SDA(x)		HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)

void OLED_I2C_Init(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
#endif
