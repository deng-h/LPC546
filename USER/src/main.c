/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

/*
接口定义：

摄像头             单片机接口
D0-D7数据口        B24 B25 B26 B27 B28 B29 B30 B31
SDA                A9   F5
SCL                A8
场信号             A0
像素信号           A1

运放
通道1              A10
通道2              A15
通道3              A16
通道4              A31
通道5              B0
通道6              A23

IIC                
SDA                A13 F1
SCL                A14

电机PWM
PWM1               A18   T1
PWM2               A20
PWM3               B4   T2
PWM4               B5

编码器         
LSB1               A2   T0
DIR                A3
LSB2               A4  T3
DIR                A5


舵机   
PWM                A6 T4

OLED/TFT
SCK                B19
MOSI               B21
MISO               B20      液晶没有这个引脚  但是SPI会占用这个引脚
CS                 B9 F4

DC                 B10
RES                B8
BL                 B7

UART                
RX                 A24
TX                 A25  

按键   
B1 B2 B3 B23任意交换  


拨码开关		
B17  B22任意交换  

NRF
B15 B14 A29 A22 A21 A17	任意交换  

*/


//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完


// 舵机中间PWM105,左转最大值119，右转最大91
#include "headfile.h"

int main(void)
{   
	get_clk();//这一句必须要写
	oled_init();
  /*----------------------------按键--------------------------------*/
	gpio_init(Button_Up, GPI, 1,PULLUP);
	gpio_init(Button_Down, GPI, 1,PULLUP);
 	gpio_init(Button_Left, GPI, 1,PULLUP);
	gpio_init(Button_Right, GPI, 1,PULLUP);  
	gpio_init(Button_Mid, GPI, 1,PULLUP);	
	gpio_init(Beep, GPO,1,PULLUP);   //设置蜂鸣器
	gpio_set(Beep,0);   //关掉蜂鸣器
	/*----------------------------编码器初始化------------------------------*/
	ctimer_count_init(CODER_L);	
	ctimer_count_init(CODER_R);
	gpio_init(CODER_DIR_L, GPI, 0,PULLDOWN);
	gpio_init(CODER_DIR_R, GPI, 0,PULLDOWN);
	camera_init();  //初始化摄像头 
/*----------------------------电机初始化-------------------------------*/
	sct_pwm_init(PWM_CH_L_Z, 12500, 0);
	sct_pwm_init(PWM_CH_L_F, 12500, 0);
	sct_pwm_init(PWM_CH_R_Z, 12500, 0);
	sct_pwm_init(PWM_CH_R_F, 12500, 0); 
	sct_pwm_duty(PWM_CH_L_Z,900);
	sct_pwm_duty(PWM_CH_L_F,1000);
 /*----------------------------舵机初始化-------------------------------*/ 
	ctimer_pwm_init(Servo,100,SteerMid);
	eeprom_init();
	Menu_Init();                                  //初始化菜单
	while(!Menu_Work()) systick_delay_ms(200);    //菜单每200ms工作一次，并根据是否按下“关闭菜单”选项后（函数返回0）结束死循环
//	Correct_Sensor();
	PID_Parameter_Init(&SteerPID);
	PID_Parameter_Init(&Left_motor_pid);
	PID_Parameter_Init(&Right_motor_pid);
	/*---------------------------定时器中断--------------------------------*/
	mrt_pit_init_ms(MRT_CH0,10);  //重点去看定时器中断服务函数里的东西
	uart_init(Bluetooth_UART,115200, Bluetooth_UART_TXD, Bluetooth_UART_RXD);
	enable_irq(FLEXCOMM3_IRQn);
	enable_irq(FLEXCOMM5_IRQn);
	EnableInterrupts;	
	while(1)
	{
		if(mt9v032_finish_flag)   //图像传输完毕
		{
			mt9v032_finish_flag = 0;
			Threshold_Value=my_adapt_threshold((uint8 *)image, COL,ROW);//大律法算出阈值 已经过优化 时间约为1ms
			GrayscaleToBinary((uint8 *)image, (uint8 *)CameraBinary,Threshold_Value, ROW*COL);//二值化 
			ScanLine((uint8 *)CameraBinary, MT9V032_W, MT9V032_H);
		}
		Oledshow();
		SteerControl();
		UI_Send();
		
  }
}


