/****************************************************************************
 * @�ļ�       SelfBuild_OLEDMenu.h
 * @����       ������
 * @����       �˵������ڵ�������
 * @ע��       �ó���ʹ�û���OLED�������������Ӧ��������ȷ�ϣ���FLASH��д
               ��һ�����������ɹرգ�����ֲʱ��ȷ����ʼ��
 * @���ʱ��   2018-4-25
****************************************************************************/
#ifndef __SELFBUILD_OLEDMENU_H
#define __SELFBUILD_OLEDMENU_H

#include "headfile.h"
#include "SEEKFREE_OLED.h"


extern int16 Date;
extern int16 Hour;
extern int16 Min;



/************************************��ֲ��������********************/
//���ż���
#define BUTTON_UP                       Button_Up
#define BUTTON_DOWN											Button_Down
#define BUTTON_LEFT											Button_Left
#define BUTTON_RIGHT	               		Button_Right
#define BUTTON_CONFIRM	                Button_Mid
#define BUZZER                          Beep




#define MENU_MAX                        10                                      //�˵��ĸ���
#define SUBMENU_MAX											4                                       //ÿ���˵��ܴ���������ı�����
#define MENU_SHOW(x)		        ((x == 0)? 0: OLED_X_MAX/2)
#define  FLASH_SAVE_RAW_MENU		(EEPROM_PAGE_COUNT  - 64)
#define  FLASH_SAVE_PRO_MENU		(EEPROM_PAGE_COUNT  - 128)

#define OLED_X_MAX 				X_WIDTH
#define OLED_Y_MAX 				Y_WIDTH
#define OLED_PAGE_MAX  		(OLED_Y_MAX/8)
#define OLED_WHITE 						(uint8_t)0XFF
#define OLED_BLACK							(uint8_t)0X00

typedef struct
{
	const char* Title;                                                      //�ò˵�������
	void* Sub_Value[SUBMENU_MAX];                                           //�ò˵�����������ı�����ֵ
	int8 Sub_Type[SUBMENU_MAX];                                             //�ò˵�����������ı���������
	const char* Sub_Title[SUBMENU_MAX];					//�ò˵�����������ı���������
	int8 Sub_Value_Num;
}Menu_TypeDef;

void Menu_Init(void);                                                           //��ʼ���˵�
uint8 Menu_Work(void);                                                          //�˵�����
//ʾ��д��
//DisableInterrupts();                          //�ر������жϣ���ֹ�˵����ڹ����г����ж�
//Menu_Init();                                  //��ʼ���˵�
//while(!Menu_Work()) systick_delay_ms(200);    //�˵�ÿ200ms����һ�Σ��������Ƿ��¡��رղ˵���ѡ��󣨺�������0��������ѭ��
//EnableInterrupts();
#endif