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

void Task01(void * argument);
osThreadId Task01_TaskHandle;
const osThreadAttr_t osID_Task02 = {
  .name = "osID_Task02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};

void APP_Main(void) {

	infra_red_init();
      //�����߳�����01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//�����߳�����01
	    //�����߳�����02
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//�����߳�����01


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
		EventBits_t r_event;  /* ����һ���¼����ձ��� */
  for(;;)
  {
		
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
																		pdTRUE,   /* �������Ȥ�������¼� */
																		portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */
													
			if((r_event & (LeftCheck_EVENT|RightCheck_EVENT)) == (LeftCheck_EVENT|RightCheck_EVENT)) 
			{
				/* ���������ɲ�����ȷ */
				check_app();
			}
			else
				printf ( "�¼�����\n");	
		}
		osDelay(100);
  }
}



