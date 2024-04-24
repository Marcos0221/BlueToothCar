#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"                  // Device header

// 配置电机控制端口和端口时钟
#define MotorControlPort GPIOB
#define MotorRccControlPort RCC_APB2Periph_GPIOB

// 配置电机控制引脚
#define MotorLeft1 GPIO_Pin_12
#define MotorLeft2 GPIO_Pin_13
#define MotorRight1 GPIO_Pin_14
#define MotorRight2 GPIO_Pin_15

// 配置电机速度控制端口和端口时钟
#define MotorSpeedPort GPIOB
#define MotorRccSpeedPort RCC_APB2Periph_GPIOB

// 配置电机速度引脚
#define MotorLeftSpeed GPIO_Pin_4
#define MotorRightSpeed GPIO_Pin_5

void Motor_Init(void);
void Motor_SetLeftSpeed(int16_t speed);
void Motor_SetRightSpeed(int16_t speed);

#endif
