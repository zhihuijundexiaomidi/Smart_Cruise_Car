#include "bsp_motor.h"  
#include "bsp_SG90.h"  

#define base_speed  20
#define left_fix_speed 0
#define right_fix_speed 0
#define turn_fix_speed	60//转弯速度不够快。会跑出轨道
void forward_run()
{
	F_RUN;
	steering_engine_PWM(950);//中间右极致
	left_wheel_speed(base_speed+left_fix_speed);
	right_wheel_speed(base_speed+right_fix_speed);
}

void back_run()
{
	B_RUN;
	steering_engine_PWM(950);//中间右极致
	left_wheel_speed(base_speed+left_fix_speed);
	right_wheel_speed(base_speed+right_fix_speed);
}

void left_run()
{
	LB_RUN;
	RF_RUN;
	steering_engine_PWM(600);//右极致
	left_wheel_speed(base_speed+left_fix_speed+20);
	right_wheel_speed(base_speed+right_fix_speed+30);
}

void right_run()
{
	LF_RUN;
	RB_RUN;
	steering_engine_PWM(1600);//左极致
	left_wheel_speed(base_speed+left_fix_speed+30);
	right_wheel_speed(base_speed+right_fix_speed+20);
}

void car_stop()
{
	CAR_STOP;
	steering_engine_PWM(950);//中间右极致
	left_wheel_speed(0);
	right_wheel_speed(0);
}











