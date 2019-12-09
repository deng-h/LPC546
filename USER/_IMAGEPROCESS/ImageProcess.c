#include "ImageProcess.h"


uint8 CenterPoint[100];  //��������ߵ�x����
int qulv_near_right=0, qulv_near_left=0 ,qulv_global_right=0 ,qulv_global_left=0; //���ʽ�����ȫ��
int qulv_far_right=0 ,qulv_far_left=0; //����Զ��
uint8 RdiuxianFlg[100]={0},LdiuxianFlg[100]={0};
uint8 DuanDian=0;
uint8 RoadWidth[100] ={172,172,172,172,172,171,171,171,171,171,
											 169,169,169,169,169,164,164,164,164,164,
											 159,159,159,159,159,156,156,156,156,156,
											 150,150,150,150,150,146,146,146,146,146,
											 140,140,140,140,140,134,134,134,134,134,
											 127,127,127,127,127,121,121,121,121,121,
											 115,115,115,115,115,109,109,109,109,109,
											 103,103,103,103,103,97,97,97,97,97,
											 90,90,90,90,90,82,82,82,82,82,
											 76,76,76,76,76,66,66,66,66,66};

/*
ɨ�߳��򣬶���0~99�У��ҵ���ɫ��߽�Ͱ�ɫ�ұ߽磬�Ӷ�ȷ����ɫ���������λ��
OLED�ĵ�����height(188)��yΪ188ʱ�п�����OLED�Ͽ������п���ʱOLED��Ļ��С��ͼ��ߴ粻һ��
�����е�height��Ӧbinary�����x��width��Ӧ�����y
*/
void ScanLine(uint8 * imageRet, int width, int height)  
{
	int xLeft = 0, xRight = 0 , index = 0 ;
	int temp;
	
	qulv_far_right = qulv_far_left = qulv_near_right = qulv_near_left = qulv_global_right = qulv_global_left = 0;  //��������
	DuanDian=0;
	
	memset(CenterPoint,0,sizeof(CenterPoint)); //����
	memset(RdiuxianFlg,0,sizeof(RdiuxianFlg));
	memset(LdiuxianFlg,0,sizeof(LdiuxianFlg));
	
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
		
	for (int yNow = height - 2; yNow > height - 100; --yNow)  //�ӵ��濪ʼɨ��100��
	{
		int xLeftNow = 0, xRightNow = 0;

		if (xLeft > 0)
		{
			for (xLeftNow = xLeft; xLeftNow > 0 && IMAGE_DATA(xLeftNow, yNow) == WHITEimg && IMAGE_DATA(xLeftNow + 1, yNow) == WHITEimg; --xLeftNow); //���������������ƣ���ת
			if (xLeftNow == xLeft) //�߽��ڸ��ұߣ�һ����ֱ������ת������
			{
				for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow);
			}
		}
		else 
		{
			for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow); 
			if(xLeftNow - xLeft <= 1) LdiuxianFlg[yNow] = 1;   //����
		}
		
		if (xRight < width - 1)  
		{
			for (xRightNow = xRight; xRightNow < width && IMAGE_DATA(xRightNow, yNow) == WHITEimg && IMAGE_DATA(xRightNow - 1, yNow) == WHITEimg; ++xRightNow);
			if (xRightNow == xRight)
			{
				for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow);
			}
		}
		else 
		{
			for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow); //�Ҷ���
			if(xRight - xRightNow <= 1) RdiuxianFlg[yNow] = 1;  //�Ҷ���
		}
		
//		if((1 == LdiuxianFlg[yNow]) && (0 == RdiuxianFlg[yNow]))  //����
//		{
//			xLeftNow = xRightNow - RoadWidth[119 - yNow];
//		}
//		if((0 == LdiuxianFlg[yNow]) && (1 == RdiuxianFlg[yNow]))
//		{
//			xRightNow = xLeftNow + RoadWidth[119 - yNow];
//		}
		
//		if(((1 == LdiuxianFlg[yNow]) || (1 == RdiuxianFlg[yNow])) && yNow > 100) DuanDian++;   //���������Ҷ��ߣ��ϵ�++
		
		
		
		if (xLeftNow < xRightNow)
		{
			xLeft = xLeftNow; xRight = xRightNow;
			IMAGE_DATA((xLeft + xRight) / 2, yNow) = LINE;
			IMAGE_DATA((xLeft + xRight) / 2 + 1, yNow) = LINE;
			IMAGE_DATA((xLeft + xRight) / 2 - 1, yNow) = LINE;
			CenterPoint[index++] = (xLeft + xRight) / 2;
			if(index >= 100) index = 99;
			if(1 == LdiuxianFlg[yNow]) xLeft = 0;
			if(1 == RdiuxianFlg[yNow]) xRight = width - 1;
		}
		else break;
		
		/*  ���ʼ���  */
//		if(yNow > 70)  //������48��, ���ʷ�Χ-48~48
//		{
//			if(CenterPoint[index-1] - CenterPoint[index-2] > 0) qulv_near_right++;
//			if(CenterPoint[index-1] - CenterPoint[index-2] < 0) qulv_near_left++;
//		}
//		if(yNow > 25 && yNow < 70)  //Զ��43��,���ʷ�Χ-43~43
//		{
//			if(CenterPoint[index-1] - CenterPoint[index-2] > 0) qulv_far_right++;
//			if(CenterPoint[index-1] - CenterPoint[index-2] < 0) qulv_far_left++;
//		}
//		if(yNow > 30 && yNow < 110) //����78��,���ʷ�Χ-78~78
//		{
//			if(CenterPoint[index-1] - CenterPoint[index-2] > 0) qulv_global_right++;
//			if(CenterPoint[index-1] - CenterPoint[index-2] < 0) qulv_global_left++;
//		}
		/*  ���ʼ������  */
		
	}//��ѭ������
}


/*
���������ߵ�б��
���������Իع鷽����б��
����:��ʼ�к���ֹ��
����ֵ:б��
*/
float CenterSlope(int startline, int endline)
{
	float Slope=0.0;
	if(endline >= 100) endline = 99;
	int i;
	int sumX=0,sumY=0,averX=0,averY=0;
	int num=0,B_up1=0,B_up2=0,B_up,B_down;  //up�Ƿ��ӣ�down�Ƿ�ĸ������������
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
		B_up += (int)(10*B_up1*B_up2);
		B_up=B_up/100*100;
		B_down += (int)(10*((i-averX)*(i-averX)));
   }
   
   if(0 == B_down) Slope=0.0;
   else Slope = 16 * B_up/B_down;
   
   return Slope;
}










