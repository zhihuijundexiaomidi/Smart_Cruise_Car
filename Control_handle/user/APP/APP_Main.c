#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include "App_Main.h"

#include "bsp_led.h"  
#include "bsp_debug.h"
#include "bsp_oled096_spi.h"
#include "bsp_oled096_spi_BMP.h"
#include "bsp_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "bsp_nrf24l01.h"

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
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

void Task03(void * argument);
osThreadId Task03_TaskHandle;
const osThreadAttr_t osID_Task03 = {
  .name = "osID_Task03",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal6,
};

void Task04(void * argument);
osThreadId Task04_TaskHandle;
const osThreadAttr_t osID_Task04 = {
  .name = "osID_Task04",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal7,
};

void APP_Main(void) {

      //定义线程任务01
  Task01_TaskHandle = osThreadNew(Task01, NULL, &osID_Task01);//创建线程任务01
	    //定义线程任务02
  Task02_TaskHandle = osThreadNew(Task02, NULL, &osID_Task02);//创建线程任务02
//    //定义线程任务03
  Task03_TaskHandle = osThreadNew(Task03, NULL, &osID_Task03);//创建线程任务03
	  //定义线程任务04
  Task04_TaskHandle = osThreadNew(Task04, NULL, &osID_Task04);//创建线程任务04

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
	char str[32]="";
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	;  
	OLED_DrawBMP(0,2,128,8,BMP1);
  osDelay(1000);
  OLED_Clear();
  OLED_ShowString(0,0,"temp:    ",12);
  OLED_ShowString(0,1,"pitch:   ",12);
  OLED_ShowString(0,2,"roll:    ",12);
  OLED_ShowString(0,3,"yaw:     ",12);
	OLED_ShowString(0,4,"r:     ",12);
	OLED_ShowString(0,5,"mode:     ",12);
  for(;;)
  {
		sprintf(str,"%0.2f     ",temp);
		OLED_ShowString(40,0,(u8*)str,12);
		sprintf(str,"%0.1f    ",pitch);
		OLED_ShowString(48,1,(u8*)str,12);
		sprintf(str,"%0.1f    ",roll);
		OLED_ShowString(40,2,(u8*)str,12);
		sprintf(str,"%0.1f    ",yaw);
		OLED_ShowString(32,3,(u8*)str,12);
		snprintf(str,10,"%s               ",rf_rxbuf);
		OLED_ShowString(16,4,(u8*)str,12);
		osDelay(100);
  }
}

int NRF24L01_Init_sucflag=0;
void Task03(void * argument)
{
	MPU_Init();							//初始化MPU6050
	char str[32]="";
  while(mpu_dmp_init())		//dmp初始化
 	{
		printf ("MPU6050 Error\r\n");
		delay_ms(1000);
	}  
  printf ("MPU6050 OK \r\n");  
  int error = 0;
  for(;;)
  {	
//    while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0);
//    printf ("得到： pitch：%0.1f\r\n roll：%0.1f\r\n yaw：%0.1f\r\n",pitch,roll,yaw);
		error = mpu_dmp_get_data(&pitch,&roll,&yaw);
    if(error==0)//采集频率太低，会造成dmp的内存溢出。
		{ 
			temp=MPU_Get_Temperature();	//得到温度值
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//      printf ("得到欧拉：pitch：%0.1f\r\n roll：%0.1f\r\n yaw：%0.1f\r\n",pitch,roll,yaw);
//      printf ("得到温度值：%d\r\n",temp);
//			printf ("得到加速度传感器数据：%d,%d,%d\r\n",aacx,aacy,aacz);
//			printf ("得到陀螺仪数据：%d,%d,%d\r\n",gyrox,gyroy,gyroz);
		}
    else 
		{
			mpu_dmp_get_data(&pitch,&roll,&yaw);
//				printf("mpu_dmp_get_data error: %d \r\n",error);
//        printf ("得到温度值：%d\r\n",temp);
//				printf ("得到欧角:pitch：%0.1f\r\n roll：%0.1f\r\n yaw：%0.1f\r\n",pitch,roll,yaw);
		}
		if(NRF24L01_Init_sucflag==1)
		error=NRF24L01_RxPacket(rf_rxbuf);
		if(strstr((const char *)(rf_rxbuf), "HANDLE MODE") != NULL)
		{
				snprintf(str,11,"HANDLE MODE  ");
				OLED_ShowString(40,5,(u8*)str,12);
		}	
		if(strstr((const char *)(rf_rxbuf), "SMART MODE") != NULL)
		{
				snprintf(str,11,"SMART MODE   ");
				OLED_ShowString(40,5,(u8*)str,12);
		}	
//		printf("NRF24L01_RxPacket %d \r\n",error);
		osDelay(100);
  }
}

void Task04(void * argument)
{
	int mode = 1;//寻迹模式
	while(NRF24L01_Check())	//检测NRF24L01是否存在
	{
			printf("NRF24L01_Check erro \r\n");
			osDelay(1000);
	}
	printf("NRF24L01_Check suc \r\n");
	NRF24L01_Init();
	NRF24L01_Init_sucflag=1;
	char rf_txbuf[TX_PLOAD_WIDTH+1];	//发送数据组（32位）
	int error = 0;
  for(;;)
  {
	
//		sprintf((char*)rx_buf,"AT+MPU6050=p:%0.1f,r:%0.1f,y:%0.1f",pitch,roll,yaw);
//		printf("AT+MPU6050=pitch:%0.1f,roll:%0.1f,yaw:%0.1f \r\n",pitch,roll,yaw);
		if(-20<pitch && pitch<20 && roll<20 && roll>-20)
		{
			snprintf(rf_txbuf,32,"AT+CMD=STOP");
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
		if(pitch>20)
		{
			snprintf(rf_txbuf,32,"AT+CMD=B,%0.1f",pitch);
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
		if(pitch<-20)
		{
			snprintf(rf_txbuf,32,"AT+CMD=F,%0.1f",pitch);
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
		if(roll>20)
		{
			snprintf(rf_txbuf,32,"AT+CMD=L,%0.1f",roll);
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
		if(roll<-20)
		{
			snprintf(rf_txbuf,32,"AT+CMD=R,%0.1f",roll);
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
		if(yaw>40 && mode == 1)
		{
			mode = 0;
			snprintf(rf_txbuf,32,"AT+CMD=HANDLE");
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
		if(yaw<-40 && mode == 0)
		{
			mode = 1;
			snprintf(rf_txbuf,32,"AT+CMD=SMART");
			NRF24L01_TxPacket((u8*)rf_txbuf);
			printf("%s \r\n",rf_txbuf);
			osDelay(100);
		}
//		if(NRF24L01_Init_sucflag==1)
//		error=NRF24L01_RxPacket(rf_rxbuf);
//		printf("NRF24L01_RxPacket %d \r\n",error);
		osDelay(100);
  }
}

