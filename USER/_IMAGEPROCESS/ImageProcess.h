#ifndef _IMAGEPROCESS_h
#define _IMAGEPROCESS_h

#include "headfile.h"

#define IMAGE_DATA(x, y)  CameraBinary[y][x]  //���껥��һ��                                                   
#define BLACKimg 0
#define LINE		 0
#define WHITEimg 1

extern uint8  CenterPoint[100];  //��������ߵ�x����

void ScanLine(uint8 * imageRet, int width, int height);
float CenterSlope(int startline, int endline);  //����б��
#endif

