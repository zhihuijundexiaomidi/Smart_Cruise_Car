//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PA5��SCL��
//              D1   ��PA7��SDA��
//              RES  ��PB0
//              DC   ��PB1
//              CS   ��PA4               
//              ----------------------------------------------------------------
//******************************************************************************/

#include "bsp_oled096_spi.h"
#include "bsp_oled096_spi_font.h"  	 


//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

#if OLED_MODE==1
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 //16��12��˫��yλ�ơ�
void OLED_ShowChar(u8 x,u8 y,u8 chr, u8 size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
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
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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

//��ʾһ���ַ��Ŵ�
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
//��ʾ����
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
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
//Y0:����y�ᣬһ����ֵ�ƶ�8�����ء���������y1������y����ʾ���٣����8��ȫ����ʾ��4����ʾһ�롣��������
//x0��Ϊ0���Ӵ󣬻�����ת����ͼ��x1��Ϊ128.
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


//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address����ҳ��Ѱַģʽ��������ʼ��ַ(00h~0Fh)
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column addressΪҳ��Ѱַģʽ���ø��ߵ�����ʼ��ַ(10h~1Fh)

	OLED_WR_Byte(0x20,OLED_CMD);//�����ڴ�Ѱַģʽ��20h��-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(2,OLED_CMD);//��SSD1306�У���3�ֲ�ͬ���ڴ�Ѱַģʽ

	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address������ʾ��ʼ��(40h~7Fh)

	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register����BANK0�ĶԱȶȿ��ƣ�81h��
	OLED_WR_Byte(0,OLED_CMD); //Set SEG Output Current Brightness����SEG�����������0��255 

	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping���ö�����ӳ�䣨A0h/A1h��     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xA4,OLED_CMD);//������ʾ�� Disable Entire Display On (0xa4/0xa5)������ʾ�򿪣�A4h/A5h��
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display��������/������ʾ����A6/A7��

	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)���ö��رȣ�A8h��
	OLED_WR_Byte(63,OLED_CMD);//--16��63 duty�����Ĭ�ϵ�63��·ģʽ�л�Ϊ�κζ�·���ʣ���Χ��16��63�������COM0~COM63�����л�����Ӧ��COM�źš�

//	OLED_WR_Byte(0xB0,OLED_CMD);//����ҳ��Ѱַģʽ��ҳ����ʼ��ַ(B0h~B7h)Set Page Start Address for Page Addressing Mode (B0h~B7h)
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����

	OLED_WR_Byte(0xD3,OLED_CMD);//������ʾƫ�ƣ�D3h��-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0,OLED_CMD);//-not offset��0-63��

	OLED_WR_Byte(0xD5,OLED_CMD);//������ʾʱ�ӷֱ�/����Ƶ�ʣ�D5h��--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0xF1,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec

	OLED_WR_Byte(0xD9,OLED_CMD);//����Ԥ����ڣ�D9h��--set pre-charge period
	OLED_WR_Byte(0x11,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock����Ԥ���Ϊ15��ʱ�ӣ�Ϊ1��ʱ��
	//Ԥ������ڻ�Ӱ�����ȡ�
	OLED_WR_Byte(0xDA,OLED_CMD);//����COM����Ӳ������(DAh)--set com pins hardware configuration
	OLED_WR_Byte(0xD8,OLED_CMD);//�����COM�źŵ�������������Ϊƥ��OLED����Ӳ�����֡��±���ʾ�˲�ͬ�����µ�COM��������(����MUX����=64)

	OLED_WR_Byte(0xDB,OLED_CMD);////Set VCOM Deselect Level--set vcomh����VCOMHȡ��ѡ�񼶱�(DBh)
	OLED_WR_Byte(0,OLED_CMD);//

	OLED_Display_On();
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  





























