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
	
#include "bsp_led.h"   
#include "stdint.h"
#include "cmsis_os.h"


led_err_t led_twinkle(int time)
{
	led_err_t err=led_ok;
	if(time<=0)
	{
		err=led_time_err;
	}
	int i=0;
	for(i=0;i <= time ;i++)
	{
		led_on;
		osDelay(1000);
		led_off;
		osDelay(1000);
	}
	return err;
}





