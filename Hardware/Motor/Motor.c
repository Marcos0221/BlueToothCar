/*
* File: Motor.c
* Author: Fanxing
* Date: 2024-02-27
* Description: 电机驱动相关函数
* Version: 1.0
*/

#include "Motor.h"

/*
* Function: Motor_Init
* Description: 初始化电机控制程序
* Input: None
* Output: None
* Returns: None
*/
void Motor_Init(void)
{
	// 共用结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//----------电机方向控制引脚----------
	RCC_APB2PeriphClockCmd(MotorRccControlPort, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = MotorLeft1 | MotorLeft2 | MotorRight1 | MotorRight2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(MotorControlPort, &GPIO_InitStruct);
	GPIO_ResetBits(MotorControlPort, MotorLeft1 | MotorLeft2 | MotorRight1 | MotorRight2);
	//-----------------------------------
	
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//开启GPIOA的时钟
	
	/*GPIO重映射*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//开启AFIO的时钟，重映射必须先开启AFIO的时钟
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);			//将TIM2的引脚部分重映射，具体的映射方案需查看参考手册
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//将JTAG引脚失能，作为普通GPIO引脚使用
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//将PA0引脚初始化为复用推挽输出	
																	//受外设控制的引脚，均需要配置为复用模式		
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM3);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 200 - 1;					//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
	/*输出比较初始化*/
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);							//结构体初始化，若结构体没有完整赋值
																	//则最好执行此函数，给结构体所有成员都赋一个默认值
																	//避免结构体初值不确定的问题
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//初始的CCR值
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);						//将结构体变量交给TIM_OC1Init，配置TIM2的输出比较通道1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			//使能TIM2，定时器开始运行 
}

void Motor_SetRightSpeed(int16_t speed)
{
	if(speed<0){
		GPIO_SetBits(MotorControlPort, MotorLeft2);
		GPIO_ResetBits(MotorControlPort, MotorLeft1);
		TIM_SetCompare1(TIM3, -1 * speed);
		return;
	}
	GPIO_SetBits(MotorControlPort, MotorLeft1);
	GPIO_ResetBits(MotorControlPort, MotorLeft2);
	TIM_SetCompare1(TIM3, speed);
}

void Motor_SetLeftSpeed(int16_t speed)
{
	if(speed<0){
		GPIO_SetBits(MotorControlPort, MotorRight1);
		GPIO_ResetBits(MotorControlPort, MotorRight2);
		TIM_SetCompare2(TIM3, -1 * speed);
		return;
	}
	GPIO_SetBits(MotorControlPort, MotorRight2);
	GPIO_ResetBits(MotorControlPort, MotorRight1);
	TIM_SetCompare2(TIM3, speed);
}
