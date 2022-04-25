#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "App_Main.h"

#include "bsp_led.h"  
#include "bsp_infra_red.h"
#include "bsp_debug.h"
#include "bsp_nrf24l01.h"
#include "bsp_SG90.h"  
#include "bsp_encoder.h"  
#include "bsp_motor.h" 

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
  .priority = (osPriority_t) osPriorityHigh7,
};

void Task03(void * argument);
osThreadId Task03_TaskHandle;
const osThreadAttr_t osID_Task03 = {
  .name = "osID_Task03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};

void Task04(void * argument);
osThreadId Task04_TaskHandle;
const osThreadAttr_t osID_Task04 = {
  .name = "osID_Task04",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal6,
};

void Task05(void * argument);
osThreadId Task05_TaskHandle;
const osThreadAttr_t osID_Task05 = {
  .name = "osID_Task05",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh6,
};

void APP_Main(void) 
{
      //定义线程任务01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//创建线程任务01
//	    //定义线程任务02
//  Task02_TaskHandle = osThreadNew(Task02, NULL, &osID_Task02);//创建线程任务02
    //定义线程任务03
  Task03_TaskHandle = osThreadNew(Task03, NULL, &osID_Task03);//创建线程任务03
//	    //定义线程任务04
//  Task04_TaskHandle = osThreadNew(Task04, NULL, &osID_Task04);//创建线程任务04
//	Task05_TaskHandle = osThreadNew(Task05, NULL, &osID_Task05);//创建线程任务05
}




void Task01(void * argument)
{
		steering_engine_PWM(950);//中间右极致
  for(;;)
  {
		led_twinkle(100);
  }
}

void Task02(void * argument)
{
		EventBits_t r_event;  /* 定义一个事件接收变量 */
		infra_red_init();//这是创建事件，如果中断触发的时候，事件还没有创建，程序会宕机。（在中断中做了事件存在判断，不会死机了）
  for(;;)
  {
		check_app();
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
      
//			printf ( "r_event is %d \r\n",r_event);										
			check_app();
//			printf ( "check_app成功\r\n");
		}
//		osDelay(100);
  }
}

void Task03(void * argument)
{
	static unsigned int t=0;
  u8 rx_buf[33]="www.prechin.cn";
	while(NRF24L01_Check())	 //检测NRF24L01是否存在
	{
		printf("Error   \r\n");	
		osDelay(1000);		
	}
	printf("Success   \r\n");
	NRF24L01_Init();
  for(;;)
  {
		if(NRF24L01_RxPacket(rx_buf)==0) //接收到数据显示
			{
				rx_buf[32]='\0';
			  printf("%s",rx_buf);		
			}
			else
			{
				osDelay(100);
			}
			t++;
			if(t>=10)
			{
				t=0;
				printf("1S   \r\n");			
			}	
  }
}
//编码器开始工作
void Task04(void * argument)
{
	Encoder_Start();
//  uint16_t ecValue01 = get_left_encoder;
//  uint8_t direction01 = get_left_direction;
//  uint16_t ecValue02 = get_right_encoder;
//  uint8_t direction02 = get_right_direction;
  for(;;)
  {
//		 ecValue01 = get_left_encoder ; 
//		 direction01 = get_left_direction; 
//		 ecValue02 = get_right_encoder ; 
//		 direction02 = get_right_direction; 
//		printf("%d,%d,%d,%d \r\n",ecValue01,direction01,ecValue02,direction02);printf("speed: %d \r\n",base_speed);
		osDelay(1000);
  }
}
//实现左轮右轮的pid速度刷新
void Task05(void * argument)
{
	uint32_t tick;
  for(;;)
  {		
  	tick = osKernelGetTickCount();        // 获取当前系统的时间
	  tick += 10U;                      // 绝对延时1000个Systick
    osDelayUntil(tick);
		PID_REFLASH(0);
		PID_REFLASH(1);
//		back_run();
  }
}
