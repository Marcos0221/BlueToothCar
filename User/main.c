#include "stm32f10x.h"                  // Device header
#include "Motor.h"

int main(void) {  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	
		Motor_Init();
//		Motor_SetLeftSpeed(187);
//		Motor_SetRightSpeed(200);

  
    while (1) {  
 
    }  
}
