/*
* File: Motor.c
* Author: Fanxing
* Date: 2024-02-27
* Description: ���������غ���
* Version: 1.0
*/

#include "Motor.h"

/*
* Function: Motor_Init
* Description: ��ʼ��������Ƴ���
* Input: None
* Output: None
* Returns: None
*/
void Motor_Init(void)
{
	// ���ýṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//----------��������������----------
	RCC_APB2PeriphClockCmd(MotorRccControlPort, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = MotorLeft1 | MotorLeft2 | MotorRight1 | MotorRight2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(MotorControlPort, &GPIO_InitStruct);
	GPIO_ResetBits(MotorControlPort, MotorLeft1 | MotorLeft2 | MotorRight1 | MotorRight2);
	//-----------------------------------
	
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//����TIM2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ӳ��*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ������ȿ���AFIO��ʱ��
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);			//��TIM2�����Ų�����ӳ�䣬�����ӳ�䷽����鿴�ο��ֲ�
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//��JTAG����ʧ�ܣ���Ϊ��ͨGPIO����ʹ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//��PA0���ų�ʼ��Ϊ�����������	
																	//��������Ƶ����ţ�����Ҫ����Ϊ����ģʽ		
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM3);		//ѡ��TIM2Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 200 - 1;					//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;				//Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ
	
	/*����Ƚϳ�ʼ��*/
	TIM_OCInitTypeDef TIM_OCInitStructure;							//����ṹ�����
	TIM_OCStructInit(&TIM_OCInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//����Ƚ�ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//������ԣ�ѡ��Ϊ�ߣ���ѡ����Ϊ�ͣ�������ߵ͵�ƽȡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//��ʼ��CCRֵ
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);						//���ṹ���������TIM_OC1Init������TIM2������Ƚ�ͨ��1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	/*TIMʹ��*/
	TIM_Cmd(TIM3, ENABLE);			//ʹ��TIM2����ʱ����ʼ���� 
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
