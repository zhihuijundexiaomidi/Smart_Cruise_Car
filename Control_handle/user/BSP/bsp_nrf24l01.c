#include "bsp_nrf24l01.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
static u8 rf_read_modeflag=0;
u8 rf_len;						//接收长度
#ifdef RX_PLOAD_WIDTH
u8 rf_rxbuf[RX_PLOAD_WIDTH+1]="123456";	//接收数据组（32位）
#else
u8 rf_rxbuf[100];	//接收数据组（100位）
#endif
#ifdef TX_PLOAD_WIDTH
char rf_txbuf[TX_PLOAD_WIDTH+1]="";	//发送数据组（32位）
#else
u8 rf_rxbuf[100];	//接收数据组（100位）
#endif
u8 rf_flag;						//接收标志

const char TX_ADDRESS[TX_ADR_WIDTH]={0x19,0x95,0x03,0x26,0x13}; //发送地址
const char RX_ADDRESS[RX_ADR_WIDTH]={0x19,0x95,0x03,0x26,0x13}; //读取地址

//初始化24L01的IO口
void NRF24L01_Init(void)
{ 			 
	NRF24L01_CE(0); 			//使能24L01配置
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_AW,0x03);  //设置地址宽度(所有数据通道)02,4字节;03五字节宽度。
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率	,即是通信通道。
#ifdef RX_PLOAD_WIDTH
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 
#else
	{	NRF24L01_Write_Reg(NRF_WRITE_REG+DYNPD,0X3F);}	     //使能动态有效数据宽度数据通道0-5
#endif
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);		//清空状态寄存器
	NRF24L01_RX_Mode();	
	NRF24L01_CE (1); //关闭24L01配置使能， 		 	 
}

//检测24L01是否存在
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	NRF24L01_CE(0); 			//使能24L01配置
	NRF24L01_CSN(1);			//SPI片选取消 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;//如果读取的地址不是写入的地址	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(uint8_t TxData)//发送一个字节，并从寄存器返回一个字节
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&hspi2, &TxData, &Rxdata, 1, 1000);       
 	return Rxdata;          		    
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN(0);                 //使能SPI传输
  	status =SPI1_ReadWriteByte(reg);//发送寄存器号 
  	SPI1_ReadWriteByte(value);      //写入寄存器的值
  	NRF24L01_CSN(1);                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
		u8 reg_val;	    
		NRF24L01_CSN(0);         //使能SPI传输		
  	SPI1_ReadWriteByte(reg);   //发送寄存器号
  	reg_val=SPI1_ReadWriteByte(0XFF);//读取寄存器内容
  	NRF24L01_CSN(1);         //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}	

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN(0);      //使能SPI传输
  	status=SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//读出数据
  	NRF24L01_CSN(1);       //关闭SPI传输
  	return status;        //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
		u8 status,u8_ctr;	    
		NRF24L01_CSN (0);          //使能SPI传输
  	status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //写入数据	 
  	NRF24L01_CSN (1);       //关闭SPI传输
  	return status;          //返回读到的状态值
}				

#ifdef RX_PLOAD_WIDTH
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	if(rf_read_modeflag == 1 )NRF24L01_TX_Mode();	//进入发送模式	
	NRF24L01_CE(0);	
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01_CE(1);//启动发送	   
	while(NRF24L01_IRQ!=0);//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		NRF24L01_RX_Mode();//变为接收模式.
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
#else
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf,u8 TX_PLOAD_WIDTH)
{
	u8 sta;
	if(rf_read_modeflag == 1 )NRF24L01_TX_Mode();	//进入发送模式	
	NRF24L01_CE(0);	
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01_CE(1);//启动发送	   
	while(NRF24L01_IRQ!=0);//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		NRF24L01_RX_Mode();//变为接收模式.
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

#endif
//启动NRF24L01接收一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	if(rf_read_modeflag == 0 )NRF24L01_RX_Mode();
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{	
		#ifdef RX_PLOAD_WIDTH
				NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据	
//				rxbuf[32]='\0';
//				printf("%s \r\n",rxbuf);
//		    memset(rxbuf,0,33);
		#else
				rf_len = NRF24L01_Read_Reg(R_RX_PL_WID);			//读寄存器得到接收到的数据长度
				NRF24L01_Read_Buf(RD_RX_PLOAD, rf_rxbuf, rf_len);	//把接收到的数据存的数组rf_rxbuf中
		#endif
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}		

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(void)
{
		NRF24L01_CE(0);	  
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清空接收FIFO的数据   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	NRF24L01_CE (1); //CE为高,进入接收模式 
		rf_read_modeflag=1;
}					

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE(0);	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清空接收FIFO的数据    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01_CE(1);//CE为高,10us后启动发送
	rf_read_modeflag=0;
}



	 	 

