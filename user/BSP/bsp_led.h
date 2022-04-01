#ifndef _BSP_LED_H
#define	_BSP_LED_H

#include "main.h"

#define led_on					 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);//���ŵ�ƽ���ͣ�����
#define led_off          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);//���ŵ�ƽ���ߣ�Ϩ��

typedef enum
{
  led_ok = 1u,
  led_time_err,
} led_err_t;

led_err_t led_twinkle(int time);

#endif /* __LED_H */
