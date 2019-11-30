#include "SelfBuild_PID.h"

PID SteerPID = {  //���PID
				3.5,
				0,
				7,
				140,
				-140,
};

PID Left_motor_pid= {
          2,//P3.6
          1,//I0.08
          0,//D 
          499,//MAX
          -499,//MIN
};


PID Right_motor_pid= {
          2,//P3.6
          1,//I0.08
          0,//D 
          499,//MAX
          -499,//MIN
};

float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask)//����ʽPID
{
 
  PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]Ϊ��ǰ��PID_Ptr->error[1]Ϊ��һ����PID_Ptr->error[2]Ϊ���������
  PID_Ptr->Ad_value = PID_Ptr->P *(PID_Ptr->error[0] - PID_Ptr->error[1]) + 
                      PID_Ptr->I * PID_Ptr->error[0] +
                      PID_Ptr->D * (PID_Ptr->error[0] - 2 * PID_Ptr->error[1] + PID_Ptr->error[2]);//����PID����
  
  PID_Ptr->error[2] = PID_Ptr->error[1];//�����仰˳��Ի������
  PID_Ptr->error[1] = PID_Ptr->error[0];
  
  PID_Ptr->PID_Out += PID_Ptr->Ad_value;//����PID
  
  if(PID_Ptr->PID_Out >PID_Ptr->MAX) PID_Ptr->PID_Out =PID_Ptr-> MAX;//PID�޷�
  if(PID_Ptr->PID_Out <PID_Ptr->MIN) PID_Ptr->PID_Out =PID_Ptr-> MIN;
  
  return PID_Ptr->PID_Out;
}

float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask)//λ��ʽPID
{
  float Positional = 0;
  PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]Ϊ��ǰ��PID_Ptr->error[1]Ϊ��һ����PID_Ptr->error[2]Ϊ������
  
	PID_Ptr->error[2] += PID_Ptr->error[0];
  if(PID_Ptr->error[2] >  50) PID_Ptr->error[2] = 50;//һ�����ﻹ���л����޷�
  if(PID_Ptr->error[2]  <  -50) PID_Ptr->error[2] = -50;
	
  Positional = PID_Ptr->P * PID_Ptr->error[0] + 
               PID_Ptr->I * PID_Ptr->error[2] +
               PID_Ptr->D * (PID_Ptr->error[0] - PID_Ptr->error[1]);//����PID
  
  PID_Ptr->error[1] = PID_Ptr->error[0];
  if(Positional >PID_Ptr-> MAX) Positional =PID_Ptr-> MAX;//PID�޷�
  if(Positional <PID_Ptr-> MIN) Positional =PID_Ptr-> MIN;
  
  return  Positional;
}

// PID������ʼ��
void PID_Parameter_Init(PID *sptr)
{
	sptr->error[0]  = 0;
	sptr->error[1]  = 0;
	sptr->error[2]  = 0;
	sptr->Ad_value = 0;	//Error[-2]	
	sptr->PID_Out = 0;
	sptr->SumError= 0;
}  



