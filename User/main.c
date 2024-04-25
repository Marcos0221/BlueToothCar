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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //��AFIOʱ���ߣ�AFIO�������棬���ж�����ѡ���ܣ���EXTI��NVICʱ��һֱ������NVIC���ں����裬����Ҫ��������
	
	//����GPIO�ṹ��
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);  //Selects the GPIO pin used as EXTI Line.����IO�����ж��ߵ�ӳ���ϵ
	
	//����EXTI�ṹ��
	EXTI_InitTypeDef EXTI_InitStructure;  
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;  //��Ӧ14����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //�ж�ģʽ  ����������EXTI_Mode_Interrupt = 0x00, �¼�ģʽ��EXTI_Mode_Event = 0x04
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش��� �ж�
	EXTI_Init(&EXTI_InitStructure);
	
 
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //ʹ��NVIC�ĵڶ��飨���ú�����оƬ��ֻ������һ���������������Ӧͼ��
	
	//����NVIC�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //��Ӧ���ж�����ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //��ռ���ȼ�
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
