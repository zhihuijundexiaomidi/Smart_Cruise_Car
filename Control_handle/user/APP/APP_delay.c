#include "APP_delay.h"
#include "tim.h"




/* USER CODE BEGIN 1 */
void delay_us(uint16_t us)
{
	uint16_t differ = 0xffff-5-us;				
	__HAL_TIM_SET_COUNTER(&htim2,differ);	//���ü���ֵ
	HAL_TIM_Base_Start(&htim2);		//��ʼ��ʱ������
	
	while(differ < 0xffff-5)
  {	//������ֵС�ڼ���ʱ
		differ = __HAL_TIM_GET_COUNTER(&htim2);		//��ȡ����ֵ
	}
	HAL_TIM_Base_Stop(&htim2);//������ʱ������
}
/* USER CODE END 1 */

