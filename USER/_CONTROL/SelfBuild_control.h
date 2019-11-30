#ifndef _SELFBUILD_CONTROL_H_
#define _SELFBUILD_CONTROL_H_

#include "headfile.h"

#define FULL_duty  1000  
#define SteerMid 1050
#define SteerMAX 1190
#define SteerMIN 910

extern int16 g_nLeft_duty;
extern int16 g_nRight_duty;
extern int16 g_nSpeedLeft; 
extern int16 g_nSpeedRight;
extern int16 g_nSpeedLeft_set;
extern int16 g_nSpeedRight_set;

extern float Weight[10];//权重
extern float CenterMeanValue;
extern uint32 SteerPwm;
extern float SteerPwmAdd;

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


