#include "bsp_motor.h"  

#define base_speed  50
#define left_fix_speed 0
#define right_fix_speed 0

void forward_run()
{
	F_RUN;
	left_wheel_speed(base_speed+left_fix_speed);
	right_wheel_speed(base_speed+right_fix_speed);
}

void back_run()
{
	B_RUN;
	left_wheel_speed(base_speed+left_fix_speed);
	right_wheel_speed(base_speed+right_fix_speed);
}

void left_run()
{
	LB_RUN;
	RF_RUN;
	left_wheel_speed(base_speed+left_fix_speed);
	right_wheel_speed(base_speed+right_fix_speed);
}

void right_run()
{
	LF_RUN;
	RB_RUN;
	left_wheel_speed(base_speed+left_fix_speed);
	right_wheel_speed(base_speed+right_fix_speed);
}

void car_stop()
{
	CAR_STOP;
	left_wheel_speed(0);
	right_wheel_speed(0);
}











