#include "bsp_infra_red.h"
#include <stdio.h>
#include "bsp_motor.h"  

EventGroupHandle_t infra_red_Event_Handle =NULL;

void infra_red_init(void )
{
	/* 创建 Event_Handle */
  infra_red_Event_Handle = xEventGroupCreate();	 
	 if(NULL != infra_red_Event_Handle)
    printf("infra_red_Event_Handle 事件创建成功!\r\n");
	 else
		printf("infra_red_Event_Handle 事件创建失败!\r\n");
	
}
//循迹功能逻辑
/*红外接受到产生低电平，没接收到红外，产生高电平。*/
void check_app(void)
{
	if(left_state==state_on && right_state== state_on)
	{
		car_stop();
		printf("car_stop();\r\n");
	}
	if(left_state==state_off && right_state== state_on)
	{
		right_run();
		printf("right_run();\r\n");
	}
	if(left_state==state_on && right_state== state_off)
	{
		left_run();
		printf("left_run();\r\n");
	}
	if(left_state==state_off && right_state== state_off)
	{
		forward_run();
		printf("forward_run();\r\n");
	}
	
}

BaseType_t xHigherPriorityTaskWoken = pdFALSE;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  switch(GPIO_Pin)
	{
		case infrared_sensor_left_Pin:
			xEventGroupSetBitsFromISR(infra_red_Event_Handle,LeftCheck_EVENT,&xHigherPriorityTaskWoken);
		  break ;
		case infrared_sensor_right_Pin:
			xEventGroupSetBitsFromISR(infra_red_Event_Handle,RightCheck_EVENT,&xHigherPriorityTaskWoken); 
			break ;
	}
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}

