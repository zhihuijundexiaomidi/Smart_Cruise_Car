//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA5（SCL）
//              D1   接PA7（SDA）
//              RES  接PB0
//              DC   接PB1
//              CS   接PA4               
//              ----------------------------------------------------------------
//******************************************************************************/

#include "bsp_oled096_spi.h"
#include "bsp_oled096_spi_font.h"  	 


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

#if OLED_MODE==1
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
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
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 //16是12的双倍y位移。
void OLED_ShowChar(u8 x,u8 y,u8 chr, u8 size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		  if(size ==16)
			{
					y=2*y;
					OLED_Set_Pos(x,y);	
					for(i=0;i<8;i++)
					{
						OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
					}
					OLED_Set_Pos(x,y+1);
					for(i=0;i<8;i++)
					{
						OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
					}
			}
			else 
     {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
} 

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{
			if(size==16)
			{
				OLED_ShowChar(x,y,chr[j],size);
					x+=8;
				if(x>120){x=0;y+=2;}
				j++;
			}
			if(size==12)
			{
				OLED_ShowChar(x,y,chr[j],size);
					x+=8;
				if(x>120){x=0;y+=2;}
				j++;
			}
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
//Y0:控制y轴，一个数值移动8个像素。从上往下y1：控制y轴显示多少，最大8，全部显示，4，显示一半。从上自下
//x0：为0，加大，会右旋转变形图像，x1：为128.
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{ 	
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address设置页面寻址模式的下列起始地址(00h~0Fh)
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address为页面寻址模式设置更高的列起始地址(10h~1Fh)

	OLED_WR_Byte(0x20,OLED_CMD);//设置内存寻址模式（20h）-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(2,OLED_CMD);//在SSD1306中，有3种不同的内存寻址模式

	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address设置显示起始线(40h~7Fh)

	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register设置BANK0的对比度控制（81h）
	OLED_WR_Byte(0,OLED_CMD); //Set SEG Output Current Brightness设置SEG输出电流亮度0到255 

	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping设置段重新映射（A0h/A1h）     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xA4,OLED_CMD);//整个显示打开 Disable Entire Display On (0xa4/0xa5)整个显示打开（A4h/A5h）
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display设置正常/反向显示器（A6/A7）

	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)设置多重比（A8h）
	OLED_WR_Byte(63,OLED_CMD);//--16到63 duty此命令将默认的63多路模式切换为任何多路比率，范围从16到63。输出垫COM0~COM63将被切换到相应的COM信号。

//	OLED_WR_Byte(0xB0,OLED_CMD);//设置页面寻址模式的页面起始地址(B0h~B7h)Set Page Start Address for Page Addressing Mode (B0h~B7h)
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常

	OLED_WR_Byte(0xD3,OLED_CMD);//设置显示偏移（D3h）-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0,OLED_CMD);//-not offset（0-63）

	OLED_WR_Byte(0xD5,OLED_CMD);//设置显示时钟分比/振荡器频率（D5h）--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0xF1,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec

	OLED_WR_Byte(0xD9,OLED_CMD);//设置预充电期（D9h）--set pre-charge period
	OLED_WR_Byte(0x11,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock设置预充电为15个时钟，为1个时钟
	//预充电周期会影响亮度。
	OLED_WR_Byte(0xDA,OLED_CMD);//设置COM引脚硬件配置(DAh)--set com pins hardware configuration
	OLED_WR_Byte(0xD8,OLED_CMD);//此命令将COM信号的引脚配置设置为匹配OLED面板的硬件布局。下表显示了不同条件下的COM引脚配置(对于MUX比率=64)

	OLED_WR_Byte(0xDB,OLED_CMD);////Set VCOM Deselect Level--set vcomh设置VCOMH取消选择级别(DBh)
	OLED_WR_Byte(0,OLED_CMD);//

	OLED_Display_On();
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  





























