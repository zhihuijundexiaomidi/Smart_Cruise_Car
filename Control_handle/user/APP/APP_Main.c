#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "App_Main.h"

#include "bsp_led.h"  
#include "bsp_debug.h"
#include "bsp_oled096_spi.h"
#include "bsp_oled096_spi_BMP.h"

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

void Task03(void * argument);
osThreadId Task03_TaskHandle;
const osThreadAttr_t osID_Task03 = {
  .name = "osID_Task03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal6,
};

void APP_Main(void) {

      //定义线程任务01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//创建线程任务01
//	    //定义线程任务02
  Task02_TaskHandle = osThreadNew(Task02, NULL, &osID_Task02);//创建线程任务02
    //定义线程任务03
  Task03_TaskHandle = osThreadNew(Task03, NULL, &osID_Task03);//创建线程任务02

}




void Task01(void * argument)
{
	printf("任务1启动成功。\r\n");
  for(;;)
  {
		led_twinkle(100);
  }
}

void Task02(void * argument)
{
  uint8_t t=' ';
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 
	OLED_ShowCHinese(0,0,0);//中
	OLED_ShowCHinese(18,0,1);//景
	OLED_ShowCHinese(36,0,2);//园
	OLED_ShowCHinese(54,0,3);//电
	OLED_ShowCHinese(72,0,4);//子
	OLED_ShowCHinese(90,0,5);//科
	OLED_ShowCHinese(108,0,6);//技
	OLED_ShowString(0,3,"1.3' OLED TEST");
	OLED_ShowString(0,6,"ASCII:");  
	OLED_ShowString(63,6,"CODE:");  

  for(;;)
  {
		OLED_ShowChar(48,6,t);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
		osDelay(1000);
  }
}

void Task03(void * argument)
{
	
  for(;;)
  {	
		osDelay(100);
  }
}

