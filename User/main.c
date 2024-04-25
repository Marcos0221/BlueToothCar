#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "CarAction.h"
#include "hc05.h"
#include "string.h"
#include "math.h"
#include "Delay.h"

uint16_t LeftSpeed=0;
uint16_t RightSpeed=0;

int main(void) {  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	
		Motor_Init();
		HC05_Init();
		

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //打开AFIO时钟线（AFIO挂在外面，的中断引脚选择功能）。EXTI和NVIC时钟一直打来，NVIC是内核外设，不需要打开其外设
	
	//配置GPIO结构体
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);  //Selects the GPIO pin used as EXTI Line.设置IO口与中断线的映射关系
	
	//配置EXTI结构体
	EXTI_InitTypeDef EXTI_InitStructure;  
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;  //对应14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式  、、、、、EXTI_Mode_Interrupt = 0x00, 事件模式：EXTI_Mode_Event = 0x04
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发 中断
	EXTI_Init(&EXTI_InitStructure);
	
 
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //使用NVIC的第二组（设置后，整个芯片都只能用这一组来），共四组对应图。
	
	//配置NVIC结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //对应的中断向量通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //抢占优先级
	NVIC_Init(&NVIC_InitStructure);
//		Motor_SetLeftSpeed(97);
//		Motor_SetRightSpeed(100);
//		Delay_s(2);
    while (1) {  
			
    }  
}

void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line15)==SET)
	{
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
		Motor_SetLeftSpeed(50);
		Motor_SetRightSpeed(200);
		Delay_ms(900);
		Motor_SetLeftSpeed(97);
		Motor_SetRightSpeed(100);
		Delay_ms(1500);
		Motor_SetLeftSpeed(0);
		Motor_SetRightSpeed(0);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

void USART1_IRQHandler()
{
		uint16_t rec_data;
	
		if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
			rec_data=USART_ReceiveData(USART1);
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			if(rec_data=='a')
			{
					Motor_SetLeftSpeed(100);
					Motor_SetRightSpeed(100);
					Delay_ms(500);
					Motor_SetLeftSpeed(0);
					Motor_SetRightSpeed(0);
			}
			if(rec_data=='b')
			{
					Motor_SetLeftSpeed(-100);
					Motor_SetRightSpeed(-100);
					Delay_ms(500);
					Motor_SetLeftSpeed(0);
					Motor_SetRightSpeed(0);
			}
			if(rec_data=='c')
			{
					Motor_SetLeftSpeed(-100);
					Motor_SetRightSpeed(100);
					Delay_ms(500);
					Motor_SetLeftSpeed(0);
					Motor_SetRightSpeed(0);
			}
			if(rec_data=='d')
			{
					Motor_SetLeftSpeed(100);
					Motor_SetRightSpeed(-100);
					Delay_ms(500);
					Motor_SetLeftSpeed(0);
					Motor_SetRightSpeed(0);
			}
		}
		
}
