#ifndef _BSP_INFRA_RED_H
#define	_BSP_INFRA_RED_H
#include "FreeRTOS.h"
#include "event_groups.h"

extern EventGroupHandle_t infra_red_Event_Handle;
	
#define left_check			infrared_sensor_left_Pin
#define right_check			infrared_sensor_right_Pin


#define LeftCheck_EVENT  (0x01 << 0)//设置事件掩码的位0
#define RightCheck_EVENT  (0x01 << 1)//设置事件掩码的位1

#define left_state    HAL_GPIO_ReadPin(GPIOB,  left_check)
#define right_state   HAL_GPIO_ReadPin(GPIOB,  right_check)
/*红外接受到产生低电平，没接收到红外，产生高电平。*/
#define state_on			GPIO_PIN_SET
#define state_off			GPIO_PIN_RESET

void infra_red_init(void );
void check_app(void);
#endif /* _BSP_INFRA_RED_H */



