#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "App_Main.h"

#include "bsp_led.h"  
#include "bsp_infra_red.h"
#include "bsp_debug.h"

void Task01(void * argument);
osThreadId Task01_TaskHandle;
const osThreadAttr_t osID_Task01 = {
  .name = "osID_Task01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void Task02(void * argument);
osThreadId Task02_TaskHandle;
const osThreadAttr_t osID_Task02 = {
  .name = "osID_Task02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};

void APP_Main(void) {

	infra_red_init();
      //定义线程任务01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//创建线程任务01
	    //定义线程任务02
  Task02_TaskHandle = osThreadNew(Task02, NULL, &osID_Task02);//创建线程任务02


}




void Task01(void * argument)
{

  for(;;)
  {
		led_twinkle(100);
  }
}

void Task02(void * argument)
{
		EventBits_t r_event;  /* 定义一个事件接收变量 */
  for(;;)
  {
		
		/* 任务都是一个无限循环，不能返回 */
		while (1)
		{
			/*******************************************************************
			 * 等待接收事件标志 
			 * 
			 * 如果xClearOnExit设置为pdTRUE，那么在xEventGroupWaitBits()返回之前，
			 * 如果满足等待条件（如果函数返回的原因不是超时），那么在事件组中设置
			 * 的uxBitsToWaitFor中的任何位都将被清除。 
			 * 如果xClearOnExit设置为pdFALSE，
			 * 则在调用xEventGroupWaitBits()时，不会更改事件组中设置的位。
			 *
			 * xWaitForAllBits如果xWaitForAllBits设置为pdTRUE，则当uxBitsToWaitFor中
			 * 的所有位都设置或指定的块时间到期时，xEventGroupWaitBits()才返回。 
			 * 如果xWaitForAllBits设置为pdFALSE，则当设置uxBitsToWaitFor中设置的任何
			 * 一个位置1 或指定的块时间到期时，xEventGroupWaitBits()都会返回。 
			 * 阻塞时间由xTicksToWait参数指定。          
				*********************************************************/
			r_event = xEventGroupWaitBits(infra_red_Event_Handle,  /* 事件对象句柄 */
																		LeftCheck_EVENT|RightCheck_EVENT,/* 接收线程感兴趣的事件 */
																		pdTRUE,   /* 退出时清除事件位 */
																		pdFALSE,   /* 满足感兴趣的所有事件 */
																		portMAX_DELAY);/* 指定超时事件,一直等 */
			printf ( "r_event is %d \r\n",r_event);										
			check_app();
			printf ( "check_app成功\r\n");
		}
		osDelay(100);
  }
}



