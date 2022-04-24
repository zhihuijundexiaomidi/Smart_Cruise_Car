#include "bsp_encoder.h"  
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "bsp_motor.h"  

int   TargetVelocity=5, TargetCircle=100, CurrentPosition , Encoder, PWM;  //Ŀ���ٶȡ�Ŀ��Ȧ��(λ��)����ǰλ�ã�������������PWM���Ʊ���
float Velcity_Kp=80,  Velcity_Ki=10,  Velcity_Kd; 				//����ٶ�PID����
float Position_Kp=120, Position_Ki=0.1, Position_Kd=400; 	//���λ��PID����


void Encoder_Start()
{
	right_encoder_start;//��ʼ��ȡ��������ֵ
  left_encoder_start;//��ʼ��ȡ��������ֵ
}

/**************************************************************************
�������ܣ���ȡ��������ֵ
��ڲ�����choose 0��left 1��right
����  ֵ��Encoder_number��int�ͣ���������Ϊ��ֵʱ��Ϊ����ת��
**************************************************************************/
int Read_Encoder(int choose)
{
	int Encoder_number;
	if(choose == 1)
	{
	   Encoder_number=get_left_encoder; //��ȡ����
		if(get_left_direction == dirt_back)Encoder_number=Encoder_number-0xffff; //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
																													//TIM4->CNT��ΧΪ0-0xffff��65535������ֵΪ0��
		set_left_encoder(0); //��ȡ����������
	}
	if(choose == 0)
	{
		Encoder_number=get_right_encoder; //��ȡ����
		if(get_right_direction == dirt_back)Encoder_number=Encoder_number-0xffff; //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
																													//TIM4->CNT��ΧΪ0-0xffff��65535������ֵΪ0��
		set_right_encoder(0); //��ȡ����������
	}
	
	return Encoder_number; //����ֵ
}


/**************************************************************************
�������ܣ��ٶȱջ�PID����(ʵ��ΪPI����)
��ڲ�����Ŀ���ٶ� ��ǰ�ٶ�
����  ֵ���ٶȿ���ֵ
��������ʽ��ɢPID��ʽ 
ControlVelocity+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
ControlVelocity�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
ControlVelocity+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
		int Bias;  //������ر���
		static int ControlVelocity, Last_bias; //��̬�������������ý�������ֵ��Ȼ����
		
		Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
		
		ControlVelocity+=Velcity_Kp*(Bias-Last_bias)+Velcity_Ki*Bias;  //����ʽPI������
                                                                   //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                                 //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
		Last_bias=Bias;	
		return ControlVelocity; //�����ٶȿ���ֵ
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ�����Ŀ��Ȧ��(λ��) ��ǰλ��
����  ֵ���ٶȿ���ֵ
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_FeedbackControl(float Circle, int CurrentPosition)
{
		float TargetPosition,Bias, ControlVelocity;     //������ر���
		static float Last_bias, Integral_Bias;          //��̬�������������ý�������ֵ��Ȼ����
		
	  TargetPosition=Circle*1560*1.01; //Ŀ��λ��=Ŀ��Ȧ��*1560
	                              //10ms��ȡһ�α�����(��100HZ)��������ٱ�Ϊ30����������������13��AB˫����ϵõ�4��Ƶ��
	                              //��ת1Ȧ����������Ϊ30*13*4=1560�����ת��=Encoder*100/1560r/s 
	                              //1.01�����ϵ�����������������ɸ���ʵ�����������ϵ������߿��ƾ���
	
		Bias=TargetPosition-CurrentPosition; //��λ��ƫ��
	  Integral_Bias+=Bias;
    if(Integral_Bias> 970) Integral_Bias= 970;	//�����޷� ��ֹ����Ŀ��λ�ú����
	  if(Integral_Bias<-970) Integral_Bias=-970;	//�����޷� ��ֹ����Ŀ��λ�ú����
	
		ControlVelocity=Position_Kp*Bias+Position_Ki*Integral_Bias+Position_Kd*(Bias-Last_bias);  //����ʽPI������
	                                                                                            //Position_Kp*Bias ƫ��Խ���ٶ�Խ��
	                                                                                            //Position_Ki*Integral_Bias ��С��̬���
	                                                                                            //Position_Kd*(Bias-Last_bias) �����ٶ�

		Last_bias=Bias;	
		return ControlVelocity;    //�����ٶȿ���ֵ 
}

/**************************************************************************
�������ܣ��ٶ�(PWM)�޷�
��ڲ�����PWM_P:λ�û������PWMֵ TargetVelocity:Ŀ���ٶ�(�ٶ�����ֵ)
����  ֵ����
**************************************************************************/
int PWM_Restrict(int PWM_P, int TargetVelocity)
{
    if     (PWM_P>+TargetVelocity*76) PWM_P=+TargetVelocity*76;
	  else if(PWM_P<-TargetVelocity*76) PWM_P=-TargetVelocity*76;
	  else PWM_P=PWM_P;
	  return PWM_P;
}
/**************************************************************************
�������ܣ���ʱ��ȡ��������ֵ������. 10ms����һ��������� 
				���ݶ�ȡ���ٶȽ���pid����.��������pwm��ֵ��
��ڲ�������
����  ֵ����
**************************************************************************/
void PID_REFLASH(int choose)
{
	  static int PWM_P, PWM_V;  //�ٶ�λ�ô���PID���Ʊ���PWM_P��PWM_V,��̬�������������ý�������ֵ��Ȼ����
	  Encoder=Read_Encoder(choose);   //��ȡ��ǰ���������������ٶ�
		if(Encoder<0)Encoder=-Encoder;
//		printf("Read_Encoder: %d \r\n",Encoder);
		CurrentPosition+=Encoder; //����������(�ٶ�)���ֵõ�λ��
		PWM_P=Position_FeedbackControl(TargetCircle, CurrentPosition); //λ�ñջ�����
		PWM_P=PWM_Restrict(PWM_P,TargetVelocity); //�޷�λ�û������PWM
		PWM_V=PWM_P/76; //PWMֵת��Ϊ�ٶ�ֵ
		PWM=Velocity_FeedbackControl(PWM_V, Encoder); //�ٶȻ��ջ����� �൱��λ�û������Ϊ�ٶȻ������룬�γɴ���PID
		if(PWM<0)PWM=-PWM;
//		printf("PWM_P: %d PWM_V: %d PWM: %d \r\n",PWM_P,PWM_V,PWM);
		if(PWM>7200-1)PWM=7200-1;
		base_speed=PWM;//����PWM		
}

void PID_SET_SPEED(int x,int y)
{
	TargetVelocity=x;//5-200
	TargetCircle=y;//10����
}
