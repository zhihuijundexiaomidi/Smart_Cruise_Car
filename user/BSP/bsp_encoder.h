#ifndef _BSP_encoder_H
#define	_BSP_encoder_H

#include "main.h"
#include "tim.h"

#define left_htim				htim4
#define right_htim			htim2

//开始获取编码器的值
#define right_encoder_start 			HAL_TIM_Encoder_Start(&right_htim,TIM_CHANNEL_ALL);//开始获取编码器的值
#define left_encoder_start 			  HAL_TIM_Encoder_Start(&left_htim,TIM_CHANNEL_ALL);//开始获取编码器的值
//得到编码器的计数（正向转动值增加，逆向转动，值减小）
#define get_left_encoder				__HAL_TIM_GET_COUNTER(&left_htim);//得到编码器的计数（正向转动值增加，逆向转动，值减小）
#define get_right_encoder				__HAL_TIM_GET_COUNTER(&right_htim);//得到编码器的计数（正向转动值增加，逆向转动，值减小）
// 获取编码器的方向，0为正向，1为逆向。
#define get_left_direction 	 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&left_htim);
#define get_right_direction 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&right_htim);
//停止获取编码器的值
#define right_encoder_stop				HAL_TIM_Encoder_Stop(&right_htim,TIM_CHANNEL_ALL);
#define left_encoder_stop					HAL_TIM_Encoder_Stop(&left_htim,TIM_CHANNEL_ALL);

void Encoder_Start();


#endif /* _BSP_encoder_H */

