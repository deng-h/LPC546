/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#include "headfile.h"
#include "isr.h"

uint8 Flag_run=0;
/************************以下不可修改************************/
void PIN_INT7_DriverIRQHandler(void)
{
    mt9v032_vsync();        //总钻风场中断代码，当使用总钻风的时候执行该代码
}

void DMA0_DriverIRQHandler(void)
{
    if(READ_DMA_FLAG(MT9V032_DMA_CH))
    {
        CLEAR_DMA_FLAG(MT9V032_DMA_CH);
        mt9v032_dma();      //总钻风dma中断代码，当使用总钻风的时候执行该代码
    }
}



void FLEXCOMM5_DriverIRQHandler(void)
{
    vuint32 flag;
    flag = UART5_FIFO_FLAG;
    
    
    if(flag & USART_FIFOINTSTAT_RXLVL_MASK)//接收FIFO达到设定水平（库默认设定水平 当接收FIFO有一个数据的时候触发中断）
    {
        mt9v032_cof_uart_interrupt();
    }
    
    if(flag & USART_FIFOINTSTAT_RXERR_MASK)//接收FIFO错误
    {
        USART5->FIFOCFG  |= USART_FIFOCFG_EMPTYRX_MASK;//清空RX FIFO
        USART5->FIFOSTAT |= USART_FIFOSTAT_RXERR_MASK;
    }
}


void MRT0_DriverIRQHandler(void)
{
	
    if(MRT_FLAG_READ(MRT_CH0))
    {
        MRT_FLAG_CLR(MRT_CH0);
        Speed_read();//读取速度
//			  DataRead();//陀螺仪数据读取
//			  ADC_DataGet();//ADC读取	
			if(Flag_run)   //run的条件是拨码开关打开
			{
				g_nLeft_duty=PID_Incremental(&Left_motor_pid, g_nSpeedLeft, g_nSpeedLeft_set);//左轮pid计算
			  g_nRight_duty=PID_Incremental(&Right_motor_pid, g_nSpeedRight, g_nSpeedRight_set);//右轮pid计算
				PWM_OUT((int16)g_nLeft_duty,(int16)g_nRight_duty);//输出
			}
			else
			{
				
				PWM_OUT(0,0);//输出
			}
    }
    
    if(MRT_FLAG_READ(MRT_CH1))
    {
        MRT_FLAG_CLR(MRT_CH1);
       
    }
    
    if(MRT_FLAG_READ(MRT_CH2))
    {
        MRT_FLAG_CLR(MRT_CH2);
        
    }
    
    if(MRT_FLAG_READ(MRT_CH3))
    {
        MRT_FLAG_CLR(MRT_CH3);
        
    }
}

/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器 通道0得中断
void RIT_DriverIRQHandler(void)
{
    ;
}
记得进入中断后清除标志位

WDT_BOD_DriverIRQHandler
DMA0_DriverIRQHandler
GINT0_DriverIRQHandler
GINT1_DriverIRQHandler
PIN_INT0_DriverIRQHandler
PIN_INT1_DriverIRQHandler
PIN_INT2_DriverIRQHandler
PIN_INT3_DriverIRQHandler
UTICK0_DriverIRQHandler
MRT0_DriverIRQHandler
CTIMER0_DriverIRQHandler
CTIMER1_DriverIRQHandler
SCT0_DriverIRQHandler
CTIMER3_DriverIRQHandler
FLEXCOMM0_DriverIRQHandler
FLEXCOMM1_DriverIRQHandler
FLEXCOMM2_DriverIRQHandler
FLEXCOMM3_DriverIRQHandler
FLEXCOMM4_DriverIRQHandler
FLEXCOMM5_DriverIRQHandler
FLEXCOMM6_DriverIRQHandler
FLEXCOMM7_DriverIRQHandler
ADC0_SEQA_DriverIRQHandler
ADC0_SEQB_DriverIRQHandler
ADC0_THCMP_DriverIRQHandler
DMIC0_DriverIRQHandler
HWVAD0_DriverIRQHandler
USB0_NEEDCLK_DriverIRQHandler
USB0_DriverIRQHandler
RTC_DriverIRQHandler
Reserved46_DriverIRQHandler
Reserved47_DriverIRQHandler
PIN_INT4_DriverIRQHandler
PIN_INT5_DriverIRQHandler
PIN_INT6_DriverIRQHandler
PIN_INT7_DriverIRQHandler
CTIMER2_DriverIRQHandler
CTIMER4_DriverIRQHandler
RIT_DriverIRQHandler
SPIFI0_DriverIRQHandler
FLEXCOMM8_DriverIRQHandler
FLEXCOMM9_DriverIRQHandler
SDIO_DriverIRQHandler
CAN0_IRQ0_DriverIRQHandler
CAN0_IRQ1_DriverIRQHandler
CAN1_IRQ0_DriverIRQHandler
CAN1_IRQ1_DriverIRQHandler
USB1_DriverIRQHandler
USB1_NEEDCLK_DriverIRQHandler
ETHERNET_DriverIRQHandler
ETHERNET_PMT_DriverIRQHandler
ETHERNET_MACLP_DriverIRQHandler
EEPROM_DriverIRQHandler
LCD_DriverIRQHandler
SHA_DriverIRQHandler
SMARTCARD0_DriverIRQHandler
SMARTCARD1_DriverIRQHandler
*/



