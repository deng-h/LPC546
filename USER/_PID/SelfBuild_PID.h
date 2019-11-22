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
	float SumError;//累计误差
}PID;

typedef struct
{
	float Proportion; //参加PID运算的参数
  float Derivative; 
  float ProportionL; //左
  float DerivativeL; 
  float ProportionR; //右
  float DerivativeR; 
  float ProportionIL; //环岛？
  float ProportionIR; 
  int LastError; //上次误差
  float steer_out;  //输出
  int iError;  //当前误差
}STEER;


void steer_calcu(void);  //舵机的计算
extern PID Left_motor_pid;
extern PID Right_motor_pid;


extern PID Track_HengDuan;

float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask);//不一定要和我这样这么写，只是给你一个参考
float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask);
void PID_Parameter_Init(PID *sptr);
#endif
