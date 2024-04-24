#ifndef __HC05_H
#define __HC05_H

#include "stm32f10x.h"                  // Device header

void HC05_Init(void);
void HC05_SendByte(uint8_t Byte);
void HC05_SendString(char *String);
void HC05_Printf(char *format, ...);


#endif
