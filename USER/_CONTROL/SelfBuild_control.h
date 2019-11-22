#ifndef _SELFBUILD_CONTROL_H_
#define _SELFBUILD_CONTROL_H_

#include "headfile.h"

#define FULL_duty  1000  
#define SteerMid 105
#define SteerMAX 119
#define SteerMIN 91

extern int16 g_nLeft_duty;
extern int16 g_nRight_duty;
extern int16 g_nSpeedLeft; 
extern int16 g_nSpeedRight;
extern int16 g_nSpeedLeft_set;
extern int16 g_nSpeedRight_set;

extern float Weight[10];//权重
extern float Steer_KP;//舵机方向比例系数
extern float Steer_KD;//舵机微分系数，影响舵机的打角反应
extern float SteerPwmAdd;//舵机pwm增量
extern float SteerError;//偏差值
extern float SteerLastError;//上次偏差值
extern float WeightSum;//权重之和
extern float CenterMeanValue;
extern float CenterSum;
extern float J;//调节p和偏差的关系，越大作用越强
extern float BasicP; //基本的p值
extern uint32 SteerPwm,LastSteerPwm;//本次舵机pwm和上次的pwm

extern float pitch, roll, yaw;//欧拉角
extern int16 fGyroY;//Y轴角速度
extern int16 fGyroZ;//Z轴角速度
extern int16 fGyroX;//x轴角速度
extern float g_fGyroYRead;
extern float g_fGyroZRead;
extern float g_fGyroXRead;

void PWM_OUT(int16 pwm_left,int16 pwm_right);
extern int32 range_protect(int32 duty, int32 min, int32 max);
void Speed_read(void); 
void DataRead(void);
void Oledshow(void);
void UI_Send(void);
void SteerControl(void);
void SteerErrorCalcu(void);

#endif


