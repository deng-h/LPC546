#include "ImageProcess.h"

uint8  CenterPoint[100];  //��������ߵ�x����


/*
ɨ�߳��򣬶���0~99�У��ҵ���ɫ��߽�Ͱ�ɫ�ұ߽磬�Ӷ�ȷ����ɫ���������λ��
OLED�ĵ�����height(188)��yΪ188ʱ�п�����OLED�Ͽ������п���ʱOLED��Ļ��С��ͼ��ߴ粻һ��
�����е�height��Ӧbinary�����x��width��Ӧ�����y
*/
void ScanLine(uint8 * imageRet, int width, int height)  
{
	int xLeft = 0, xRight = 0 ,i = 0, index = 0;
	int temp;
	for(i = 0;i < 100;i++) CenterPoint[i] = 0;  
	for (temp = 0; temp < width - 3; ++temp)  //��������ɨ 
		if (IMAGE_DATA(temp, height-1) == WHITEimg && IMAGE_DATA(temp + 1, height-1) == WHITEimg && IMAGE_DATA(temp + 2, height-1) == WHITEimg && IMAGE_DATA(temp + 3, height-1) == WHITEimg)
			break;
	xLeft = temp;
	for (temp = width - 1; temp >= 3; --temp) //��������ɨ 
		if (IMAGE_DATA(temp, height-1) == WHITEimg && IMAGE_DATA(temp - 1, height-1) == WHITEimg && IMAGE_DATA(temp - 2, height-1) == WHITEimg && IMAGE_DATA(temp - 3, height-1) == WHITEimg)
			break;
	xRight = temp;
	IMAGE_DATA((xLeft + xRight) / 2, height-1) = LINE; //�����ߴ�һ��
	IMAGE_DATA((xLeft + xRight) / 2 + 1, height-1) = LINE;
	IMAGE_DATA((xLeft + xRight) / 2 - 1, height-1) = LINE;
		
	CenterPoint[index++] = (xLeft + xRight) / 2;  
		
	for (int yNow = height-2; yNow > height - 100; --yNow)  //�ӵ��濪ʼɨ��100��
	{
		int xLeftNow = 0, xRightNow = 0;

		if (xLeft > 0)
		{
			for (xLeftNow = xLeft; xLeftNow >= 0 && IMAGE_DATA(xLeftNow, yNow) == WHITEimg && IMAGE_DATA(xLeftNow + 1, yNow) == WHITEimg; --xLeftNow);//???????? 
			if (xLeftNow == xLeft)//������û�а׵���
			{
				for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow);//��߽������Ұ׵�
			}
		}
		else for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow);  

		if (xRight < width - 1)  
		{
			for (xRightNow = xRight; xRightNow < width && IMAGE_DATA(xRightNow, yNow) == WHITEimg && IMAGE_DATA(xRightNow - 1, yNow) == WHITEimg; ++xRightNow);//????????
			if (xRightNow == xRight)//����ұ�û�а׵���
			{
				for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow);//�ұ߽������Ұ׵�
			}
		}
		else for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow);//�ұ߽������Ұ׵� 

		if (xLeftNow < xRightNow)
		{
			xLeft = xLeftNow; xRight = xRightNow;
			IMAGE_DATA((xLeft + xRight) / 2, yNow) = LINE;
			IMAGE_DATA((xLeft + xRight) / 2 + 1, yNow) = LINE;
			IMAGE_DATA((xLeft + xRight) / 2 - 1, yNow) = LINE;
			CenterPoint[index++] = (xLeft + xRight) / 2;
			if(index >= 100) index = 99;
		}
		else break;
	}
}


/*
���������ߵ�б��
���������Իع鷽����б��
param:��ʼ�к���ֹ��
return:б��
*/
float CenterSlope(int startline, int endline)
{
	if(endline >= 100) endline = 99;
	int i;
	int sumX=0,sumY=0,averX=0,averY=0;
	int num=0,B_up1=0,B_up2=0,B_up,B_down;  //up�Ƿ��ӣ�down�Ƿ�ĸ������������
	float B;
	for(i=startline;i<=endline;i++)
  {
		num++;
    sumX += i;
    sumY += CenterPoint[i];
  }
	 averX=sumX/num;  //���x�����y�����ƽ��ֵ
   averY=sumY/num;
   B_up=0;
   B_down=0;
   for(i=startline;i<=endline;i++)  //Sigma
   {   
		 B_up1=(int)(CenterPoint[i]-averY);
		 B_up2=i-averX;
		 B_up += (int)(B_up1*B_up2);
		 B_down += (int)((i-averX)*(i-averX));
   }
   if(B_down==0) B=0.0;
   else B = B_up/B_down;
   return B;
}










