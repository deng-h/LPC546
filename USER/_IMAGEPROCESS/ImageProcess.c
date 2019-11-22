#include "ImageProcess.h"

uint8  CenterPoint[100];  //存放中心线的x坐标


/*
扫线程序，对于0~99行，找到白色左边界和白色右边界，从而确定白色区域的中心位置
OLED的底面是height(188)，y为188时有可能在OLED上看不到有可能时OLED屏幕大小与图像尺寸不一样
函数中的height对应binary数组的x，width对应数组的y
*/
void ScanLine(uint8 * imageRet, int width, int height)  
{
	int xLeft = 0, xRight = 0 ,i = 0, index = 0;
	int temp;
	for(i = 0;i < 100;i++) CenterPoint[i] = 0;  
	for (temp = 0; temp < width - 3; ++temp)  //从左往右扫 
		if (IMAGE_DATA(temp, height-1) == WHITEimg && IMAGE_DATA(temp + 1, height-1) == WHITEimg && IMAGE_DATA(temp + 2, height-1) == WHITEimg && IMAGE_DATA(temp + 3, height-1) == WHITEimg)
			break;
	xLeft = temp;
	for (temp = width - 1; temp >= 3; --temp) //从右往左扫 
		if (IMAGE_DATA(temp, height-1) == WHITEimg && IMAGE_DATA(temp - 1, height-1) == WHITEimg && IMAGE_DATA(temp - 2, height-1) == WHITEimg && IMAGE_DATA(temp - 3, height-1) == WHITEimg)
			break;
	xRight = temp;
	IMAGE_DATA((xLeft + xRight) / 2, height-1) = LINE; //中线线粗一点
	IMAGE_DATA((xLeft + xRight) / 2 + 1, height-1) = LINE;
	IMAGE_DATA((xLeft + xRight) / 2 - 1, height-1) = LINE;
		
	CenterPoint[index++] = (xLeft + xRight) / 2;  
		
	for (int yNow = height-2; yNow > height - 100; --yNow)  //从底面开始扫，100行
	{
		int xLeftNow = 0, xRightNow = 0;

		if (xLeft > 0)
		{
			for (xLeftNow = xLeft; xLeftNow >= 0 && IMAGE_DATA(xLeftNow, yNow) == WHITEimg && IMAGE_DATA(xLeftNow + 1, yNow) == WHITEimg; --xLeftNow);//???????? 
			if (xLeftNow == xLeft)//如果左边没有白点了
			{
				for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow);//左边界往右找白点
			}
		}
		else for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow);  

		if (xRight < width - 1)  
		{
			for (xRightNow = xRight; xRightNow < width && IMAGE_DATA(xRightNow, yNow) == WHITEimg && IMAGE_DATA(xRightNow - 1, yNow) == WHITEimg; ++xRightNow);//????????
			if (xRightNow == xRight)//如果右边没有白点了
			{
				for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow);//右边界往左找白点
			}
		}
		else for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow);//右边界往左找白点 

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
计算中心线的斜率
方法：线性回归方程求斜率
param:起始行和终止行
return:斜率
*/
float CenterSlope(int startline, int endline)
{
	if(endline >= 100) endline = 99;
	int i;
	int sumX=0,sumY=0,averX=0,averY=0;
	int num=0,B_up1=0,B_up2=0,B_up,B_down;  //up是分子，down是分母，分子有两个
	float B;
	for(i=startline;i<=endline;i++)
  {
		num++;
    sumX += i;
    sumY += CenterPoint[i];
  }
	 averX=sumX/num;  //求出x方向和y方向的平均值
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










