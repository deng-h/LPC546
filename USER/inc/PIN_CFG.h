#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

//----LED引脚----	 
#define  OLED_SCL_PIN	SPI4_SCK_B19
#define  OLED_SDA_PIN	SPI4_MOSI_B21
#define  OLED_RST_PIN	B1
#define  OLED_DC_PIN	B2
#define  OLED_CS_PIN	SPI4_CS0_B9
#define  OLED_SDA_IN  SPI4_MISO_B20

//----MPU6050模拟IIC引脚-----
#define MPU6050_SCL_PIN      B12
#define MPU6050_SDA_PIN      B3

//电感
#define AD_CH_L_1				ADC_CH5_A31 
#define AD_CH_L__				ADC_CH6_B0    
#define AD_CH_M_1				ADC_CH3_A15   
#define AD_CH_R__			  ADC_CH4_A16  
#define AD_CH_R_1				ADC_CH0_A10
  

#define CS0		B23  
#define CS1		B22
#define CS2		B10
#define CS3		B9
#define CS4		B8
#define CS5		0xff
#define CS6		0xff
#define CS7		0xff

#define p0		AD_CH_L_1
#define p1		AD_CH_L__
#define p2		AD_CH_M_1
#define p3		AD_CH_R__
#define p4		AD_CH_R_1
#define p5		0xff
#define p6		0xff
#define p7		0xff

//----按键----
#define Button_Up    A18
#define Button_Down  B4
#define Button_Left  B13
#define Button_Right B7
#define Button_Mid   B11

//----拨码开关----
#define Switch_1 B15	//无
#define Switch_2 B14	//无
#define Switch_3 A17	//无线调试使能
#define Switch_4 B17	//电磁校准

//----蜂鸣器----
#define Beep   A22

//舵机
#define Servo  TIMER4_PWMCH0_A6

//电机PWM

#define PWM_CH_L_Z			SCT0_OUT7_A28
#define PWM_CH_L_F			SCT0_OUT6_A27
#define PWM_CH_R_Z			SCT0_OUT8_A29
#define PWM_CH_R_F			SCT0_OUT5_A26



//编码器
#define CODER_L         TIMER3_COUNT0_A4
#define CODER_R         TIMER2_COUNT0_A24
#define CODER_DIR_L     A5
#define CODER_DIR_R     A25

//超声波
#define SONIC_Trig	B3
#define SONIC_Echo	B1

//串口
#define Bluetooth_UART     USART_3
#define Bluetooth_UART_TXD UART3_TX_A2
#define Bluetooth_UART_RXD UART3_RX_A3

#endif
