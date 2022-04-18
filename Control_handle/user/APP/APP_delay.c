#include "APP_delay.h"
#include "tim.h"




/* USER CODE BEGIN 1 */
void delay_us(uint16_t us)
{
	uint16_t differ = 0xffff-5-us;				
	__HAL_TIM_SET_COUNTER(&htim2,differ);	//设置计数值
	HAL_TIM_Base_Start(&htim2);		//开始定时器计数
	
	while(differ < 0xffff-5)
  {	//当计数值小于计数时
		differ = __HAL_TIM_GET_COUNTER(&htim2);		//获取计数值
	}
	HAL_TIM_Base_Stop(&htim2);//结束定时器计数
}
/* USER CODE END 1 */

