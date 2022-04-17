/**
  ******************************************************************************
  * @file    bsp_led.c
  ******************************************************************************
  * @attention
  *一切的开端都要从点灯开始说起，mxcub能够直接初始化底层后，我们只需要把板级支持包的接口和
  *对应的硬件接口关联在一起。led只是简单的GPIO的输出操作，而hal库已经完成了这些硬件接口。我们
	*只需要从定义一下名字，让功能看起来更直观就可以了。
  ******************************************************************************
  */
	
#include "bsp_SG90.h"  

//		steering_engine_PWM(1400);//左极致
//		osDelay(500);
//		steering_engine_PWM(950);//中间右极致
//    osDelay(500);
//    steering_engine_PWM(500);//右极致
//    osDelay(500);
