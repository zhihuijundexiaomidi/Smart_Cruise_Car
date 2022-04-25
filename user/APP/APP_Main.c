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
      //�����߳�����01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//�����߳�����01
//	    //�����߳�����02
//  Task02_TaskHandle = osThreadNew(Task02, NULL, &osID_Task02);//�����߳�����02
    //�����߳�����03
  Task03_TaskHandle = osThreadNew(Task03, NULL, &osID_Task03);//�����߳�����03
//	    //�����߳�����04
//  Task04_TaskHandle = osThreadNew(Task04, NULL, &osID_Task04);//�����߳�����04
//	Task05_TaskHandle = osThreadNew(Task05, NULL, &osID_Task05);//�����߳�����05
}




void Task01(void * argument)
{
		steering_engine_PWM(950);//�м��Ҽ���
  for(;;)
  {
		led_twinkle(100);
  }
}

void Task02(void * argument)
{
		EventBits_t r_event;  /* ����һ���¼����ձ��� */
		infra_red_init();//���Ǵ����¼�������жϴ�����ʱ���¼���û�д����������崻��������ж��������¼������жϣ����������ˣ�
  for(;;)
  {
		check_app();
		/* ������һ������ѭ�������ܷ��� */
		while (1)
		{
			/*******************************************************************
			 * �ȴ������¼���־ 
			 * 
			 * ���xClearOnExit����ΪpdTRUE����ô��xEventGroupWaitBits()����֮ǰ��
			 * �������ȴ�����������������ص�ԭ���ǳ�ʱ������ô���¼���������
			 * ��uxBitsToWaitFor�е��κ�λ����������� 
			 * ���xClearOnExit����ΪpdFALSE��
			 * ���ڵ���xEventGroupWaitBits()ʱ����������¼��������õ�λ��
			 *
			 * xWaitForAllBits���xWaitForAllBits����ΪpdTRUE����uxBitsToWaitFor��
			 * ������λ�����û�ָ���Ŀ�ʱ�䵽��ʱ��xEventGroupWaitBits()�ŷ��ء� 
			 * ���xWaitForAllBits����ΪpdFALSE��������uxBitsToWaitFor�����õ��κ�
			 * һ��λ��1 ��ָ���Ŀ�ʱ�䵽��ʱ��xEventGroupWaitBits()���᷵�ء� 
			 * ����ʱ����xTicksToWait����ָ����          
				*********************************************************/
			r_event = xEventGroupWaitBits(infra_red_Event_Handle,  /* �¼������� */
																		LeftCheck_EVENT|RightCheck_EVENT,/* �����̸߳���Ȥ���¼� */
																		pdTRUE,   /* �˳�ʱ����¼�λ */
																		pdFALSE,   /* �������Ȥ�������¼� */
																		portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */
      
//			printf ( "r_event is %d \r\n",r_event);										
			check_app();
//			printf ( "check_app�ɹ�\r\n");
		}
//		osDelay(100);
  }
}

void Task03(void * argument)
{
	static unsigned int t=0;
  u8 rx_buf[33]="www.prechin.cn";
	while(NRF24L01_Check())	 //���NRF24L01�Ƿ����
	{
		printf("Error   \r\n");	
		osDelay(1000);		
	}
	printf("Success   \r\n");
	NRF24L01_Init();
  for(;;)
  {
		if(NRF24L01_RxPacket(rx_buf)==0) //���յ�������ʾ
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
//��������ʼ����
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
//ʵ���������ֵ�pid�ٶ�ˢ��
void Task05(void * argument)
{
	uint32_t tick;
  for(;;)
  {		
  	tick = osKernelGetTickCount();        // ��ȡ��ǰϵͳ��ʱ��
	  tick += 10U;                      // ������ʱ1000��Systick
    osDelayUntil(tick);
		PID_REFLASH(0);
		PID_REFLASH(1);
//		back_run();
  }
}
