/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		标准计数/定时器
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#ifndef _LPC546XX_ctimer_h
#define _LPC546XX_ctimer_h


//此枚举定义不允许用户修改
typedef enum
{
    TIMER0,
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
}CTIMER_enum;

//此枚举定义不允许用户修改
typedef enum
{
    TIMER0_PWMCH0_A0=0,       TIMER0_PWMCH0_A30,      //定时器0  0通道输出PWM  引脚可选范围
    TIMER0_PWMCH1_A3=2,       TIMER0_PWMCH1_A31,      //定时器0  1通道输出PWM  引脚可选范围
    TIMER0_PWMCH2_A19=4,                              //定时器0  2通道输出PWM  引脚可选范围
    TIMER0_PWMCH3_B2=6,       TIMER0_PWMCH3_B27,      //定时器0  3通道输出PWM  引脚可选范围
                                                      
    TIMER1_PWMCH0_A18=8,      TIMER1_PWMCH0_B10,      //定时器1  0通道输出PWM  引脚可选范围
    TIMER1_PWMCH1_A20=10,     TIMER1_PWMCH1_B12,      //定时器1  1通道输出PWM  引脚可选范围
    TIMER1_PWMCH2_A23=12,     TIMER1_PWMCH2_B14,      //定时器1  2通道输出PWM  引脚可选范围
    TIMER1_PWMCH3_B16=14,                             //定时器1  3通道输出PWM  引脚可选范围
                                                      
    TIMER2_PWMCH0_A10=16,     TIMER2_PWMCH0_B5,       //定时器2  0通道输出PWM  引脚可选范围
    TIMER2_PWMCH1_B4=18,      TIMER2_PWMCH1_B6,       //定时器2  1通道输出PWM  引脚可选范围
    TIMER2_PWMCH2_A11=20,     TIMER2_PWMCH2_B7,       //定时器2  2通道输出PWM  引脚可选范围（A11与下载接口冲突）
    TIMER2_PWMCH3_A29=22,     TIMER2_PWMCH3_B22,      //定时器2  3通道输出PWM  引脚可选范围
                                            
    TIMER3_PWMCH0_A5=24,                              //定时器3  0通道输出PWM  引脚可选范围
    TIMER3_PWMCH1_B19=26,                             //定时器3  1通道输出PWM  引脚可选范围
    TIMER3_PWMCH2_A27=28,     TIMER3_PWMCH2_B21,      //定时器3  2通道输出PWM  引脚可选范围
    TIMER3_PWMCH3_A21=30,     TIMER3_PWMCH3_A23,      //定时器3  3通道输出PWM  引脚可选范围
                    
    TIMER4_PWMCH0_A6=32,                              //定时器4  0通道输出PWM  引脚可选范围
    TIMER4_PWMCH1_P4_14=34,                           //100引脚LPC 无此引脚占位用于设置为周期通道
        
}CTIMER_PWMCH_enum;


//此枚举定义不允许用户修改
typedef enum
{
    //每一个定时器，同一时间只能对一个引脚计数
    TIMER0_COUNT0_A1=0,       TIMER0_COUNT0_A13,     //定时器0  0通道计数输入引脚  引脚可选范围
    TIMER0_COUNT1_A2=2,         TIMER0_COUNT1_A14,   //定时器0  1通道计数输入引脚  引脚可选范围
    TIMER0_COUNT2_A28=4,                              //定时器0  2通道计数输入引脚  引脚可选范围
    TIMER0_COUNT3_B1=6,       TIMER0_COUNT3_B26,      //定时器0  3通道计数输入引脚  引脚可选范围
    //每一个定时器，同一时间只能对一个引脚计数                                                                    
    TIMER1_COUNT0_A16=8,        TIMER1_COUNT0_B9,     //定时器1  0通道计数输入引脚  引脚可选范围
    TIMER1_COUNT1_B11=10,                             //定时器1  1通道计数输入引脚  引脚可选范围
    TIMER1_COUNT2_B13=12,                             //定时器1  2通道计数输入引脚  引脚可选范围
    TIMER1_COUNT3_B15=14,                             //定时器1  3通道计数输入引脚  引脚可选范围
    //每一个定时器，同一时间只能对一个引脚计数                                                                    
    TIMER2_COUNT0_A24=16,                             //定时器2  0通道计数输入引脚  引脚可选范围
    TIMER2_COUNT1_A25=18,                             //定时器2  1通道计数输入引脚  引脚可选范围
    TIMER2_COUNT2_A10=20,                             //定时器2  2通道计数输入引脚  引脚可选范围 此引脚与SWD下载接口冲突  
    TIMER2_COUNT3_A28=22,                             //定时器2  3通道计数输入引脚  引脚可选范围
    //每一个定时器，同一时间只能对一个引脚计数                                                                    
    TIMER3_COUNT0_A4=24,                              //定时器3  0通道计数输入引脚  引脚可选范围
    TIMER3_COUNT1_A6=26,                              //定时器3  1通道计数输入引脚  引脚可选范围
    TIMER3_COUNT2_A26=28,     TIMER3_COUNT2_B20,      //定时器3  2通道计数输入引脚  引脚可选范围
    TIMER3_COUNT3_A20=30,     TIMER3_COUNT3_A22,      //定时器3  3通道计数输入引脚  引脚可选范围
                                                                        
    TIMER4_COUNT0_A15=32,                             //定时器4  0通道计数输入引脚  引脚可选范围
        
}CTIMER_COUNTCH_enum;


//此枚举定义不允许用户修改
typedef enum
{
    TIMER0_CAPTURECH0_A1=0,       TIMER0_CAPTURECH0_A13,     //定时器0  0通道输入捕捉  引脚可选范围
    TIMER0_CAPTURECH1_A14=2,      TIMER0_CAPTURECH1_A2,      //定时器0  1通道输入捕捉  引脚可选范围
    TIMER0_CAPTURECH2_B28=4,      TIMER0_CAPTURECH2_B0,    //定时器0  2通道输入捕捉  引脚可选范围
    TIMER0_CAPTURECH3_B1=6,       TIMER0_CAPTURECH3_B26,    //定时器0  3通道输入捕捉  引脚可选范围
                                                      
    TIMER1_CAPTURECH0_B9=8,      TIMER1_CAPTURECH0_A16, //定时器1  0通道输入捕捉  引脚可选范围
    TIMER1_CAPTURECH2_B13=10,         									 //定时器1  2通道输入捕捉  引脚可选范围
    TIMER1_CAPTURECH3_B15=11,     											 //定时器1  3通道输入捕捉  引脚可选范围
                                                      
    TIMER2_CAPTURECH0_A24=12,           								//定时器2  0通道输入捕捉  引脚可选范围
    TIMER2_CAPTURECH1_A25=13,           									//定时器2  1通道输入捕捉  引脚可选范围
    TIMER2_CAPTURECH2_A10=14,          									//定时器2  2通道输入捕捉  引脚可选范围
    TIMER2_CAPTURECH3_A28=15,         									//定时器2  3通道输入捕捉  引脚可选范围
                                            
    TIMER3_CAPTURECH0_A4=16,                              //定时器3  0通道输入捕捉  引脚可选范围
    TIMER3_CAPTURECH1_A6=17,                             //定时器3  1通道输入捕捉  引脚可选范围
    TIMER3_CAPTURECH2_A26=18,     TIMER3_CAPTURECH2_B20,      //定时器3  2通道输入捕捉  引脚可选范围
    TIMER3_CAPTURECH3_A20=20,     TIMER3_CAPTURECH3_A22,      //定时器3  3通道输入捕捉  引脚可选范围
                    
    TIMER4_CAPTURECH0_A15=22,                              //定时器4  0通道输入捕捉  引脚可选范围
        
}CTIMER_CAPTURE_enum;

typedef enum
{
	CAPTURE_RISING,
	CAPTURE_FALLING,
	CAPTURE_BOTH,
}CAPTURE_Input_cfg_enum;

typedef enum
{
	CH0,
	CH1,
	CH2,
	CH3,
}CAPTURE_ch_enum;

typedef enum
{
	  CAPTURE_PS_1,
    CAPTURE_PS_2,
    CAPTURE_PS_4,
    CAPTURE_PS_8,
    CAPTURE_PS_16,
    CAPTURE_PS_32,
    CAPTURE_PS_64,
    CAPTURE_PS_128,

    CAPTURE_PS_MAX,
}CAPTURE_PS_e_enum;

#define   CTIMER0_IR_FLAG(ch) CTIMER0->IR & 1<<(ch+4)		//中断标志位 ch-->通道号
#define   CTIMER1_IR_FLAG(ch) CTIMER1->IR & 1<<(ch+4)
#define   CTIMER2_IR_FLAG(ch) CTIMER2->IR & 1<<(ch+4)
#define   CTIMER3_IR_FLAG(ch) CTIMER3->IR & 1<<(ch+4)

#define   CTIMER0_IR_FLAG_CLEAR(ch) CTIMER0->IR |= 1<<(ch+4)		//清中断标志位 ch-->通道号
#define   CTIMER1_IR_FLAG_CLEAR(ch) CTIMER1->IR |= 1<<(ch+4)
#define   CTIMER2_IR_FLAG_CLEAR(ch) CTIMER2->IR |= 1<<(ch+4)
#define   CTIMER3_IR_FLAG_CLEAR(ch) CTIMER3->IR |= 1<<(ch+4)



#define TIMER0_PWM_PERIOD_CH    TIMER0_PWMCH2_A19     //定时器0 PWM周期占用的的通道，被占用的通道不能用于输出PWM（包含该通道其他引脚也不能输出PWM）
#define TIMER1_PWM_PERIOD_CH    TIMER1_PWMCH3_B16     //定时器1 PWM周期占用的的通道，被占用的通道不能用于输出PWM（包含该通道其他引脚也不能输出PWM）
#define TIMER2_PWM_PERIOD_CH    TIMER2_PWMCH3_B22     //定时器2 PWM周期占用的的通道，被占用的通道不能用于输出PWM（包含该通道其他引脚也不能输出PWM）
#define TIMER3_PWM_PERIOD_CH    TIMER3_PWMCH0_A5      //定时器3 PWM周期占用的的通道，被占用的通道不能用于输出PWM（包含该通道其他引脚也不能输出PWM）
#define TIMER4_PWM_PERIOD_CH    TIMER4_PWMCH1_P4_14   //定时器4 PWM周期占用的的通道，被占用的通道不能用于输出PWM（包含该通道其他引脚也不能输出PWM）

#define TIMER0_PWM_DUTY_MAX     10000                 //定时器0 PWM最大占空比  最大占空比越大占空比的步进值越小
#define TIMER1_PWM_DUTY_MAX     10000                 //定时器1 PWM最大占空比  最大占空比越大占空比的步进值越小
#define TIMER2_PWM_DUTY_MAX     10000                 //定时器2 PWM最大占空比  最大占空比越大占空比的步进值越小
#define TIMER3_PWM_DUTY_MAX     10000                 //定时器3 PWM最大占空比  最大占空比越大占空比的步进值越小
#define TIMER4_PWM_DUTY_MAX     10000                //定时器4 PWM最大占空比  最大占空比越大占空比的步进值越小

void    ctimer_pwm_init(CTIMER_PWMCH_enum pwmch, uint32 freq, uint32 duty);
void    ctimer_pwm_duty(CTIMER_PWMCH_enum pwmch, uint32 duty);
void    ctimer_pwm_freq(CTIMER_PWMCH_enum pwmch, uint32 freq);

void    ctimer_count_init(CTIMER_COUNTCH_enum countch);
uint32  ctimer_count_read(CTIMER_COUNTCH_enum countch);
void    ctimer_count_clean(CTIMER_COUNTCH_enum countch);

void ctimer_capturemux(CTIMER_CAPTURE_enum capturech);
void ctimer_capture_init(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch,CAPTURE_Input_cfg_enum cfg,CAPTURE_PS_e_enum ps);
void capture_SetInput_cfg(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch,CAPTURE_Input_cfg_enum cfg);

void capture_enable_irq(CTIMER_CAPTURE_enum capturech);
void capture_disable_irq(CTIMER_CAPTURE_enum capturech);

#endif
