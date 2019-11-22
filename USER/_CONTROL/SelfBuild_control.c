#include "SelfBuild_control.h"

int16 g_nLeft_duty=0;
int16 g_nRight_duty=0;
int16 g_nSpeedLeft=0; 
int16 g_nSpeedRight=0;
int16 g_nSpeedLeft_set=0;
int16 g_nSpeedRight_set=0;

float pitch = 0, roll = 0, yaw = 0;//ŷ����
int16 fGyroY=0;//Y����ٶ�
int16 fGyroZ=0;//Z����ٶ�
int16 fGyroX=0;//x����ٶ�
float g_fGyroYRead;
float g_fGyroZRead;
float g_fGyroXRead;

float Steer_KP=0.5;//����������ϵ��
float Steer_KD=0;//���΢��ϵ����Ӱ�����Ĵ�Ƿ�Ӧ
float SteerPwmAdd=0.0;//���pwm����
float SteerError;//ƫ��ֵ
float SteerLastError;//�ϴ�ƫ��ֵ
float WeightSum=0;//Ȩ��֮��
float CenterMeanValue=0;
float CenterSum=0;
float J=0.0290;//����p��ƫ��Ĺ�ϵ��Խ������Խǿ
float BasicP=3.0; //������pֵ
uint32 SteerPwm=0,LastSteerPwm=0;//���ζ��pwm���ϴε�pwm
float Weight[10] = {0,0,1,1,1.5,1.8,2,2,3,3};  //ÿ10�е�Ȩ�أ��ܹ�100��
extern uint8  CenterPoint[100];


void SteerErrorCalcu(void)
{
	int i;
	CenterSum=0;
	CenterMeanValue=0;
	WeightSum=0;
	
	for(i=0;i<100;i++)
	{
		CenterSum += CenterPoint[i]*Weight[i/10];
		WeightSum += Weight[i/10]; 
	}
	if(WeightSum!=0) CenterMeanValue=(CenterSum/WeightSum);  //�����Ȩƽ���������ֵ                  
  SteerLastError = SteerError;
	SteerError=(MT9V032_W/2 - CenterMeanValue);//һ��ͼƬ��ƫ��
  SteerError = SteerError;
	//ƫ���޷�
//	Steer_KP=BasicP+(SteerError* SteerError)*J;//��̬����pģ��      
//  if(Steer_KP>=11) Steer_KP = 11;//pֵ�޷�  �˴���Ҫ����ʵ������ģ�����
	
}

void SteerControl(void)
{
	SteerErrorCalcu();
	
	SteerPwmAdd=(Steer_KP*SteerError)+Steer_KD*(SteerError-SteerLastError);  //pd����ʽ
	
	SteerPwm=(uint32)(SteerPwmAdd+SteerMid);
	
	if(SteerPwm > SteerMAX) SteerPwm = SteerMAX;  //pwm����޷�
	if(SteerPwm < SteerMIN) SteerPwm = SteerMIN;
	
//	ctimer_pwm_duty(Servo,SteerPwm);
}


void DataRead(void)//�����ǻ��ȡ����ʧ��
{
	static float RollReadOld = 0, PitchReadOld = 0, YawReadOld = 0;
	int16 fGyroY = 0;
	
	
	if(MPU6050_DMPGetData(&pitch, &roll, &yaw))
	{
	   pitch = PitchReadOld;
		 roll= RollReadOld;
		 yaw = YawReadOld;
	}
	MPU6050_ReadData(MPU6050_GYRO_Y ,&fGyroY);
	MPU6050_ReadData(MPU6050_GYRO_Z ,&fGyroZ);
	MPU6050_ReadData(MPU6050_GYRO_X ,&fGyroX);
    g_fGyroYRead = (fGyroY) / 16.384 - 0.2259;
	  g_fGyroZRead = (fGyroZ) / 16.384 - 0.2259;
	  g_fGyroXRead = (fGyroX) / 16.384 - 0.2259;
	  PitchReadOld = pitch;
	  RollReadOld = roll;
	  YawReadOld = yaw;
}

uint8 Flag_Delay=0;

//*********ɨ�谴��***************//

int8 Button_get(void)//ɨ�谴��
{
   static int8 times = 0, Flag_K1 = 0, Flag_K2 = 0,Flag_K3=0,Flag_K4=0,Flag_K5=0;
	 int8 key_value=0;
   times ++;
  if(times >= 5)//ÿ times msִ��һ��
  {
    //-------------����----------------// 
		times = 0;
    if(gpio_get(Button_Up ) == 0 && Flag_K1 == 0) //��
      Flag_K1 = 1, Flag_Delay = 5,key_value|=0x01;
    
    else if(gpio_get(Button_Up) == 1 && Flag_K1 == 1)//�жϷ���
      Flag_K1 = 0;
    
    if(gpio_get(Button_Down) == 0 && Flag_K2 == 0) //��
      Flag_K2 = 1, Flag_Delay = 5,key_value|=0x02;
    else if(gpio_get(Button_Down) == 1 && Flag_K2 == 1)//�жϷ���
      Flag_K2 = 0;
    
    if(gpio_get(Button_Left) == 0 && Flag_K3 == 0) //��
      Flag_K3 = 1, Flag_Delay = 5,key_value|=0x04;
    else if(gpio_get(Button_Left) == 1 && Flag_K3 == 1)//�жϷ���
      Flag_K3 = 0;    
    
    if(gpio_get(Button_Right) == 0 && Flag_K4 == 0) //��
      Flag_K4 = 1, Flag_Delay = 5,key_value|=0x08;
    else if(gpio_get(Button_Right) == 1 && Flag_K4 == 1)//�жϷ���
      Flag_K4 = 0; 
    
    if(gpio_get(Button_Mid) == 0 && Flag_K5 == 0) //��
		Flag_K5 = 1, Flag_Delay = 5,key_value|=0x10;  
		 else if(gpio_get(Button_Mid) == 1 && Flag_K5 == 1)//�жϷ���
      Flag_K5 = 0; 
}
	if(Flag_Delay > 0) gpio_set(Beep, 1), Flag_Delay--;//������
    else gpio_set(Beep, 0);  
	return key_value;	  

}

//*************************���뿪��***************************//
 int8 BM_get(void)
{
	  int8 bm_value=0;
		if(gpio_get(Switch_1) == 0 ) //1
     bm_value|=0x01;
		if(gpio_get(Switch_2) == 0 ) //2
     bm_value|=0x02;
		if(gpio_get(Switch_3) == 0 ) //3
     bm_value|=0x04;
		if(gpio_get(Switch_4) == 0) //4
     bm_value|=0x08;	
	return bm_value;
}

void Oled_showGPRO()
{
	
			OLED_P6x8Str(16, 0, "GPRO");
			OLED_P6x8Flo(8, 1, pitch, -3);
			OLED_P6x8Flo(8, 2, roll, -3);
			OLED_P6x8Flo(8, 3, yaw, -3);
			OLED_P6x8Str(0, 4, "Y");
			OLED_P6x8Int(8, 4, g_fGyroYRead, -4);
			OLED_P6x8Str(0, 5, "Z");
			OLED_P6x8Int(8, 5, g_fGyroZRead, -4);
	    OLED_P6x8Str(0, 6, "X");
			OLED_P6x8Int(8, 6, g_fGyroXRead, -4);

		 
			OLED_P6x8Str(70, 0, "SPEED");
			oled_p6x8str(56, 1, "L");
			OLED_P6x8Int(64, 1, g_nSpeedLeft, -4);
			oled_p6x8str(56, 2, "R");
			OLED_P6x8Int(64, 2, g_nSpeedRight, -4); 
	
	
}

void adcshow()
{
	    OLED_P6x8Str(30, 0, "Electron_Show");
	   	OLED_P6x8Str(OLED_SHOW(1), 1, "L__");
	    OLED_P6x8Str(OLED_SHOW(2), 1, "L_1");
	    OLED_P6x8Str(OLED_SHOW(3), 1, "M_1");
	    OLED_P6x8Str(OLED_SHOW(4), 1, "R_1");
	    OLED_P6x8Str(OLED_SHOW(5), 1, "R__");//��ʾ����
		  OLED_P6x8Int(OLED_SHOW(1), 3, AD_Data.AD_L_L, 3);
			OLED_P6x8Int(OLED_SHOW(2), 3,	AD_Data.AD_L, 3);
			OLED_P6x8Int(OLED_SHOW(3), 3, AD_Data.AD_M, 3);
			OLED_P6x8Int(OLED_SHOW(4), 3, AD_Data.AD_R, 3);		
			OLED_P6x8Int(OLED_SHOW(5), 3, AD_Data.AD_R_R, 3);//��ʾ��ж���
	
}


void imageshow()
{
	    OLED_P6x8Str(10, 0, "Electron_Show");
	   	OLED_P6x8Str(OLED_SHOW(1), 1, "L__");
	    OLED_P6x8Str(OLED_SHOW(2), 1, "L_1");
	    OLED_P6x8Str(OLED_SHOW(3), 1, "M_1");
	    OLED_P6x8Str(OLED_SHOW(4), 1, "R_1");
	    OLED_P6x8Str(OLED_SHOW(5), 1, "R__");//��ʾ����
		  OLED_P6x8Int(OLED_SHOW(1), 3, AD_Data.AD_L_L, 3);
			OLED_P6x8Int(OLED_SHOW(2), 3,	AD_Data.AD_L, 3);
			OLED_P6x8Int(OLED_SHOW(3), 3, AD_Data.AD_M, 3);
			OLED_P6x8Int(OLED_SHOW(4), 3, AD_Data.AD_R, 3);		
			OLED_P6x8Int(OLED_SHOW(5), 3, AD_Data.AD_R_R, 3);//��ʾ��ж���
	
}

	//***********OLED��ʾ****************************//
void Oledshow(void)
{
	static int8 Last_BM_flag=0;
  int8 Button_flag,BM_flag;
	BM_flag=BM_get();
	if(Last_BM_flag!=BM_flag)
	{
		Last_BM_flag=BM_flag,OLED_ClearScreen(BLACK);//��ֵ�ı��������
		Button_flag=0;
		Flag_run=0;
		g_nSpeedLeft_set=0;
    g_nSpeedRight_set=0;
	}
	
  if(BM_flag&0x0f)	
	{
//		OLED_Cmd(0x01);
		if(BM_flag==0x01)	  //����4
		{ 
       Oled_showGPRO();
		}	
   if(BM_flag==0x08)	  //����3 �˴��и���0x02
		{ 
			adcshow();
		}			
   if(BM_flag==0x04)			
	 {
		 OLED_PrintBinary(COL,ROW,(uint8 *)CameraBinary); //�˺����������
    }
	 if(BM_flag==0x02)
	 {
		 
		    Flag_run=1;
		    Button_flag=Button_get();
				if(Button_flag==0x01)
				{
				    g_nSpeedLeft_set=50;
						g_nSpeedRight_set=50;
				}
				if(Button_flag==0x02)
				{
             g_nSpeedLeft_set=150;
             g_nSpeedRight_set=150;
				}
				
				if(Button_flag==0x04)
				{
             g_nSpeedLeft_set=-100;
             g_nSpeedRight_set=-100;
				}
		
				if(Button_flag==0x08)
				{
              g_nSpeedLeft_set=-150;
              g_nSpeedRight_set=-150;
				}
				if(Button_flag==0x10)
				{
              g_nSpeedLeft_set=0;
              g_nSpeedRight_set=0;
				}
			oled_p6x8str(0, 0, "real_L");
			OLED_P6x8Int(0, 1, g_nSpeedLeft_set, -4);
			OLED_P6x8Int(0, 2, g_nSpeedLeft, -4);
			oled_p6x8str(64, 0, "real_R");
			OLED_P6x8Int(64, 1, g_nSpeedRight_set, -4); 
		  OLED_P6x8Int(64, 2, g_nSpeedRight, -4);
				
	}
}
	else 	
	{
//     OLED_Cmd(0x00);
	}
}

void PWM_OUT(int16 pwm_left,int16 pwm_right)
{
	
	if(pwm_left>=0)
	{
		sct_pwm_duty(PWM_CH_L_Z,FULL_duty-pwm_left);	
	  sct_pwm_duty(PWM_CH_L_F,FULL_duty);	
	}
	else if(pwm_left<0)
	{
		sct_pwm_duty(PWM_CH_L_Z,FULL_duty);	
	  sct_pwm_duty(PWM_CH_L_F,FULL_duty+pwm_left);
	}

  if(pwm_right>=0)
	{
		sct_pwm_duty(PWM_CH_R_Z,FULL_duty-pwm_right);	
	  sct_pwm_duty(PWM_CH_R_F,FULL_duty);	
	}
	else if(pwm_right<0)
	{
		sct_pwm_duty(PWM_CH_R_Z,FULL_duty);	
	  sct_pwm_duty(PWM_CH_R_F,FULL_duty+pwm_right);
	}	

	
	
}

void Speed_read(void)
{
	g_nSpeedLeft =(gpio_get(CODER_DIR_L) == 1) ?ctimer_count_read(CODER_L) : -ctimer_count_read(CODER_L);//��ȡת��
	ctimer_count_clean(CODER_L);
	g_nSpeedRight =(gpio_get(CODER_DIR_R) == 0) ? ctimer_count_read(CODER_R) : -ctimer_count_read(CODER_R);
	ctimer_count_clean(CODER_R);
	
}

//******** �޷����� *********//
int32 range_protect(int32 duty, int32 min, int32 max)//�޷�����
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}

void UI_Send(void)
{
  float UI_Data[8] = {0};
  
  UI_Data[0] = SteerError;
  UI_Data[1] = SteerPwm;
  UI_Data[2] = SteerPwmAdd;
  UI_Data[3] = 0;
  UI_Data[4] = 0;
	UI_Data[5] = 0;
  UI_Data[6] = 0;
  UI_Data[7] = 0;
   
  vcan_sendware((uint8 *)UI_Data, sizeof(UI_Data));
}
