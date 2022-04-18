#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "App_Main.h"

#include "bsp_led.h"  
#include "bsp_debug.h"
#include "bsp_oled096_spi.h"
#include "bsp_oled096_spi_BMP.h"
#include "bsp_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

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

      //�����߳�����01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//�����߳�����01
//	    //�����߳�����02
  Task02_TaskHandle = osThreadNew(Task02, NULL, &osID_Task02);//�����߳�����02
    //�����߳�����03
  Task03_TaskHandle = osThreadNew(Task03, NULL, &osID_Task03);//�����߳�����02

}




void Task01(void * argument)
{
	printf("����1�����ɹ���\r\n");
  for(;;)
  {
		led_twinkle(100);
  }
}

void Task02(void * argument)
{
  uint8_t t=' ';
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear()  	; 
	OLED_ShowCHinese(0,0,0);//��
	OLED_ShowCHinese(18,0,1);//��
	OLED_ShowCHinese(36,0,2);//԰
	OLED_ShowCHinese(54,0,3);//��
	OLED_ShowCHinese(72,0,4);//��
	OLED_ShowCHinese(90,0,5);//��
	OLED_ShowCHinese(108,0,6);//��
//	OLED_ShowString(0,3,"1.3' OLED TEST");
//	OLED_ShowString(0,6,"ASCII:");  
//	OLED_ShowString(63,6,"CODE:");  

  for(;;)
  {
//		OLED_ShowChar(48,6,t);//��ʾASCII�ַ�	   
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
		osDelay(1000);
  }
}

void Task03(void * argument)
{
	MPU_Init();							//��ʼ��MPU6050
  float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�
  while(mpu_dmp_init())
 	{
		printf ("MPU6050 Error\r\n");
		delay_ms(1000);
	}  
  printf ("MPU6050 OK \r\n"); 
 	OLED_ShowString(0,3,"Tttt");	
 	OLED_ShowString(4*8-1,3,"Pppp");	
 	OLED_ShowString(0,6,"Rrrr");	 
 	OLED_ShowString(4*8-1,6,"Yyyy");	 
  for(;;)
  {	
//    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
      printf ("�õ�ŷ���ǣ�%f,%f,%f\r\n",pitch,roll,yaw);
      printf ("�õ��¶�ֵ��%d\r\n",temp);
			printf ("�õ����ٶȴ��������ݣ�%d,%d,%d\r\n",aacx,aacy,aacz);
			printf ("�õ����������ݣ�%d,%d,%d\r\n",gyrox,gyroy,gyroz);
//			if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
//			if(report)usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
//		}
		osDelay(1000);
  }
}

