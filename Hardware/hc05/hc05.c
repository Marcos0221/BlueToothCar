#include "hc05.h"
#include "stdio.h"
#include <stdarg.h>

void HC05_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	
	// ����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// GPIO�ṹ������
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // GPIO ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;              // GPIO ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO �ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // GPIO ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;              // GPIO ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO �ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;     // ������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;        // Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;        // ����ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;        // ��żУ��λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;     // ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;        // ���ݳ���
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);      // �����ж�����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     // NVIC�жϷ���

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;      // �ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // �ж�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;      // ��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;     // ��ռ���ȼ�
	NVIC_Init(&NVIC_InitStruct);

	USART_Cmd(USART1, ENABLE);      // ����ʹ��
}

/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void HC05_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void HC05_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		HC05_SendByte(String[i]);		//���ε���HC05_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void HC05_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	HC05_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

