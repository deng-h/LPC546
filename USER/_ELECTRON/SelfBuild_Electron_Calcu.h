#ifndef __SELFBUILD_ELECTRON_CALCU_H_
#define __SELFBUILD_ELECTRON_CALCU_H_

#include "headfile.h"

#define ADC_Max 3103//�������ֵ
#define ADC_R_R_Min ADC_R_R_Min_8bit*(ADC_Max/193)	//������Сֵ
#define ADC_R_Min   ADC_R_Min_8bit*(ADC_Max/193)
#define ADC_M_Min   ADC_M_Min_8bit*(ADC_Max/193)
#define ADC_L_Min   ADC_L_Min_8bit*(ADC_Max/193)
#define ADC_L_L_Min ADC_L_L_Min_8bit*(ADC_Max/193)

extern struct AD_Data_Typedef    AD_Data;




uint8 ADC_DataGet(void);

#endif
