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
void check_app(void)
{
	if(left_state==state_on && right_state== state_on)
	{
		car_stop();
	}
	if(left_state==state_off && right_state== state_on)
	{
		right_run();
	}
	if(left_state==state_on && right_state== state_off)
	{
		left_run();
	}
	if(left_state==state_off && right_state== state_off)
	{
		car_stop();
	}
	
}

