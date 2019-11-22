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

extern float Weight[10];//Ȩ��
extern float Steer_KP;//����������ϵ��
extern float Steer_KD;//���΢��ϵ����Ӱ�����Ĵ�Ƿ�Ӧ
extern float SteerPwmAdd;//���pwm����
extern float SteerError;//ƫ��ֵ
extern float SteerLastError;//�ϴ�ƫ��ֵ
extern float WeightSum;//Ȩ��֮��
extern float CenterMeanValue;
extern float CenterSum;
extern float J;//����p��ƫ��Ĺ�ϵ��Խ������Խǿ
extern float BasicP; //������pֵ
extern uint32 SteerPwm,LastSteerPwm;//���ζ��pwm���ϴε�pwm

extern float pitch, roll, yaw;//ŷ����
extern int16 fGyroY;//Y����ٶ�
extern int16 fGyroZ;//Z����ٶ�
extern int16 fGyroX;//x����ٶ�
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


