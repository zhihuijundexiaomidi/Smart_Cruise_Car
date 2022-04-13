#ifndef _BSP_INFRA_RED_H
#define	_BSP_INFRA_RED_H
#include "FreeRTOS.h"
#include "event_groups.h"

extern EventGroupHandle_t infra_red_Event_Handle;
	
#define left_check			infrared_sensor_left_Pin
#define right_check			infrared_sensor_right_Pin


#define LeftCheck_EVENT  (0x01 << 0)//�����¼������λ0
#define RightCheck_EVENT  (0x01 << 1)//�����¼������λ1

#define left_state    HAL_GPIO_ReadPin(GPIOB,  left_check)
#define right_state   HAL_GPIO_ReadPin(GPIOB,  right_check)
/*������ܵ������͵�ƽ��û���յ����⣬�����ߵ�ƽ��*/
#define state_on			GPIO_PIN_SET
#define state_off			GPIO_PIN_RESET

void infra_red_init(void );
void check_app(void);
#endif /* _BSP_INFRA_RED_H */



