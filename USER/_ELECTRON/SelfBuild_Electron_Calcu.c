#include "SelfBuild_Electron_Calcu.h"



struct AD_Data_Typedef  AD_Data;

uint8 ADC_DataGet(void)	
{
	uint16 ad_temp[5][5]={0};
	
	for(int8 i=0;i<5;i++)	//取五次电感值
	{
		ad_temp[0][i]=adc_convert(AD_CH_R__, ADC_12BIT);
		ad_temp[1][i]=adc_convert(AD_CH_R_1, ADC_12BIT);
		ad_temp[2][i]=adc_convert(AD_CH_M_1, ADC_12BIT);
		ad_temp[3][i]=adc_convert(AD_CH_L_1, ADC_12BIT);
		ad_temp[4][i]=adc_convert(AD_CH_L__, ADC_12BIT);
	}
	for(int8 i=0;i<5;i++)//冒泡排序升序五次电感值
	{
		for(int8 j=0;j<4;j++)
		{
			for(int8 k=0;k<4-j;k++)
			{
				if(ad_temp[i][k] > ad_temp[i][k+1])        //前面的比后面的大  则进行交换
				{
					uint16 temp;
					temp = ad_temp[i][k+1];
					ad_temp[i][k+1] = ad_temp[i][k];
					ad_temp[i][k] = temp;
				}
			}
		}
	}
	//电感值归一化，排序后中间三个值得平均值作为当前电感值
    AD_Data.AD_R_R = (uint8)range_protect(100-(100*(((ad_temp[0][1] + ad_temp[0][2] + ad_temp[0][3]) / 3.0-ADC_R_R_Min)/ ((ADC_Max-ADC_R_R_Min)*1.0))),0,155);
	  AD_Data.AD_R   = (uint8)range_protect(100-(100*(((ad_temp[1][1] + ad_temp[1][2] + ad_temp[1][3]) / 3.0-ADC_R_Min)  / ((ADC_Max-ADC_R_Min)  *1.0))),0,155);
    AD_Data.AD_M   = (uint8)range_protect(100-(100*(((ad_temp[2][1] + ad_temp[2][2] + ad_temp[2][3]) / 3.0-ADC_M_Min)  / ((ADC_Max-ADC_M_Min)  *1.0))),0,155);
    AD_Data.AD_L   = (uint8)range_protect(100-(100*(((ad_temp[3][1] + ad_temp[3][2] + ad_temp[3][3]) / 3.0-ADC_L_Min)  / ((ADC_Max-ADC_L_Min)  *1.0))),0,155);
    AD_Data.AD_L_L = (uint8)range_protect(100-(100*(((ad_temp[4][1] + ad_temp[4][2] + ad_temp[4][3]) / 3.0-ADC_L_L_Min)/ ((ADC_Max-ADC_L_L_Min)*1.0))),0,155);

    return SUCCESS;
}

uint8 ADC_LastDataSave(void)	//保存上次AD值函数
{
    AD_Data.AD_L_Last   =  AD_Data.AD_L;
    AD_Data.AD_R_Last   =  AD_Data.AD_R;
    AD_Data.AD_L_L_Last =  AD_Data.AD_L_L;
    AD_Data.AD_R_R_Last =  AD_Data.AD_R_R;
    AD_Data.AD_M_Last   =  AD_Data.AD_M;
    return SUCCESS;	
}




