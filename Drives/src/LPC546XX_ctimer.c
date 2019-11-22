/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��׼����/��ʱ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴LPC546XX_config.h�ļ��ڰ汾�궨��
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_pll.h"
#include "LPC546XX_iocon.h"
#include "LPC546XX_ctimer.h"


CTIMER_Type  * TIMERN[] = CTIMER_BASE_PTRS;
const uint32 period_ch[]={TIMER0_PWM_PERIOD_CH,TIMER1_PWM_PERIOD_CH,TIMER2_PWM_PERIOD_CH,TIMER3_PWM_PERIOD_CH,TIMER4_PWM_PERIOD_CH};
const uint32 duty_max[]={TIMER0_PWM_DUTY_MAX,TIMER1_PWM_DUTY_MAX,TIMER2_PWM_DUTY_MAX,TIMER3_PWM_DUTY_MAX,TIMER4_PWM_DUTY_MAX};

void ctimer_clock(CTIMER_enum ctimern)    //����ʱ��
{
    switch(ctimern)
    {
        case TIMER0:
        {
            SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_CTIMER0_MASK;      //��CTIMER0ʱ��
            SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_CTIMER0_RST_MASK;  //���CTIMER0��λʱ��
        }break;
        
        case TIMER1:
        {
            SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_CTIMER1_MASK;     //��CTIMER1ʱ��
            SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_CTIMER1_RST_MASK; //���CTIMER1��λʱ��
        }break;
        
        case TIMER2:
        {
            SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL_CTIMER2_MASK;     //��CTIMER2ʱ��
            SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL_CTIMER2_RST_MASK; //���CTIMER2��λʱ��
        }break;
        
        case TIMER3:
        {
            SYSCON->ASYNCAPBCTRL = SYSCON_ASYNCAPBCTRL_ENABLE_MASK;
            ASYNC_SYSCON->ASYNCAPBCLKSELA = ASYNC_SYSCON_ASYNCAPBCLKSELA_SEL(0);
            ASYNC_SYSCON->ASYNCAPBCLKCTRLSET = ASYNC_SYSCON_ASYNCAPBCLKCTRL_CTIMER3_MASK;     //��CTIMER3ʱ��
            ASYNC_SYSCON->ASYNCPRESETCTRLCLR = ASYNC_SYSCON_ASYNCPRESETCTRL_CTIMER3_MASK;     //���CTIMER3��λʱ��
        }break;
        
        case TIMER4:
        {
            SYSCON->ASYNCAPBCTRL = SYSCON_ASYNCAPBCTRL_ENABLE_MASK;
            ASYNC_SYSCON->ASYNCAPBCLKSELA = ASYNC_SYSCON_ASYNCAPBCLKSELA_SEL(0);
            ASYNC_SYSCON->ASYNCAPBCLKCTRLSET = ASYNC_SYSCON_ASYNCAPBCLKCTRL_CTIMER4_MASK;     //��CTIMER4ʱ��
            ASYNC_SYSCON->ASYNCPRESETCTRLCLR = ASYNC_SYSCON_ASYNCPRESETCTRL_CTIMER4_MASK;     //���CTIMER4��λʱ��
        }break;
    }
}

void ctimer_pwmmux(CTIMER_PWMCH_enum pwmch)		//�������Ÿ��ù���->PWM
{
    switch(pwmch/8)
    {
        case TIMER0:
        {
            if      (TIMER0_PWMCH0_A0  == pwmch)  iocon_init(A0, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH0_A30 == pwmch)  iocon_init(A30,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH1_A3  == pwmch)  iocon_init(A3, ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH1_A31 == pwmch)  iocon_init(A31,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH2_A19 == pwmch)  iocon_init(A19,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH3_B2  == pwmch)  iocon_init(B2, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_PWMCH3_B27 == pwmch)  iocon_init(B27,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER1:
        {
            if      (TIMER1_PWMCH0_A18 == pwmch)  iocon_init(A18,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH0_B10 == pwmch)  iocon_init(B10,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH1_A20 == pwmch)  iocon_init(A20,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH1_B12 == pwmch)  iocon_init(B12,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH2_A23 == pwmch)  iocon_init(A23,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH2_B14 == pwmch)  iocon_init(B14,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_PWMCH3_B16 == pwmch)  iocon_init(B16,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER2:
        {
            if      (TIMER2_PWMCH0_A10 == pwmch)  iocon_init(A10,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH0_B5  == pwmch)  iocon_init(B5, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH1_B4  == pwmch)  iocon_init(B4, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH1_B6  == pwmch)  iocon_init(B6, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH2_A11 == pwmch)  iocon_init(A11,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH2_B7  == pwmch)  iocon_init(B7, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH3_A29 == pwmch)  iocon_init(A29,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_PWMCH3_B22 == pwmch)  iocon_init(B22,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER3:
        {
            if      (TIMER3_PWMCH0_A5  == pwmch)  iocon_init(A5, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH1_B19 == pwmch)  iocon_init(B19,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH2_A27 == pwmch)  iocon_init(A27,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH2_B21 == pwmch)  iocon_init(B21,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH3_A21 == pwmch)  iocon_init(A21,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_PWMCH3_A23 == pwmch)  iocon_init(A23,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER4:
        {
            if      (TIMER4_PWMCH0_A6  == pwmch)  iocon_init(A6, ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
    }
}

void ctimer_countmux(CTIMER_COUNTCH_enum pwmch)
{
    switch(pwmch/8)
    {
        case TIMER0:
        {
            if      (TIMER0_COUNT0_A1  == pwmch)  iocon_init(A1 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT0_A13 == pwmch)  iocon_init(A13,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT1_A2  == pwmch)  iocon_init(A2 ,ALT2 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT1_A14 == pwmch)  iocon_init(A14,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT2_A28 == pwmch)  iocon_init(A28,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT3_B1  == pwmch)  iocon_init(B1 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER0_COUNT3_B26 == pwmch)  iocon_init(B26,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER1:
        {
            if      (TIMER1_COUNT0_A16 == pwmch)  iocon_init(A16,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT0_B9  == pwmch)  iocon_init(B9 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT1_B11 == pwmch)  iocon_init(B11,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT2_B13 == pwmch)  iocon_init(B13,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER1_COUNT3_B15 == pwmch)  iocon_init(B15,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER2:
        {
            if      (TIMER2_COUNT0_A24 == pwmch)  iocon_init(A24,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER2_COUNT1_A25 == pwmch)  iocon_init(A25,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER2_COUNT2_A10 == pwmch)  iocon_init(A10,ALT2 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER2_COUNT3_A28 == pwmch)  iocon_init(A28,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER3:
        {
            if      (TIMER3_COUNT0_A4  == pwmch)  iocon_init(A4 ,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT1_A6  == pwmch)  iocon_init(A6 ,ALT2 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT2_A26 == pwmch)  iocon_init(A26,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT2_B20 == pwmch)  iocon_init(B20,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT3_A20 == pwmch)  iocon_init(A20,ALT3 | DIGITAL | FILTEROFF | PULLUP);
            else if (TIMER3_COUNT3_A22 == pwmch)  iocon_init(A22,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
        
        case TIMER4:
        {
            if      (TIMER4_COUNT0_A15 == pwmch)  iocon_init(A15,ALT3 | DIGITAL | FILTEROFF | PULLUP);
        }break;
    }
}

/**********************************************************************************************
******************************������PWM����********************************************
**********************************************************************************************
**********************************************************************************************/

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ��PWMģʽ��ʼ��
//  @param      pwmch       PWMͨ���ż�����
//  @param      freq        PWMƵ��
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           ctimer_pwm_init(TIMER0_PWMCH0_A0, 50, 5000);     //��ʼ����ʱ��0  0ͨ�� ʹ������A0ΪPWMģʽ  Ƶ��50HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/TIMER0_PWM_DUTY_MAX*100
//-------------------------------------------------------------------------------------------------------------------
void ctimer_pwm_init(CTIMER_PWMCH_enum pwmch, uint32 freq, uint32 duty)
{   
    uint32 match_temp;
    uint32 period_temp; 
    uint8  ctimer_num;

    ctimer_num = pwmch/8;
    
    
    ASSERT(period_ch[ctimer_num] != pwmch);    //���� ��ʼ����ͨ������Ϊ����ͨ��
    ASSERT(duty_max[ctimer_num] >= duty);      //���� ռ�ձȲ��ܳ������ռ�ձ�
    
    //����ʱ��
    ctimer_clock((CTIMER_enum)(ctimer_num));
    //���Ÿ�������
    ctimer_pwmmux(pwmch);

    //����Ԥ��Ƶֵ��ƥ��Ĵ���ֵ
    
    period_temp = (uint32)main_clk_mhz*1000*1000/freq;
    match_temp = (long long)period_temp*(duty_max[ctimer_num]-duty)/duty_max[ctimer_num];
    
    //TIMER�Ĵ�������
    TIMERN[ctimer_num]->TCR  &= ~CTIMER_TCR_CEN_MASK;
    
    TIMERN[ctimer_num]->PR = 0;                                    //����Ԥ��Ƶ�Ĵ���
    TIMERN[ctimer_num]->MR [(pwmch%8)/2] = match_temp;             //����ռ�ձ�
    TIMERN[ctimer_num]->MSR[(pwmch%8)/2] = match_temp;             //����ռ�ձ�
    TIMERN[ctimer_num]->MR[(period_ch[ctimer_num]%8)/2] = period_temp;//��������
    TIMERN[ctimer_num]->PWMC |= 1<<((pwmch%8)/2);
    TIMERN[ctimer_num]->EMR |= (0x2<<CTIMER_EMR_EMC0_SHIFT)<<((pwmch%8)&0xfe);
    TIMERN[ctimer_num]->MCR |= 1<<((period_ch[ctimer_num]%8)/2*3+1);
    TIMERN[ctimer_num]->MCR |= 1<<((pwmch%8)/2+24);
    
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CRST_MASK;
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CEN_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ��PWMռ�ձȸ���
//  @param      pwmch       PWMͨ���ż�����
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           ctimer_pwm_duty(TIMER0_PWMCH0_A0, 5000);     //��ʼ����ʱ��0  0ͨ�� ʹ������A0 ռ�ձ�Ϊ�ٷ�֮ 5000/TIMER0_PWM_DUTY_MAX*100
//-------------------------------------------------------------------------------------------------------------------
void ctimer_pwm_duty(CTIMER_PWMCH_enum pwmch, uint32 duty)
{
    uint32 match_temp;
    uint32 period_temp; 
    uint8  ctimer_num;

    ctimer_num = pwmch/8;
    if(duty_max[ctimer_num] < duty)    duty = duty_max[ctimer_num];
    switch(ctimer_num)
    {
        case 0: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 1: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 2: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 3: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
        case 4: ASSERT(TIMER0_PWM_DUTY_MAX>=duty);  break;
    }
    
    
    period_temp = TIMERN[ctimer_num]->MR[(period_ch[ctimer_num]%8)/2];  //��ȡ����
    match_temp = (long long)period_temp*(duty_max[ctimer_num]-duty)/duty_max[ctimer_num];

    TIMERN[ctimer_num]->MSR[(pwmch%8)/2] = match_temp;               //����ռ�ձ�
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ��PWMƵ�ʸ���
//  @param      pwmch       PWMͨ���ż�����
//  @param      freq        PWMƵ��
//  @return     void
//  Sample usage:           ctimer_pwm_freq(TIMER0_PWMCH0_A0, 50);     //��ʼ����ʱ��0  0ͨ�� ʹ������A0  Ƶ��50HZ
//-------------------------------------------------------------------------------------------------------------------
void ctimer_pwm_freq(CTIMER_PWMCH_enum pwmch, uint32 freq)
{
    uint32 period_temp; 
    uint8  ctimer_num;

    ctimer_num = pwmch/8;
    
    period_temp = (uint32)main_clk_mhz*1000*1000/freq;
    TIMERN[ctimer_num]->TCR  &= ~CTIMER_TCR_CEN_MASK;
    TIMERN[ctimer_num]->MR[(period_ch[ctimer_num]%8)/2] = period_temp;  //��������
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CRST_MASK;
    TIMERN[ctimer_num]->TCR  = CTIMER_TCR_CEN_MASK;
}

/**********************************************************************************************
******************************�������ⲿ���ż�������********************************************
**********************************************************************************************
**********************************************************************************************/
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ���ⲿ���ż���ģʽ��ʼ��
//  @param      countch     ����ͨ���ż�����
//  @return     void
//  Sample usage:           ctimer_count_init(TIMER0_COUNT0_A1);     //��ʼ����ʱ��0  0ͨ�� ʹ������A1  Ϊ����ģʽ
//-------------------------------------------------------------------------------------------------------------------
void ctimer_count_init(CTIMER_COUNTCH_enum countch)
{
    ctimer_clock((CTIMER_enum)(countch/8));
    ctimer_countmux(countch);
    
    TIMERN[countch/8]->TCR  &= ~CTIMER_TCR_CEN_MASK;
    
    TIMERN[countch/8]->CTCR = CTIMER_CTCR_CTMODE(0X01) | CTIMER_CTCR_CINSEL((countch%8)/2); 
    TIMERN[countch/8]->PR = 0;    
    
    TIMERN[countch/8]->TCR  = CTIMER_TCR_CRST_MASK;
    TIMERN[countch/8]->TCR  = CTIMER_TCR_CEN_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ������ֵ
//  @param      countch     ����ͨ���ż�����
//  @return     uint32      ���ؼ���ֵ
//  Sample usage:           num = ctimer_count_read(TIMER0_COUNT0_A1);     
//-------------------------------------------------------------------------------------------------------------------
uint32 ctimer_count_read(CTIMER_COUNTCH_enum countch)
{
    return TIMERN[countch/8]->TC;
}


void ctimer_count_clean(CTIMER_COUNTCH_enum countch)
{
    TIMERN[countch/8]->TC = 0;
}

/**********************************************************************************************
******************************���������벶׽����********************************************
**********************************************************************************************
**********************************************************************************************/
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������벶׽ģʽ���ù���
//  @param      capturech    ���벶׽ͨ���ż�����
//  @return     void      
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void ctimer_capturemux(CTIMER_CAPTURE_enum capturech)		//�������Ÿ��ù���->���벶׽
{
		uint8 TIMER_CH;
		if(capturech<=7)		     TIMER_CH=TIMER0;
		else if(capturech<=11)   TIMER_CH=TIMER1;
		else if(capturech<=15)   TIMER_CH=TIMER2;
		else if(capturech<=21)   TIMER_CH=TIMER3;
		else if(capturech<=22)   TIMER_CH=TIMER4;
	
    switch(TIMER_CH)
    {
        case TIMER0:
        {
            if      (TIMER0_CAPTURECH0_A1  == capturech)  iocon_init(A1, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_CAPTURECH0_A13 == capturech)  iocon_init(A13,ALT3 | DIGITAL | FILTEROFF);
            else if (TIMER0_CAPTURECH1_A14 == capturech)  iocon_init(A14,ALT3 | DIGITAL | FILTEROFF);
            else if (TIMER0_CAPTURECH1_A2  == capturech)  iocon_init(A2, ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_CAPTURECH2_B28 == capturech)  iocon_init(B28,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_CAPTURECH2_B0  == capturech)  iocon_init(B0, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER0_CAPTURECH3_B1  == capturech)  iocon_init(B1, ALT3 | DIGITAL | FILTEROFF | NOPULL);
					  else if (TIMER0_CAPTURECH3_B26 == capturech)  iocon_init(B26,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER1:
        {
            if      (TIMER1_CAPTURECH0_B9  == capturech)  iocon_init(B9, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_CAPTURECH0_A16 == capturech)  iocon_init(A16,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_CAPTURECH2_B13 == capturech)  iocon_init(B13,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER1_CAPTURECH3_B15 == capturech)  iocon_init(B15,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER2:
        {
            if      (TIMER2_CAPTURECH0_A24 == capturech)  iocon_init(A24,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_CAPTURECH1_A25 == capturech)  iocon_init(A25,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_CAPTURECH2_A10 == capturech)  iocon_init(A10,ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER2_CAPTURECH3_A28 == capturech)  iocon_init(A28,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER3:
        {
            if      (TIMER3_CAPTURECH0_A4  == capturech)  iocon_init(A4, ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_CAPTURECH1_A6  == capturech)  iocon_init(A6, ALT2 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_CAPTURECH2_A26 == capturech)  iocon_init(A26,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_CAPTURECH2_B20 == capturech)  iocon_init(B20,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_CAPTURECH3_A20 == capturech)  iocon_init(A20,ALT3 | DIGITAL | FILTEROFF | NOPULL);
            else if (TIMER3_CAPTURECH3_A22 == capturech)  iocon_init(A22,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
        
        case TIMER4:
        {
            if      (TIMER4_CAPTURECH0_A15 == capturech)  iocon_init(A15,ALT3 | DIGITAL | FILTEROFF | NOPULL);
        }break;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���벶׽ģʽ��ʼ��
//  @param      countch     ����ͨ���ż�����
//  @return     void      
//  Sample usage:           timer_count_clean(TIMER0_COUNT0_A1);     
//-------------------------------------------------------------------------------------------------------------------
void ctimer_capture_init(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch,CAPTURE_Input_cfg_enum cfg,CAPTURE_PS_e_enum ps)
{
	
		uint8 TIMER_CH;
		if(capturech<=7)		     TIMER_CH=TIMER0;
		else if(capturech<=11)   TIMER_CH=TIMER1;
		else if(capturech<=15)   TIMER_CH=TIMER2;
		else if(capturech<=21)   TIMER_CH=TIMER3;
		else if(capturech<=22)   TIMER_CH=TIMER4;

	  ctimer_clock(TIMER_CH); 	//����ʱ��
		ctimer_capturemux(capturech);	//�������ù���

    //TIMER�Ĵ�������
    TIMERN[TIMER_CH]->TCR  &= ~CTIMER_TCR_CEN_MASK;	//�رն�ʱ��
		switch(cfg)
    {
    case CAPTURE_RISING:    //�����ش���
				TIMERN[TIMER_CH]->CCR  =1<<(CAPTURE_ch*3) | 4<<(CAPTURE_ch*3);	//���������ش����������ж�
				TIMERN[TIMER_CH]->CTCR =CTIMER_CTCR_CTMODE(0) | CTIMER_CTCR_CINSEL(CAPTURE_ch)| CTIMER_CTCR_SELCC(CAPTURE_ch*2);
        break;

    case CAPTURE_FALLING:   //�½��ش���
				TIMERN[TIMER_CH]->CCR =2<<(CAPTURE_ch*3) | 4<<(CAPTURE_ch*3);	//�����½����ش����������ж�
				TIMERN[TIMER_CH]->CTCR =CTIMER_CTCR_CTMODE(0) | CTIMER_CTCR_CINSEL(CAPTURE_ch)| CTIMER_CTCR_SELCC(CAPTURE_ch*2+1);
        break;

    case CAPTURE_BOTH: //�����ء��½��ض�����
        TIMERN[TIMER_CH]->CCR  =1<<(CAPTURE_ch*3) | 2<<(CAPTURE_ch*3) | 4<<(CAPTURE_ch*3);	//���������غ��½��ش����������ж�
				TIMERN[TIMER_CH]->CTCR =CTIMER_CTCR_CTMODE(0) | CTIMER_CTCR_CINSEL(CAPTURE_ch) | CTIMER_CTCR_SELCC(CAPTURE_ch*2+1);
        break;
    }
		TIMERN[TIMER_CH]->CTCR |= CTIMER_CTCR_ENCC_MASK;	//ʹ�������ػ��½��ش������㶨ʱ���Ĵ���
    TIMERN[TIMER_CH]->PR = (ps==0?0:1)<<ps;     //����Ԥ��Ƶ�Ĵ���
		TIMERN[TIMER_CH]->IR = 1<<(CAPTURE_ch+4);	//����жϱ�־λ
    TIMERN[TIMER_CH]->TCR  = CTIMER_TCR_CRST_MASK;	//��λ��ʱ��
}

void capture_SetInput_cfg(CTIMER_CAPTURE_enum capturech,CAPTURE_ch_enum CAPTURE_ch,CAPTURE_Input_cfg_enum cfg)
{
		uint8 TIMER_CH;
		if(capturech<=7)		     TIMER_CH=TIMER0;
		else if(capturech<=11)   TIMER_CH=TIMER1;
		else if(capturech<=15)   TIMER_CH=TIMER2;
		else if(capturech<=21)   TIMER_CH=TIMER3;
		else if(capturech<=22)   TIMER_CH=TIMER4;
		switch(cfg)
    {
    case CAPTURE_RISING:    //�����ش���
				TIMERN[TIMER_CH]->CCR &=~(2<<(CAPTURE_ch*3));	//�ر��½��ش���
				TIMERN[TIMER_CH]->CCR  =1<<(CAPTURE_ch*3);		//���������ش���
        break;
    case CAPTURE_FALLING:   //�½��ش���
				TIMERN[TIMER_CH]->CCR &=~(1<<(CAPTURE_ch*3));	//�ر������ش���
				TIMERN[TIMER_CH]->CCR  =2<<(CAPTURE_ch*3);		//�����½��ش���
        break;
    case CAPTURE_BOTH: //�����ء��½��ض�����
        TIMERN[TIMER_CH]->CCR |=1<<(CAPTURE_ch*3) | 2<<(CAPTURE_ch*3);	//���������غ��½��ش���
        break;
    }
}

void capture_enable_irq(CTIMER_CAPTURE_enum capturech)			//ʹ�����벶׽�ж�
{
	uint8 TIMER_CH;
	if(capturech<=7)		     TIMER_CH=TIMER0;
	else if(capturech<=11)   TIMER_CH=TIMER1;
	else if(capturech<=15)   TIMER_CH=TIMER2;
	else if(capturech<=21)   TIMER_CH=TIMER3;
	else if(capturech<=22)   TIMER_CH=TIMER4;
	
	if(TIMER_CH == TIMER0 || TIMER_CH == TIMER1)	enable_irq((IRQn_Type)(CTIMER0_IRQn+TIMER_CH));
	else if(TIMER_CH == TIMER2 ) enable_irq((IRQn_Type)(CTIMER2_IRQn));
	else if(TIMER_CH == TIMER3 ) enable_irq((IRQn_Type)(CTIMER3_IRQn));
	else if(TIMER_CH == TIMER4 ) enable_irq((IRQn_Type)(CTIMER4_IRQn));
}

void capture_disable_irq(CTIMER_CAPTURE_enum capturech)		//ʧ�����벶׽�ж�
{
	uint8 TIMER_CH;
	if(capturech<=7)		     TIMER_CH=TIMER0;
	else if(capturech<=11)   TIMER_CH=TIMER1;
	else if(capturech<=15)   TIMER_CH=TIMER2;
	else if(capturech<=21)   TIMER_CH=TIMER3;
	else if(capturech<=22)   TIMER_CH=TIMER4;
	
	if(TIMER_CH == TIMER0 || TIMER_CH == TIMER1)	disable_irq((IRQn_Type)(CTIMER0_IRQn+TIMER_CH));
	else if(TIMER_CH == TIMER2 ) disable_irq((IRQn_Type)(CTIMER2_IRQn));
	else if(TIMER_CH == TIMER3 ) disable_irq((IRQn_Type)(CTIMER3_IRQn));
	else if(TIMER_CH == TIMER4 ) disable_irq((IRQn_Type)(CTIMER4_IRQn));
}

void CTIMER1_DriverIRQHandler(void)
{
	
}

void CTIMER2_DriverIRQHandler(void)
{
	
}

void CTIMER3_DriverIRQHandler(void)
{
	
}

void CTIMER4_DriverIRQHandler(void)
{
	
}

