/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		ADC
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_iocon.h"
#include "LPC546XX_gpio.h"
#include "LPC546XX_pll.h"
#include "LPC546XX_systick.h"
#include "LPC546XX_adc.h"

void adc_mux(ADCCH_enum ch)
{
    switch(ch)
    {
        case ADC_CH0_A10:   iocon_init(A10,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH1_A11:   iocon_init(A11,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH2_A12:   iocon_init(A12,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH3_A15:   iocon_init(A15,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH4_A16:   iocon_init(A16,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH5_A31:   iocon_init(A31,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH6_B0 :   iocon_init(B0 ,ALT0 | NOPULL | ANALOG | FILTEROFF); break;
        case ADC_CH11_A23:  iocon_init(A23,ALT0 | NOPULL | ANALOG | FILTEROFF); break;

        default:        ASSERT(0);//通道错误 进入断言失败
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      ch          ADC通道号
//  @return     void
//  Sample usage:           adc_init(ADC_CH0_A10);     // 初始化ADC通道0  分辨率为12位   通道对应的引脚为A10
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCCH_enum ch)
{
    uint16 temp_div;
    
    
    SYSCON->PDRUNCFGCLR[0] = ( 0
                             | SYSCON_PDRUNCFGCLR_PDEN_ADC0_MASK 
                             | SYSCON_PDRUNCFGCLR_PDEN_VD2_ANA_MASK 
                             | SYSCON_PDRUNCFGCLR_PDEN_VDDA_MASK
                             | SYSCON_PDRUNCFGCLR_PDEN_VREFP_MASK
                             ); //打开ADC电源
    systick_delay_us(20);                                       //必要延时
    
    
    SYSCON->ADCCLKSEL = SYSCON_ADCCLKSEL_SEL(0x01);             //选择ADC时钟源
    SYSCON->ADCCLKDIV = 0;  
    
    SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_HALT_MASK;  
    //SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_HALT_MASK | SYSCON_ADCCLKDIV_DIV(0) | SYSCON_ADCCLKDIV_RESET_MASK;  
    //while(!(SYSCON_ADCCLKDIV_REQFLAG_MASK & SYSCON->ADCCLKDIV));
    SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_DIV(0);
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_ADC0_MASK;     //打开ADC时钟
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_ADC0_RST_MASK; //清除复位ADC时钟
    
    
    adc_mux(ch);

    temp_div = (main_clk_mhz*100/80 + 99)/100;
    ADC0->CTRL = ( 0
                 | ADC_CTRL_CLKDIV(temp_div-1)      //分频最大不超过80M
                 //| ADC_CTRL_ASYNMODE_MASK         //模式      0:同步模式    1：异步模式   屏蔽为0
                 | ADC_CTRL_RESOL(0x3)              //默认12位分辨率
                 //| ADC_CTRL_BYPASSCAL_MASK        //采样校准  0:启用校准功能    1：关闭校准   屏蔽为0
                 | ADC_CTRL_TSAMP(0)                //采样周期
                 );

    ADC0->STARTUP = ADC_STARTUP_ADC_ENA_MASK;           //开启ADC
    systick_delay_us(10);                               //必要延时
    if (!(ADC0->STARTUP & ADC_STARTUP_ADC_ENA_MASK))
    {
        ASSERT(0);//ADC没有上电 进入断言失败
    }
    
    
    ADC0->CALIB = ADC_CALIB_CALIB_MASK;                 //ADC校准
    while(ADC_CALIB_CALIB_MASK == (ADC0->CALIB & ADC_CALIB_CALIB_MASK));
    
    ADC0->STARTUP |= ADC_STARTUP_ADC_INIT_MASK;         //ADC初始化
    while(ADC_STARTUP_ADC_INIT_MASK == (ADC0->STARTUP & ADC_STARTUP_ADC_INIT_MASK));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换
//  @param      ch          ADC通道号
//  @param      resolution  ADC分辨率
//  @return     void
//  Sample usage:           adc_test = adc_convert(ADC_CH0_A10,ADC_12BIT);     //采集ADC通道0电压值，分辨率12位     电压=adc_test*3300/2^n*(毫伏)   n为ADC的分辨率
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert(ADCCH_enum ch, ADCRES_enum resolution)
{
    ADC0->CTRL &= ~ADC_CTRL_RESOL_MASK;
    ADC0->CTRL |= ADC_CTRL_RESOL(resolution);   //分辨率
    
    ADC0->SEQ_CTRL[1] = 0;
    ADC0->SEQ_CTRL[1] =( 0
                        | ADC_SEQ_CTRL_CHANNELS(1<<ch)  //设置通道
                        | ADC_SEQ_CTRL_SEQ_ENA_MASK
                        | ADC_SEQ_CTRL_TRIGPOL_MASK
                        | ADC_SEQ_CTRL_SINGLESTEP_MASK
                       );

    ADC0->SEQ_CTRL[1] |= ADC_SEQ_CTRL_START_MASK;               //启动ADC转换

    while(!(ADC_SEQ_GDAT_DATAVALID_MASK & ADC0->SEQ_GDAT[1]));  //等待转换完成
    return ((ADC0->SEQ_GDAT[1]&ADC_SEQ_GDAT_RESULT_MASK)>>(ADC_SEQ_GDAT_RESULT_SHIFT+(3-resolution)*2));
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC采集
//  @param      adcn_ch         选择ADC通道
//  @param      bit             选择精度ADC_8bit
//  @return     void
//  @since      v1.0
//  Sample usage:               uint16 dat = adc_once(ADC0_SE8,ADC_16bit);//采集ADC0_SE8电压，精度16位
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_get_once(ADCCH_enum adcn_ch,ADCRES_enum  bit) //采集某路模拟量的AD值
{

	  ADC0->CTRL &= ~ADC_CTRL_RESOL_MASK;
    ADC0->CTRL |= ADC_CTRL_RESOL(bit);   //分辨率
    
    ADC0->SEQ_CTRL[1] = 0;
    ADC0->SEQ_CTRL[1] =( 0
                        | ADC_SEQ_CTRL_CHANNELS(1<<adcn_ch)  //设置通道
                        | ADC_SEQ_CTRL_SEQ_ENA_MASK
                        | ADC_SEQ_CTRL_TRIGPOL_MASK
                        | ADC_SEQ_CTRL_SINGLESTEP_MASK
                       );

    ADC0->SEQ_CTRL[1] |= ADC_SEQ_CTRL_START_MASK;               //启动ADC转换

    while(!(ADC_SEQ_GDAT_DATAVALID_MASK & ADC0->SEQ_GDAT[1]));  //等待转换完成
    return (((ADC0->SEQ_GDAT[1]&ADC_SEQ_GDAT_RESULT_MASK)>>(ADC_SEQ_GDAT_RESULT_SHIFT+(3-bit)*2))*3300);
}

/********************************FFT相关变量*****************************************/
int16 cos_sin_search[11][2]=		//正余弦查表
{							//k=
  { 100,  00},			//0
  {  81,  59},			//1
  {  31,  95},			//2
  { -31,  95},			//3
  { -81,  59},		    //4
  {-100,  00},			//5
  { -81 ,-59},			//6
  { -31, -95},			//7
  {  31, -95},			//8
  {  81, -59},			//9
  { 100,  00}				//10
};


int adc_data_buff[50];	//ADC缓冲
uint8 finish_flag=0;	//完成标志
ADCCH_enum FFT_ADCadcn_ch;
ADCRES_enum  FFT_ADCbit;
ADC_Process_Typedef ADC_Process[10];
/****************************************************************************************/

static uint16 adc_catch_once_FFT(ADCCH_enum adcn_ch, ADCRES_enum bit); //采集某路模拟量的AD值
static void Data_Filter_ForADC(ADC_Process_Typedef *ADC_Process);	// 数据滤波
//*************************FFT采集********************************/

/*!
 *  @brief      PIT的ADC采样定时器中断服务函数
 *  @param      void
 *  @since      v1.0
 *  @note       如果修改了MK60_adc.h文件内的ADC_PIT宏定义值，需要修改此处中断服务函数名内的CHn
*  Sample usage:       如果#ADC_PIT PIT3   则中断服务函数名为PIT_CH3_IRQHandler
 */
void RIT_DriverIRQHandler(void)
{
    static uint16 k;
	adc_data_buff[++k]=adc_get_once(FFT_ADCadcn_ch,  FFT_ADCbit);	//中断共进入20次，每次采集一次放入数组中
	if(k >= 20)
	{
	  	finish_flag=1;		//置位采集完成标志位
		k=0;
		disable_irq(RIT_IRQn);
    
	}
	 PIT_FLAG_CLEAR;		//清除定时器中断标志位
}


/*!
 *  @brief      使用FFT方式采集20KHz信号基波幅值并滑动滤波
 *  @param      adcn_ch  ADC通道号
 *  @param      ADC_nbit ADC转换位数（一般为8位）
 *  @since      v1.0
 *  @note       调用此函数则采集一个通道,并滤波，
*  Sample usage:       
 */
uint16 adc_once(ADCCH_enum adcn_ch, ADCRES_enum bit)
{
  	  uint8 serial;
  	switch(adcn_ch)
	  {
	  case AD0: serial=0;break;
	  case AD1: serial=1;break;
	  case AD2: serial=2;break;
	  case AD3: serial=3;break;
	  case AD4: serial=4;break;
	  case AD5: serial=5;break;
	  default: while(1);
			
	//			default:break;
	  }
	  
	  ADC_Process[serial].ADC_CHn_result= adc_catch_once_FFT(adcn_ch, ADC_8BIT);
	  Data_Filter_ForADC(&ADC_Process[serial]);
	  return ADC_Process[serial].ADC_CHn_result_real;
}


/*!
 *  @brief      滑动滤波函数
 *  @param      ADC_Process  需要传入的需要滑动滤波的结构体指针
 *  @since      v1.0
 *  @note       
 *  Sample usage:       result=FFT(adc_data_buff)    //将adc_data_buff的数据传入，输出基波幅值
 */

static void Data_Filter_ForADC(ADC_Process_Typedef *ADC_Process)	// 数据滤波
{	
  	ADC_Process->BUF_SUM -= ADC_Process->BUF[ADC_Process->point];		//清除位历史缓存数据
	
	ADC_Process->BUF[ADC_Process->point] = ADC_Process->ADC_CHn_result;	//位缓存数据重新赋值
		
  	ADC_Process->BUF_SUM += ADC_Process->BUF[ADC_Process->point];		//将新值重新计入总量
	
	ADC_Process->point= (ADC_Process->point == (FILTER_NUM-1))?0:ADC_Process->point+1;	//指针刷新
	
	ADC_Process->ADC_CHn_result_real = (uint16)(ADC_Process->BUF_SUM / FILTER_NUM / Ratio);	//对缓存区内数据作平均和滤波
}


/*!
 *  @brief      FFT基波幅值计算函数
 *  @param      *adc_data    传入的ADC4个周期采样值数组
 *  @since      v1.0
 *  @note       
 *  Sample usage:       result=FFT(adc_data_buff)    //将adc_data_buff的数据传入，输出基波幅值
 */
static float FFT(int *adc_data)
{
	int32 An,Bn;
	for(uint8 k=1;k<=10;k++)
	{
	  An+= 5*adc_data[k+1*10] * cos_sin_search[k][0];
	  Bn+= 5*adc_data[k+1*10] * cos_sin_search[k][1];
	}
	An =(int32)(An/100.0);
	Bn =(int32)(Bn/100.0);
	return ((An*An+Bn*Bn)==0 ? 0 : sqrt((An*An+Bn*Bn)*25));	//返回基波幅值
}


/*!
 *  @brief      使用FFT方式采集20KHz信号基波幅值(不支持B通道)
 *  @param      adcn_ch  ADC通道号
 *  @param      ADC_nbit ADC转换位数（一般为8位）
 *  @return     采样值
 *  @since      v1.0
 *  Sample usage:       uint16 var = adc_once();
 */
static uint16 adc_catch_once_FFT(ADCCH_enum adcn_ch,ADCRES_enum bit) //采集某路模拟量的AD值
{
  uint16 result=0;
  FFT_ADCadcn_ch = adcn_ch;
	FFT_ADCbit     = bit;
	pit_init_us(5);				//设置定时器中断5us
//	set_vector_handler(PIT3_VECTORn ,PIT_CH3_IRQHandler);
	set_irq_priority(RTC_IRQn , 0);		//设置为最高优先级，必须为最高优先级
  enable_irq(RTC_IRQn);		//使能采集
  while(!finish_flag);	//等待采集完成
	finish_flag=0;				//清零标志位
	result=(uint16)FFT(adc_data_buff);

	memset(&adc_data_buff,0,sizeof(adc_data_buff));	//清零数组
	return result;
}



