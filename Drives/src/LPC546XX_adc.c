/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ADC
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴LPC546XX_config.h�ļ��ڰ汾�궨��
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

        default:        ASSERT(0);//ͨ������ �������ʧ��
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      ch          ADCͨ����
//  @return     void
//  Sample usage:           adc_init(ADC_CH0_A10);     // ��ʼ��ADCͨ��0  �ֱ���Ϊ12λ   ͨ����Ӧ������ΪA10
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCCH_enum ch)
{
    uint16 temp_div;
    
    
    SYSCON->PDRUNCFGCLR[0] = ( 0
                             | SYSCON_PDRUNCFGCLR_PDEN_ADC0_MASK 
                             | SYSCON_PDRUNCFGCLR_PDEN_VD2_ANA_MASK 
                             | SYSCON_PDRUNCFGCLR_PDEN_VDDA_MASK
                             | SYSCON_PDRUNCFGCLR_PDEN_VREFP_MASK
                             ); //��ADC��Դ
    systick_delay_us(20);                                       //��Ҫ��ʱ
    
    
    SYSCON->ADCCLKSEL = SYSCON_ADCCLKSEL_SEL(0x01);             //ѡ��ADCʱ��Դ
    SYSCON->ADCCLKDIV = 0;  
    
    SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_HALT_MASK;  
    //SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_HALT_MASK | SYSCON_ADCCLKDIV_DIV(0) | SYSCON_ADCCLKDIV_RESET_MASK;  
    //while(!(SYSCON_ADCCLKDIV_REQFLAG_MASK & SYSCON->ADCCLKDIV));
    SYSCON->ADCCLKDIV = SYSCON_ADCCLKDIV_DIV(0);
    
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL_ADC0_MASK;     //��ADCʱ��
    SYSCON->PRESETCTRLCLR[0] = SYSCON_PRESETCTRL_ADC0_RST_MASK; //�����λADCʱ��
    
    
    adc_mux(ch);

    temp_div = (main_clk_mhz*100/80 + 99)/100;
    ADC0->CTRL = ( 0
                 | ADC_CTRL_CLKDIV(temp_div-1)      //��Ƶ��󲻳���80M
                 //| ADC_CTRL_ASYNMODE_MASK         //ģʽ      0:ͬ��ģʽ    1���첽ģʽ   ����Ϊ0
                 | ADC_CTRL_RESOL(0x3)              //Ĭ��12λ�ֱ���
                 //| ADC_CTRL_BYPASSCAL_MASK        //����У׼  0:����У׼����    1���ر�У׼   ����Ϊ0
                 | ADC_CTRL_TSAMP(0)                //��������
                 );

    ADC0->STARTUP = ADC_STARTUP_ADC_ENA_MASK;           //����ADC
    systick_delay_us(10);                               //��Ҫ��ʱ
    if (!(ADC0->STARTUP & ADC_STARTUP_ADC_ENA_MASK))
    {
        ASSERT(0);//ADCû���ϵ� �������ʧ��
    }
    
    
    ADC0->CALIB = ADC_CALIB_CALIB_MASK;                 //ADCУ׼
    while(ADC_CALIB_CALIB_MASK == (ADC0->CALIB & ADC_CALIB_CALIB_MASK));
    
    ADC0->STARTUP |= ADC_STARTUP_ADC_INIT_MASK;         //ADC��ʼ��
    while(ADC_STARTUP_ADC_INIT_MASK == (ADC0->STARTUP & ADC_STARTUP_ADC_INIT_MASK));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCת��
//  @param      ch          ADCͨ����
//  @param      resolution  ADC�ֱ���
//  @return     void
//  Sample usage:           adc_test = adc_convert(ADC_CH0_A10,ADC_12BIT);     //�ɼ�ADCͨ��0��ѹֵ���ֱ���12λ     ��ѹ=adc_test*3300/2^n*(����)   nΪADC�ķֱ���
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert(ADCCH_enum ch, ADCRES_enum resolution)
{
    ADC0->CTRL &= ~ADC_CTRL_RESOL_MASK;
    ADC0->CTRL |= ADC_CTRL_RESOL(resolution);   //�ֱ���
    
    ADC0->SEQ_CTRL[1] = 0;
    ADC0->SEQ_CTRL[1] =( 0
                        | ADC_SEQ_CTRL_CHANNELS(1<<ch)  //����ͨ��
                        | ADC_SEQ_CTRL_SEQ_ENA_MASK
                        | ADC_SEQ_CTRL_TRIGPOL_MASK
                        | ADC_SEQ_CTRL_SINGLESTEP_MASK
                       );

    ADC0->SEQ_CTRL[1] |= ADC_SEQ_CTRL_START_MASK;               //����ADCת��

    while(!(ADC_SEQ_GDAT_DATAVALID_MASK & ADC0->SEQ_GDAT[1]));  //�ȴ�ת�����
    return ((ADC0->SEQ_GDAT[1]&ADC_SEQ_GDAT_RESULT_MASK)>>(ADC_SEQ_GDAT_RESULT_SHIFT+(3-resolution)*2));
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      bit             ѡ�񾫶�ADC_8bit
//  @return     void
//  @since      v1.0
//  Sample usage:               uint16 dat = adc_once(ADC0_SE8,ADC_16bit);//�ɼ�ADC0_SE8��ѹ������16λ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_get_once(ADCCH_enum adcn_ch,ADCRES_enum  bit) //�ɼ�ĳ·ģ������ADֵ
{

	  ADC0->CTRL &= ~ADC_CTRL_RESOL_MASK;
    ADC0->CTRL |= ADC_CTRL_RESOL(bit);   //�ֱ���
    
    ADC0->SEQ_CTRL[1] = 0;
    ADC0->SEQ_CTRL[1] =( 0
                        | ADC_SEQ_CTRL_CHANNELS(1<<adcn_ch)  //����ͨ��
                        | ADC_SEQ_CTRL_SEQ_ENA_MASK
                        | ADC_SEQ_CTRL_TRIGPOL_MASK
                        | ADC_SEQ_CTRL_SINGLESTEP_MASK
                       );

    ADC0->SEQ_CTRL[1] |= ADC_SEQ_CTRL_START_MASK;               //����ADCת��

    while(!(ADC_SEQ_GDAT_DATAVALID_MASK & ADC0->SEQ_GDAT[1]));  //�ȴ�ת�����
    return (((ADC0->SEQ_GDAT[1]&ADC_SEQ_GDAT_RESULT_MASK)>>(ADC_SEQ_GDAT_RESULT_SHIFT+(3-bit)*2))*3300);
}

/********************************FFT��ر���*****************************************/
int16 cos_sin_search[11][2]=		//�����Ҳ��
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


int adc_data_buff[50];	//ADC����
uint8 finish_flag=0;	//��ɱ�־
ADCCH_enum FFT_ADCadcn_ch;
ADCRES_enum  FFT_ADCbit;
ADC_Process_Typedef ADC_Process[10];
/****************************************************************************************/

static uint16 adc_catch_once_FFT(ADCCH_enum adcn_ch, ADCRES_enum bit); //�ɼ�ĳ·ģ������ADֵ
static void Data_Filter_ForADC(ADC_Process_Typedef *ADC_Process);	// �����˲�
//*************************FFT�ɼ�********************************/

/*!
 *  @brief      PIT��ADC������ʱ���жϷ�����
 *  @param      void
 *  @since      v1.0
 *  @note       ����޸���MK60_adc.h�ļ��ڵ�ADC_PIT�궨��ֵ����Ҫ�޸Ĵ˴��жϷ��������ڵ�CHn
*  Sample usage:       ���#ADC_PIT PIT3   ���жϷ�������ΪPIT_CH3_IRQHandler
 */
void RIT_DriverIRQHandler(void)
{
    static uint16 k;
	adc_data_buff[++k]=adc_get_once(FFT_ADCadcn_ch,  FFT_ADCbit);	//�жϹ�����20�Σ�ÿ�βɼ�һ�η���������
	if(k >= 20)
	{
	  	finish_flag=1;		//��λ�ɼ���ɱ�־λ
		k=0;
		disable_irq(RIT_IRQn);
    
	}
	 PIT_FLAG_CLEAR;		//�����ʱ���жϱ�־λ
}


/*!
 *  @brief      ʹ��FFT��ʽ�ɼ�20KHz�źŻ�����ֵ�������˲�
 *  @param      adcn_ch  ADCͨ����
 *  @param      ADC_nbit ADCת��λ����һ��Ϊ8λ��
 *  @since      v1.0
 *  @note       ���ô˺�����ɼ�һ��ͨ��,���˲���
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
 *  @brief      �����˲�����
 *  @param      ADC_Process  ��Ҫ�������Ҫ�����˲��Ľṹ��ָ��
 *  @since      v1.0
 *  @note       
 *  Sample usage:       result=FFT(adc_data_buff)    //��adc_data_buff�����ݴ��룬���������ֵ
 */

static void Data_Filter_ForADC(ADC_Process_Typedef *ADC_Process)	// �����˲�
{	
  	ADC_Process->BUF_SUM -= ADC_Process->BUF[ADC_Process->point];		//���λ��ʷ��������
	
	ADC_Process->BUF[ADC_Process->point] = ADC_Process->ADC_CHn_result;	//λ�����������¸�ֵ
		
  	ADC_Process->BUF_SUM += ADC_Process->BUF[ADC_Process->point];		//����ֵ���¼�������
	
	ADC_Process->point= (ADC_Process->point == (FILTER_NUM-1))?0:ADC_Process->point+1;	//ָ��ˢ��
	
	ADC_Process->ADC_CHn_result_real = (uint16)(ADC_Process->BUF_SUM / FILTER_NUM / Ratio);	//�Ի�������������ƽ�����˲�
}


/*!
 *  @brief      FFT������ֵ���㺯��
 *  @param      *adc_data    �����ADC4�����ڲ���ֵ����
 *  @since      v1.0
 *  @note       
 *  Sample usage:       result=FFT(adc_data_buff)    //��adc_data_buff�����ݴ��룬���������ֵ
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
	return ((An*An+Bn*Bn)==0 ? 0 : sqrt((An*An+Bn*Bn)*25));	//���ػ�����ֵ
}


/*!
 *  @brief      ʹ��FFT��ʽ�ɼ�20KHz�źŻ�����ֵ(��֧��Bͨ��)
 *  @param      adcn_ch  ADCͨ����
 *  @param      ADC_nbit ADCת��λ����һ��Ϊ8λ��
 *  @return     ����ֵ
 *  @since      v1.0
 *  Sample usage:       uint16 var = adc_once();
 */
static uint16 adc_catch_once_FFT(ADCCH_enum adcn_ch,ADCRES_enum bit) //�ɼ�ĳ·ģ������ADֵ
{
  uint16 result=0;
  FFT_ADCadcn_ch = adcn_ch;
	FFT_ADCbit     = bit;
	pit_init_us(5);				//���ö�ʱ���ж�5us
//	set_vector_handler(PIT3_VECTORn ,PIT_CH3_IRQHandler);
	set_irq_priority(RTC_IRQn , 0);		//����Ϊ������ȼ�������Ϊ������ȼ�
  enable_irq(RTC_IRQn);		//ʹ�ܲɼ�
  while(!finish_flag);	//�ȴ��ɼ����
	finish_flag=0;				//�����־λ
	result=(uint16)FFT(adc_data_buff);

	memset(&adc_data_buff,0,sizeof(adc_data_buff));	//��������
	return result;
}



