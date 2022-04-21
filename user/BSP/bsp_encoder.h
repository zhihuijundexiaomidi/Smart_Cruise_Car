#ifndef _BSP_encoder_H
#define	_BSP_encoder_H

#include "main.h"
#include "tim.h"
//�ⲿ���� extern˵���ı������������ļ�����
extern int   Encoder, CurrentPosition; //��ǰ�ٶȡ���ǰλ��
extern int   TargetVelocity, TargetCircle, CurrentPosition, Encoder,PWM; //Ŀ���ٶȡ�Ŀ��Ȧ����������������PWM���Ʊ���
extern float Velcity_Kp,  Velcity_Ki,  Velcity_Kd; //����ٶ�PID����
extern float Position_Kp, Position_Ki, Position_Kd; //���λ��PID����
extern int   MortorRun;  //���������Ʊ�־λ

#define left_htim				htim4
#define right_htim			htim2

#define dirt_forward    0
#define dirt_back			  1

//��ʼ��ȡ��������ֵ
#define right_encoder_start 			HAL_TIM_Encoder_Start(&right_htim,TIM_CHANNEL_ALL)//��ʼ��ȡ��������ֵ
#define left_encoder_start 			  HAL_TIM_Encoder_Start(&left_htim,TIM_CHANNEL_ALL)//��ʼ��ȡ��������ֵ
//�õ��������ļ���������ת��ֵ���ӣ�����ת����ֵ��С��
#define get_left_encoder				__HAL_TIM_GET_COUNTER(&left_htim)//�õ��������ļ���������ת��ֵ���ӣ�����ת����ֵ��С��
#define get_right_encoder				__HAL_TIM_GET_COUNTER(&right_htim)//�õ��������ļ���������ת��ֵ���ӣ�����ת����ֵ��С��
//���ñ�������ֵ
#define set_left_encoder(x)				__HAL_TIM_SET_COUNTER(&left_htim, x)
#define set_right_encoder(x)				__HAL_TIM_SET_COUNTER(&right_htim, x)
// ��ȡ�������ķ���0Ϊ����1Ϊ����
#define get_left_direction 	 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&left_htim)
#define get_right_direction 		__HAL_TIM_IS_TIM_COUNTING_DOWN(&right_htim)
//ֹͣ��ȡ��������ֵ
#define right_encoder_stop				HAL_TIM_Encoder_Stop(&right_htim,TIM_CHANNEL_ALL)
#define left_encoder_stop					HAL_TIM_Encoder_Stop(&left_htim,TIM_CHANNEL_ALL)

void Encoder_Start(void);
void PID_SET_SPEED(int x,int y);
void PID_REFLASH(int choose);
#endif /* _BSP_encoder_H */

