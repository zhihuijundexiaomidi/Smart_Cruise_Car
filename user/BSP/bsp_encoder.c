#include "bsp_encoder.h"  
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "bsp_motor.h"  

int   TargetVelocity=5, TargetCircle=100, CurrentPosition , Encoder, PWM;  //目标速度、目标圈数(位置)、当前位置，编码器读数、PWM控制变量
float Velcity_Kp=80,  Velcity_Ki=10,  Velcity_Kd; 				//相关速度PID参数
float Position_Kp=120, Position_Ki=0.1, Position_Kd=400; 	//相关位置PID参数


void Encoder_Start()
{
	right_encoder_start;//开始获取编码器的值
  left_encoder_start;//开始获取编码器的值
}

/**************************************************************************
函数功能：读取编码器数值
入口参数：choose 0：left 1：right
返回  值：Encoder_number：int型，有正负，为负值时，为逆向转动
**************************************************************************/
int Read_Encoder(int choose)
{
	int Encoder_number;
	if(choose == 1)
	{
	   Encoder_number=get_left_encoder; //读取计数
		if(get_left_direction == dirt_back)Encoder_number=Encoder_number-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
																													//TIM4->CNT范围为0-0xffff（65535），初值为0。
		set_left_encoder(0); //读取完后计数清零
	}
	if(choose == 0)
	{
		Encoder_number=get_right_encoder; //读取计数
		if(get_right_direction == dirt_back)Encoder_number=Encoder_number-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
																													//TIM4->CNT范围为0-0xffff（65535），初值为0。
		set_right_encoder(0); //读取完后计数清零
	}
	
	return Encoder_number; //返回值
}


/**************************************************************************
函数功能：速度闭环PID控制(实际为PI控制)
入口参数：目标速度 当前速度
返回  值：速度控制值
根据增量式离散PID公式 
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
ControlVelocity代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
ControlVelocity+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
		int Bias;  //定义相关变量
		static int ControlVelocity, Last_bias; //静态变量，函数调用结束后其值依然存在
		
		Bias=TargetVelocity-CurrentVelocity; //求速度偏差
		
		ControlVelocity+=Velcity_Kp*(Bias-Last_bias)+Velcity_Ki*Bias;  //增量式PI控制器
                                                                   //Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
	                                                                 //Velcity_Ki*Bias             速度控制值由Bias不断积分得到 偏差越大加速度越大
		Last_bias=Bias;	
		return ControlVelocity; //返回速度控制值
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：目标圈数(位置) 当前位置
返回  值：速度控制值
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_FeedbackControl(float Circle, int CurrentPosition)
{
		float TargetPosition,Bias, ControlVelocity;     //定义相关变量
		static float Last_bias, Integral_Bias;          //静态变量，函数调用结束后其值依然存在
		
	  TargetPosition=Circle*1560*1.01; //目标位置=目标圈数*1560
	                              //10ms读取一次编码器(即100HZ)，电机减速比为30，霍尔编码器精度13，AB双相组合得到4倍频，
	                              //则转1圈编码器读数为30*13*4=1560，电机转速=Encoder*100/1560r/s 
	                              //1.01是误差系数，电机本身存在误差，可根据实际情况调整该系数以提高控制精度
	
		Bias=TargetPosition-CurrentPosition; //求位置偏差
	  Integral_Bias+=Bias;
    if(Integral_Bias> 970) Integral_Bias= 970;	//积分限幅 防止到达目标位置后过冲
	  if(Integral_Bias<-970) Integral_Bias=-970;	//积分限幅 防止到达目标位置后过冲
	
		ControlVelocity=Position_Kp*Bias+Position_Ki*Integral_Bias+Position_Kd*(Bias-Last_bias);  //增量式PI控制器
	                                                                                            //Position_Kp*Bias 偏差越大速度越大
	                                                                                            //Position_Ki*Integral_Bias 减小稳态误差
	                                                                                            //Position_Kd*(Bias-Last_bias) 限制速度

		Last_bias=Bias;	
		return ControlVelocity;    //返回速度控制值 
}

/**************************************************************************
函数功能：速度(PWM)限幅
入口参数：PWM_P:位置环输出的PWM值 TargetVelocity:目标速度(速度限制值)
返回  值：无
**************************************************************************/
int PWM_Restrict(int PWM_P, int TargetVelocity)
{
    if     (PWM_P>+TargetVelocity*76) PWM_P=+TargetVelocity*76;
	  else if(PWM_P<-TargetVelocity*76) PWM_P=-TargetVelocity*76;
	  else PWM_P=PWM_P;
	  return PWM_P;
}
/**************************************************************************
函数功能：定时读取编码器数值并清零. 10ms进入一次这个函数 
				根据读取的速度进行pid计算.重新设置pwm的值。
入口参数：无
返回  值：无
**************************************************************************/
void PID_REFLASH(int choose)
{
	  static int PWM_P, PWM_V;  //速度位置串级PID控制变量PWM_P、PWM_V,静态变量，函数调用结束后其值依然存在
	  Encoder=Read_Encoder(choose);   //读取当前编码器读数，即速度
		if(Encoder<0)Encoder=-Encoder;
//		printf("Read_Encoder: %d \r\n",Encoder);
		CurrentPosition+=Encoder; //编码器读数(速度)积分得到位置
		PWM_P=Position_FeedbackControl(TargetCircle, CurrentPosition); //位置闭环控制
		PWM_P=PWM_Restrict(PWM_P,TargetVelocity); //限幅位置环输出的PWM
		PWM_V=PWM_P/76; //PWM值转换为速度值
		PWM=Velocity_FeedbackControl(PWM_V, Encoder); //速度环闭环控制 相当于位置环的输出为速度环的输入，形成串级PID
		if(PWM<0)PWM=-PWM;
//		printf("PWM_P: %d PWM_V: %d PWM: %d \r\n",PWM_P,PWM_V,PWM);
		if(PWM>7200-1)PWM=7200-1;
		base_speed=PWM;//设置PWM		
}

void PID_SET_SPEED(int x,int y)
{
	TargetVelocity=x;//5-200
	TargetCircle=y;//10以上
}
