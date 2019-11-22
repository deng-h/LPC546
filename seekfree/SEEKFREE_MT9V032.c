/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		总钻风
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看common.h内VERSION宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SDA(51的RX)         A9
						SCL(51的TX)         A8
						场中断(VSY)         A0
						行中断(HREF)		未使用，因此不接
						像素中断(PCLK)      A1        
						数据口(D0-D7)		B24-B31
					------------------------------------ 
	
					默认分辨率是            188*120
					默认FPS                 50帧
 ********************************************************************************************************************/


#include "common.h"
#include "LPC546XX_sct.h"
#include "LPC546XX_dma.h"
#include "LPC546XX_gpio.h"
#include "LPC546XX_uart.h"
#include "LPC546XX_systick.h"
#include "LPC546XX_pint.h"
#include "SEEKFREE_MT9V032.h"




uint8  CameraBinary[MT9V032_H][MT9V032_W];//二值化图像，二维数组
uint8  image[MT9V032_H][MT9V032_W];      //图像数组
uint8  receive[3];
uint8  receive_num = 0;
uint8  uart_receive_flag = 1;
int16  Threshold_Value=60;

//需要配置到摄像头的数据
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                              //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          600}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               50},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           MT9V032_W}, //图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           MT9V032_H}, //图像行数量        范围1-480
    {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              32},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度

    
    {INIT,              0}    //摄像头开始初始化
};

//从摄像头内部获取到的配置数据
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      
    {EXP_TIME,          0},   //曝光时间          
    {FPS,               0},   //图像帧率          
    {SET_COL,           0},   //图像列数量        
    {SET_ROW,           0},   //图像行数量        
    {LR_OFFSET,         0},   //图像左右偏移量    
    {UD_OFFSET,         0},   //图像上下偏移量    
    {GAIN,              0},   //图像增益          
};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头串口中断函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       该函数在ISR文件 串口3中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_cof_uart_interrupt(void)
{
    uart_query(MT9V032_COF_UART,&receive[receive_num]);
    receive_num++;
    
    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
    if(3 == receive_num)
    {
        receive_num = 0;
        uart_receive_flag = 1;
    }
        
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头内部配置信息
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart3
//-------------------------------------------------------------------------------------------------------------------
void get_config(void)
{
	uint16 temp, i;
    uint8  send_buffer[4];
    
    for(i=0; i<CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = GET_CFG[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        
        //等待接受回传数据
        while(!uart_receive_flag);
        uart_receive_flag = 0;
        
        GET_CFG[i][1] = receive[1]<<8 | receive[2];
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头固件版本
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart3
//-------------------------------------------------------------------------------------------------------------------
uint16 get_version(void)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        
    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    return ((uint16)(receive[1]<<8) | receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      单独设置摄像头曝光时间
//  @param      light   设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间如果设置的数据过大，那么摄像头将会设置这个最大值
//  @return     uint16  当前曝光值，用于确认是否正确写入
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint16 set_exposure_time(uint16 light)
{
	uint16 temp;
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      对摄像头内部寄存器进行写操作
//  @param      addr    摄像头内部寄存器地址
//  @param      data    需要写入的数据
//  @return     uint16  寄存器当前数据，用于确认是否写入成功
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v032_reg(uint8 addr, uint16 data)
{
	uint16 temp;
    uint8  send_buffer[4];
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    systick_delay_ms(10);
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头初始化
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
void camera_init(void)
{
    //设置参数    具体请参看使用手册
    uint16 temp, i;
    uint8  send_buffer[4];

    
    uart_init (MT9V032_COF_UART, 9600,MT9V032_COF_UART_TX,MT9V032_COF_UART_RX);	//初始换串口 配置摄像头    
    uart_rx_irq(MT9V032_COF_UART,2);
    EnableInterrupts;
    
    //等待摄像头上电初始化成功
    systick_delay_ms(1000);
    uart_receive_flag = 0;
    
    //开始配置摄像头并重新初始化
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = MT9V032_CFG[i][0];
        temp = MT9V032_CFG[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        systick_delay_ms(2);
    }
    //等待摄像头初始化成功
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    while((0xff != receive[1]) || (0xff != receive[2]));
    //以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
    //利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
    
    //获取配置便于查看配置是否正确
    get_config();
    
    
    //摄像头采集初始化
    DisableInterrupts;
    //初始化SCT模块用于接收PCLK信号并触发DMA
    sct_camera_dma(MT9V032_PCLK_SCT, MT9V032_PCLK, RISING);
    dam_init_linked(MT9V032_DMA_CH, (void *)&MT9V032_DATAPORT, (void *)image[0], MT9V032_W*MT9V032_H);
    pint_init(MT9V032_VSYNC_PINT,MT9V032_VSYNC_PIN,FALLING);
    
    
    set_irq_priority(MT9V032_VSYNC_IRQN,1); //设置场中断为最高优先级
    set_irq_priority(DMA0_IRQn,2);          //设置DMA中断为第二优先级
    
    enable_irq(MT9V032_VSYNC_IRQN);
    enable_irq(DMA0_IRQn);
    sct_start();

	EnableInterrupts;
}

uint8   mt9v032_finish_flag = 0;    //一场图像采集完成标志位

uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
   #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    
    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }
                      
    //计算每个像素值的点在整幅图像中的比例  
  
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        
    }

    //遍历灰度级[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    
     
        
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)         
        {
            
                w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
                u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值 
           
               w1=1-w0;
               u1tmp=gray_sum/pixelSum-u0tmp;
        
                u0 = u0tmp / w0;              //背景平均灰度
                u1 = u1tmp / w1;              //前景平均灰度
                u = u0tmp + u1tmp;            //全局平均灰度
                deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
                if (deltaTmp > deltaMax)
                {
                    deltaMax = deltaTmp;
                    threshold = j;
                }
                if (deltaTmp < deltaMax)
                {
                break;
                }
          
         }

    return threshold;

}

void GrayscaleToBinary(uint8 *Grayscale, uint8 *Binary, uint8 Threshold, uint32 size)//二值化
{
	uint32 index;
	for(index = 0; index < size; ++index)
	Binary[index] = (Grayscale[index] > Threshold) ?1:0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头场中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_vsync(void)
{
    PINT_IST_FLAG_CLEAR(MT9V032_VSYNC_PINT);    //清除标志位
    if(SCT_DMA0REQUEST_DRQ0_MASK & SCT0->DMA0REQUEST)   DMA_ABORT(MT9V032_DMA_CH);
    dma_reload_linked(MT9V032_DMA_CH, (void *)image[0], MT9V032_W*MT9V032_H);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头DMA完成中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_dma(void)
{
	CLEAR_DMA_FLAG(MT9V032_DMA_CH);
    //由于LPC的DMA链接传输模式会导致没耗尽一个通道描述符就会进入一次中断因此，采集一副图像实际进入中断的次数为120*188/1024=23（图像分辨率为120*188  结果进一法取整）
    if(!DMA_STATUS(MT9V032_DMA_CH))//检查当前的DMA链接传输是否完毕
    {
        mt9v032_finish_flag = 1;//一副图像从采集开始到采集结束耗时3.8MS左右(50FPS、188*120分辨率)
    }

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      uartn			使用的串口号
//  @return     void			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(UARTN_enum uartn)
{
	uart_putchar(uartn,0x00);uart_putchar(uartn,0xff);uart_putchar(uartn,0x01);uart_putchar(uartn,0x01);//发送命令
    uart_putbuff(uartn, (uint8_t *)image, MT9V032_H*MT9V032_W);  //发送图像
}

/*!
*  @描述       发送灰度图数据到上位机
*  @参数       uartn   		发送的UART号
*  @参数			 grayscale	灰度图数据地址
*  @参数       width			灰度图的宽
*  @参数       height			灰度图的高
*  注意:       这是一个通信协议,协议的规定:每次发送数据开始都要发送两个起始位(0x4C),再发送图片的宽和高,
							 然后发送图像数据,但是发送数据中每当遇到和起始位(0x4C)和结束位(0x55)和数据位(0x49)时,在
							 前面加上一个数据位,即用0x49,0x4C和0x49,0x55和0x49,0x49替换他们
							 例如： 发送一个2*3数据为0x4C,0x00,0x49,0x55,0x55,0x10的图像
							 发送格式为：0x4C,0x4C,  0x02,0x03,  0x49,0x4C,  0x00,  0x49,0x49,  0x49,0x55,  0x49,0x55,  0x10,  0x55,0x55
*  示例:       GrayscaleSend (uart4, image, 188, 120);    //用uart4发送一个188*120的灰度图image
*/
void GrayscaleSend(UARTN_enum uartn, unsigned char *grayscale, uint16 width, uint16 height)
{
	#define CMD_START		0x4C
	#define CMD_DATA		0x49
	#define CMD_END			0x55
	
	uint8 cmds[] = {CMD_START, CMD_START};
	uint8 cmde[] = {CMD_END, CMD_END};
	
	uart_putbuff(uartn, cmds, sizeof(cmds));
	
	if(width == CMD_START || width == CMD_DATA || width == CMD_END)
	{
		uart_putchar(uartn, CMD_DATA);
	}
	uart_putchar(uartn, width);
	if(height == CMD_START || height == CMD_DATA || height == CMD_END)
	{
		uart_putchar(uartn, CMD_DATA);
	}
	uart_putchar(uartn, height);
	
	for(int temp = 0; temp < width * height; ++temp)
	{
		if(grayscale[temp] == CMD_START || grayscale[temp] == CMD_DATA || grayscale[temp] == CMD_END)
		{
			uart_putchar(uartn, CMD_DATA);
		}
		
		uart_putchar(uartn, grayscale[temp]);
	}
	
	uart_putbuff(uartn, cmde, sizeof(cmde));
}

