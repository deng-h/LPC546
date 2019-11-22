#ifndef _SELFBUILD_CORRECT_SENSOR_H
#define _SELFBUILD_CORRECT_SENSOR_H

#include "headfile.h"

typedef enum
{
  L__ = 0,
  L_1,
  M_1,
  R_1,
  R__,
  ADC_NUM,
}ADC_POSITION;

struct AD_Data_Typedef	//电磁传感器数据相关结构体
{
	uint8 AD_L_L;	//最左侧电感值
	uint8 AD_L_L_Last;	//最左侧电感值上一次
	
	uint8 AD_L;	//左侧电感值
	uint8 AD_L_Last;	//左侧电感值上一次
	
	uint8 AD_M;	//中间电感值
	uint8 AD_M_Last;	//中间电感值上一次
	
	uint8 AD_R;	//右侧电感值
	uint8 AD_R_Last;	//右侧电感值上一次
	
	uint8 AD_R_R;	//最右侧电感值
	uint8 AD_R_R_Last;	//最右侧电感值上一次
};
#define	FLASH_SAVE_MCP41				(EEPROM_PAGE_COUNT - 23)

#define ADC_R_R_Min_8bit 100//五电感最小值
#define ADC_R_Min_8bit   70
#define ADC_M_Min_8bit   100
#define ADC_L_Min_8bit   70
#define ADC_L_L_Min_8bit 100

extern void Correct_Sensor(void);
extern void Normalized_MCP41(void);


#endif

