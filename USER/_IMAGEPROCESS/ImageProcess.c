#include "ImageProcess.h"


uint8 CenterPoint[100];  //存放中心线的x坐标
int qulv_near_right=0, qulv_near_left=0 ,qulv_global_right=0 ,qulv_global_left=0; //曲率近处，全局
int qulv_far_right=0 ,qulv_far_left=0; //曲率远处
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
扫线程序，对于0~99行，找到白色左边界和白色右边界，从而确定白色区域的中心位置
OLED的底面是height(188)，y为188时有可能在OLED上看不到有可能时OLED屏幕大小与图像尺寸不一样
函数中的height对应binary数组的x，width对应数组的y
*/
void ScanLine(uint8 * imageRet, int width, int height)  
{
	int xLeft = 0, xRight = 0 , index = 0 ;
	int temp;
	
	qulv_far_right = qulv_far_left = qulv_near_right = qulv_near_left = qulv_global_right = qulv_global_left = 0;  //数据清零
	DuanDian=0;
	
	memset(CenterPoint,0,sizeof(CenterPoint)); //清零
	memset(RdiuxianFlg,0,sizeof(RdiuxianFlg));
	memset(LdiuxianFlg,0,sizeof(LdiuxianFlg));
	
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
		
	for (int yNow = height - 2; yNow > height - 100; --yNow)  //从底面开始扫，100行
	{
		int xLeftNow = 0, xRightNow = 0;

		if (xLeft > 0)
		{
			for (xLeftNow = xLeft; xLeftNow > 0 && IMAGE_DATA(xLeftNow, yNow) == WHITEimg && IMAGE_DATA(xLeftNow + 1, yNow) == WHITEimg; --xLeftNow); //左边有向左弯的趋势，左转
			if (xLeftNow == xLeft) //边界在更右边，一般是直道或右转的趋势
			{
				for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow);
			}
		}
		else 
		{
			for (xLeftNow = xLeft; xLeft < width && IMAGE_DATA(xLeftNow, yNow) == BLACKimg; ++xLeftNow); 
			if(xLeftNow - xLeft <= 1) LdiuxianFlg[yNow] = 1;   //左丢线
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
			for (xRightNow = xRight; xRightNow >= 0 && IMAGE_DATA(xRightNow, yNow) == BLACKimg; --xRightNow); //右丢线
			if(xRight - xRightNow <= 1) RdiuxianFlg[yNow] = 1;  //右丢线
		}
		
//		if((1 == LdiuxianFlg[yNow]) && (0 == RdiuxianFlg[yNow]))  //补线
//		{
//			xLeftNow = xRightNow - RoadWidth[119 - yNow];
//		}
//		if((0 == LdiuxianFlg[yNow]) && (1 == RdiuxianFlg[yNow]))
//		{
//			xRightNow = xLeftNow + RoadWidth[119 - yNow];
//		}
		
//		if(((1 == LdiuxianFlg[yNow]) || (1 == RdiuxianFlg[yNow])) && yNow > 100) DuanDian++;   //最下面左右丢线，断点++
		
		
		
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
		
		/*  曲率计算  */
//		if(yNow > 70)  //近处的48行, 曲率范围-48~48
//		{
//			if(CenterPoint[index-1] - CenterPoint[index-2] > 0) qulv_near_right++;
//			if(CenterPoint[index-1] - CenterPoint[index-2] < 0) qulv_near_left++;
//		}
//		if(yNow > 25 && yNow < 70)  //远处43行,曲率范围-43~43
//		{
//			if(CenterPoint[index-1] - CenterPoint[index-2] > 0) qulv_far_right++;
//			if(CenterPoint[index-1] - CenterPoint[index-2] < 0) qulv_far_left++;
//		}
//		if(yNow > 30 && yNow < 110) //整体78行,曲率范围-78~78
//		{
//			if(CenterPoint[index-1] - CenterPoint[index-2] > 0) qulv_global_right++;
//			if(CenterPoint[index-1] - CenterPoint[index-2] < 0) qulv_global_left++;
//		}
		/*  曲率计算结束  */
		
	}//行循环结束
}


/*
计算中心线的斜率
方法：线性回归方程求斜率
参数:起始行和终止行
返回值:斜率
*/
float CenterSlope(int startline, int endline)
{
	float Slope=0.0;
	if(endline >= 100) endline = 99;
	int i;
	int sumX=0,sumY=0,averX=0,averY=0;
	int num=0,B_up1=0,B_up2=0,B_up,B_down;  //up是分子，down是分母，分子有两个
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
		B_up += (int)(10*B_up1*B_up2);
		B_up=B_up/100*100;
		B_down += (int)(10*((i-averX)*(i-averX)));
   }
   
   if(0 == B_down) Slope=0.0;
   else Slope = 16 * B_up/B_down;
   
   return Slope;
}










