#ifndef _BSP_SG90_H
#define	_BSP_SG90_H

#include "main.h"
#include "tim.h"
/*******************************************************
舵机的控制周期是20ms
脉宽从0.5ms-2.5ms,分别对应-90°到+90°
20ms/20000=0.001ms
0.5/0.001=500
2.5/0.001=2500
********************************************************/
#define steering_engine_PWM(x)			if(x>=500&&x<=2500) \
	{__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,x);HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);printf("left_wheel_speed %d 成功!\r\n",x);} \
	else \
	{printf("left_wheel_speed 失败!\r\n");}
#endif /* _BSP_SG90_H */


