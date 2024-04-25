#include "CarAction.h"
#include "Delay.h"

extern uint16_t LeftSpeed;
extern uint16_t RightSpeed;

void StartTurnLeft(void)
{
		Motor_SetLeftSpeed(80);
		Motor_SetRightSpeed(200);
		Delay_s(5);
		return; 
}
