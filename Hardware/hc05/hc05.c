#include "hc05.h"
#include "stdio.h"
#include <stdarg.h>

void HC05_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// GPIO结构体配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // GPIO 模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;              // GPIO 引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO 速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // GPIO 模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;              // GPIO 引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO 速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;     // 波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;        // 硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;        // 串口模式
	USART_InitStruct.USART_Parity = USART_Parity_No;        // 奇偶校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;     // 停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;        // 数据长度
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);      // 串口中断配置

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     // NVIC中断分组

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;      // 中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // 中断使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;      // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;     // 从占优先级
	NVIC_Init(&NVIC_InitStruct);

	USART_Cmd(USART1, ENABLE);      // 串口使能
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void HC05_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void HC05_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		HC05_SendByte(String[i]);		//依次调用HC05_SendByte发送每个字节数据
	}
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void HC05_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	HC05_SendString(String);		//串口发送字符数组（字符串）
}

