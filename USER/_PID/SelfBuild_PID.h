#ifndef _SELFBUILD_PID_H
#define _SELFBUILD_PID_H

#include "headfile.h"
#define	PWM_MAX                         990
#define	PWM_MIN                         -990

typedef struct
{
  float P;
  float I;
  float D;
  int16 MAX;
  int16 MIN;
  float error[3];
  float Ad_value;
	float PID_Out;
	float SumError;//�ۼ����
}PID;


extern PID SteerPID;
extern PID Left_motor_pid;
extern PID Right_motor_pid;


extern PID Track_HengDuan;

float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask);//��һ��Ҫ����������ôд��ֻ�Ǹ���һ���ο�
float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask);
void PID_Parameter_Init(PID *sptr);
#endif
