#ifndef _BSP_encoder_H
#define	_BSP_encoder_H

#include "main.h"
#include "tim.h"

#define left_htim				htim4
#define right_htim			htim2

//��ʼ��ȡ��������ֵ
#define right_encoder_start 			HAL_TIM_Encoder_Start(&right_htim,TIM_CHANNEL_ALL);//��ʼ��ȡ��������ֵ
#define left_encoder_start 			  HAL_TIM_Encoder_Start(&left_htim,TIM_CHANNEL_ALL);//��ʼ��ȡ��������ֵ
//�õ��������ļ���������ת��ֵ���ӣ�����ת����ֵ��С��
#define get_left_encoder				__HAL_TIM_GET_COUNTER(&left_htim);//�õ��������ļ���������ת��ֵ���ӣ�����ת����ֵ��С��
#define get_right_encoder				__HAL_TIM_GET_COUNTER(&right_htim);//�õ��������ļ���������ת��ֵ���ӣ�����ת����ֵ��С��
// ��ȡ�������ķ���0Ϊ����1Ϊ����
#define get_left_direction 	 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&left_htim);
#define get_right_direction 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&right_htim);
//ֹͣ��ȡ��������ֵ
#define right_encoder_stop				HAL_TIM_Encoder_Stop(&right_htim,TIM_CHANNEL_ALL);
#define left_encoder_stop					HAL_TIM_Encoder_Stop(&left_htim,TIM_CHANNEL_ALL);

void Encoder_Start();


#endif /* _BSP_encoder_H */

