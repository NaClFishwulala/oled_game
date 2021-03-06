//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）            
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////?

#include "oled.h"
#include "stdlib.h"
#include "string.h"
#include "oledfont.h"  	 


//OLED的显存
unsigned char oled_cache[8][128] = {0};
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
			OLED_SDIN_Set();
		else 
			OLED_SDIN_Clr();
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
  Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
  Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
  IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
  IIC_Start();
  Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
  Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
  Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
  IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
		Write_IIC_Data(dat);
	else
		Write_IIC_Command(dat);
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>Max_Column-1)
	{
		x=0;
		y=y+2;
	}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);
	}
}
//m^n函数
unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字 16*16
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no)
{      			    
	unsigned char t;
	OLED_Set_Pos(x,y);	
	for(t=0;t<16;t++)
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);				
}

//自己写的

/*
//OLED_DrawPoint
//功能：画点
//参数: x(起始点横坐标0~128)
//			y(起始点纵坐标0~63)			
*/
void OLED_DrawPoint(unsigned char x_pos,unsigned char y_pos)
{
	//先根据纵坐标y计算在第几页的第几个像素点
	unsigned char page = y_pos / 8;
	unsigned char dot = y_pos % 8;
	unsigned char data = 0x01;
	for(int i=0;i<dot-1;i++)
		data <<= 1;
	OLED_Set_Pos(x_pos,page);
	OLED_WR_Byte(data,OLED_DATA);
}

/*
//函数：OLED_DrawCol
//功能：画n列的点阵
//参数: x(起始点横坐标)
//			y(起始点页数0~7)
//			col(二维数组的列数)
//			no(二维数组下标)
*/
void OLED_DrawCol(unsigned char x,unsigned char y,int col,unsigned char no)
{      			    
	unsigned char t;
	OLED_Set_Pos(x,y);	
	for(t=0;t<col;t++)
		OLED_WR_Byte(BMP[no][t],OLED_DATA);			
}

/*
//OLED_DrawScreen
//功能：以每次画16列点阵的方式画满画屏幕
//参数: 无
*/
void OLED_DrawScreen(void)
{
//	对于16*8的点阵,i为行（上限为8）,j为列（上限为8）
	int count = 0;
	for(int i = 0;i<8;i++) 
	{
		for(int j=0;j<8;j++)
			OLED_DrawCol(j*16,i,16,count++);
	}
}

void OLED_CacheClear(void)
{
	for(int i=0;i<8;i++)
		memset(oled_cache,0,128*sizeof(unsigned char));
}

/**
* @brief  将位坐标映射成oled_cache值
* @param  x_pos 位图的横坐标
* @param  y_pos 位图的纵坐标
* @param  flag 可取WRITE(写点阵)&CLEAR(清点阵)
* @param  size 生成点阵的大小：1~7
* @retval 根据位坐标生成size*size的点阵，将其映射写入(或清除)oled_cache
*/
void PointToCache(unsigned char x_pos,unsigned char y_pos,unsigned char flag,unsigned char size)
{
	//先根据纵坐标y计算在第几页的第几个像素点
	for(int i=0;i<size;i++)
	{
		unsigned char page = (y_pos+i) / 8;
		unsigned char dot = (y_pos+i) % 8;
		unsigned char data = 0x01;
		for(int m=0;m<dot;m++)
			data <<= 1;

		for(int j=0;j<size;j++)
		{
			if(x_pos+j <Column && flag == WRITE  )
				oled_cache[page][x_pos+j] |= data;
			else if(x_pos+j <Column && flag == CLEAR )
				oled_cache[page][x_pos+j] &= (~data);
		}
	}
}

void OLED_DrawCache(void)
{
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<Column;j++)
		{
			OLED_Set_Pos(j,i);
			OLED_WR_Byte(oled_cache[i][j],OLED_DATA);
		}
	}
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;
  
  if(y1%8==0) 
		y=y1/8;      
  else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)      
			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	
	HAL_GPIO_WritePin(GPIOB, SCL_Pin|SDA_Pin, GPIO_PIN_SET);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  
