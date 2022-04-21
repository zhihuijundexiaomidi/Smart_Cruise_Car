#ifndef _BSP_encoder_H
#define	_BSP_encoder_H

#include "main.h"
#include "tim.h"
//外部变量 extern说明改变量已在其它文件定义
extern int   Encoder, CurrentPosition; //当前速度、当前位置
extern int   TargetVelocity, TargetCircle, CurrentPosition, Encoder,PWM; //目标速度、目标圈数、编码器读数、PWM控制变量
extern float Velcity_Kp,  Velcity_Ki,  Velcity_Kd; //相关速度PID参数
extern float Position_Kp, Position_Ki, Position_Kd; //相关位置PID参数
extern int   MortorRun;  //允许电机控制标志位

#define left_htim				htim4
#define right_htim			htim2

#define dirt_forward    0
#define dirt_back			  1

//开始获取编码器的值
#define right_encoder_start 			HAL_TIM_Encoder_Start(&right_htim,TIM_CHANNEL_ALL)//开始获取编码器的值
#define left_encoder_start 			  HAL_TIM_Encoder_Start(&left_htim,TIM_CHANNEL_ALL)//开始获取编码器的值
//得到编码器的计数（正向转动值增加，逆向转动，值减小）
#define get_left_encoder				__HAL_TIM_GET_COUNTER(&left_htim)//得到编码器的计数（正向转动值增加，逆向转动，值减小）
#define get_right_encoder				__HAL_TIM_GET_COUNTER(&right_htim)//得到编码器的计数（正向转动值增加，逆向转动，值减小）
//设置编码器的值
#define set_left_encoder(x)				__HAL_TIM_SET_COUNTER(&left_htim, x)
#define set_right_encoder(x)				__HAL_TIM_SET_COUNTER(&right_htim, x)
// 获取编码器的方向，0为正向，1为逆向。
#define get_left_direction 	 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&left_htim)
#define get_right_direction 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&right_htim)
//停止获取编码器的值
#define right_encoder_stop				HAL_TIM_Encoder_Stop(&right_htim,TIM_CHANNEL_ALL)
#define left_encoder_stop					HAL_TIM_Encoder_Stop(&left_htim,TIM_CHANNEL_ALL)

void Encoder_Start(void);
void PID_SET_SPEED(int x,int y);
void PID_REFLASH(int choose);
#endif /* _BSP_encoder_H */

