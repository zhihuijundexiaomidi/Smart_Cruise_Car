#ifndef _BSP_MOTOR_H
#define	_BSP_MOTOR_H

#include "main.h"
#include <stdio.h>

//轮子的速度	tim3的通道1或通道2
extern TIM_HandleTypeDef htim3;

#define left_wheel_speed(x)			if(x>=0&&x<=100) \
	{__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,x);HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);printf("left_wheel_speed %d 成功!\r\n",x);} \
	else \
	{printf("left_wheel_speed 失败!\r\n");}
#define right_wheel_speed(x)	  if(x>=0&&x<=100) \
	{__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,x);HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);printf("right_wheel_speed %d 成功!\r\n",x);} \
	else \
	{printf("right_wheel_speed 失败!\r\n");}

/*
//轮子的定义
转向不对时，只需修改AB接口对应的引脚就好
*/
#define L_wheel_B		TB6612_A1_Pin
#define L_wheel_A		TB6612_A0_Pin
#define R_wheel_B		TB6612_B1_Pin
#define R_wheel_A		TB6612_B0_Pin
//轮子前进方式的定义
#define LF_RUN {HAL_GPIO_WritePin(GPIOB, L_wheel_A, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOB, L_wheel_B, GPIO_PIN_SET); }
#define RF_RUN {HAL_GPIO_WritePin(GPIOB, R_wheel_A, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOB, R_wheel_B, GPIO_PIN_SET); }
#define LB_RUN {HAL_GPIO_WritePin(GPIOB, L_wheel_A, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOB, L_wheel_B, GPIO_PIN_RESET); }
#define RB_RUN {HAL_GPIO_WritePin(GPIOB, R_wheel_A, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOB, R_wheel_B, GPIO_PIN_RESET); }

#define F_RUN  {LF_RUN;RF_RUN}
#define B_RUN	 {LB_RUN;RB_RUN}

#define CAR_STOP {HAL_GPIO_WritePin(GPIOB, L_wheel_A|L_wheel_B|R_wheel_A|R_wheel_B, GPIO_PIN_RESET);}

void forward_run(void);
void back_run(void);
void left_run(void);
void right_run(void);
void car_stop(void);


#endif /* _BSP_MOTOR_H */












