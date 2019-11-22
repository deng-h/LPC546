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

struct AD_Data_Typedef	//��Ŵ�����������ؽṹ��
{
	uint8 AD_L_L;	//�������ֵ
	uint8 AD_L_L_Last;	//�������ֵ��һ��
	
	uint8 AD_L;	//�����ֵ
	uint8 AD_L_Last;	//�����ֵ��һ��
	
	uint8 AD_M;	//�м���ֵ
	uint8 AD_M_Last;	//�м���ֵ��һ��
	
	uint8 AD_R;	//�Ҳ���ֵ
	uint8 AD_R_Last;	//�Ҳ���ֵ��һ��
	
	uint8 AD_R_R;	//���Ҳ���ֵ
	uint8 AD_R_R_Last;	//���Ҳ���ֵ��һ��
};
#define	FLASH_SAVE_MCP41				(EEPROM_PAGE_COUNT - 23)

#define ADC_R_R_Min_8bit 100//������Сֵ
#define ADC_R_Min_8bit   70
#define ADC_M_Min_8bit   100
#define ADC_L_Min_8bit   70
#define ADC_L_L_Min_8bit 100

extern void Correct_Sensor(void);
extern void Normalized_MCP41(void);


#endif

