#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "main.h"
 	   		   
//IO��������
#define MPU_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define MPU_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO��������	 
#define MPU_IIC_SCL(x)    HAL_GPIO_WritePin(MPU_IIC_SCL_GPIO_Port,MPU_IIC_SCL_Pin,(GPIO_PinState)x) 		//SCL
#define MPU_IIC_SDA(x)  HAL_GPIO_WritePin(MPU_IIC_SDA_GPIO_Port,MPU_IIC_SDA_Pin,(GPIO_PinState)x)		//SDA	 
#define MPU_READ_SDA   HAL_GPIO_ReadPin(MPU_IIC_SDA_GPIO_Port,MPU_IIC_SDA_Pin) 		//����SDA 

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















