#include "bsp_nrf24l01.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
static u8 rf_read_modeflag=0;
u8 rf_len;						//���ճ���
#ifdef RX_PLOAD_WIDTH
u8 rf_rxbuf[RX_PLOAD_WIDTH+1]="123456";	//���������飨32λ��
#else
u8 rf_rxbuf[100];	//���������飨100λ��
#endif
#ifdef TX_PLOAD_WIDTH
char rf_txbuf[TX_PLOAD_WIDTH+1]="";	//���������飨32λ��
#else
u8 rf_rxbuf[100];	//���������飨100λ��
#endif
u8 rf_flag;						//���ձ�־

const char TX_ADDRESS[TX_ADR_WIDTH]={0x19,0x95,0x03,0x26,0x13}; //���͵�ַ
const char RX_ADDRESS[RX_ADR_WIDTH]={0x19,0x95,0x03,0x26,0x13}; //��ȡ��ַ

//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{ 			 
	NRF24L01_CE(0); 			//ʹ��24L01����
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_AW,0x03);  //���õ�ַ���(��������ͨ��)02,4�ֽ�;03���ֽڿ�ȡ�
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��	,����ͨ��ͨ����
#ifdef RX_PLOAD_WIDTH
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 
#else
	{	NRF24L01_Write_Reg(NRF_WRITE_REG+DYNPD,0X3F);}	     //ʹ�ܶ�̬��Ч���ݿ������ͨ��0-5
#endif
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);		//���״̬�Ĵ���
	NRF24L01_RX_Mode();	
	NRF24L01_CE (1); //�ر�24L01����ʹ�ܣ� 		 	 
}

//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	NRF24L01_CE(0); 			//ʹ��24L01����
	NRF24L01_CSN(1);			//SPIƬѡȡ�� 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;//�����ȡ�ĵ�ַ����д��ĵ�ַ	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI1_ReadWriteByte(uint8_t TxData)//����һ���ֽڣ����ӼĴ�������һ���ֽ�
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&hspi2, &TxData, &Rxdata, 1, 1000);       
 	return Rxdata;          		    
}

//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN(0);                 //ʹ��SPI����
  	status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN(1);                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
		u8 reg_val;	    
		NRF24L01_CSN(0);         //ʹ��SPI����		
  	SPI1_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN(1);         //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	

//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN(0);      //ʹ��SPI����
  	status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
  	NRF24L01_CSN(1);       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
		u8 status,u8_ctr;	    
		NRF24L01_CSN (0);          //ʹ��SPI����
  	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	NRF24L01_CSN (1);       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				

#ifdef RX_PLOAD_WIDTH
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	if(rf_read_modeflag == 1 )NRF24L01_TX_Mode();	//���뷢��ģʽ	
	NRF24L01_CE(0);	
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE(1);//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		NRF24L01_RX_Mode();//��Ϊ����ģʽ.
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
#else
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf,u8 TX_PLOAD_WIDTH)
{
	u8 sta;
	if(rf_read_modeflag == 1 )NRF24L01_TX_Mode();	//���뷢��ģʽ	
	NRF24L01_CE(0);	
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE(1);//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		NRF24L01_RX_Mode();//��Ϊ����ģʽ.
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

#endif
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	if(rf_read_modeflag == 0 )NRF24L01_RX_Mode();
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{	
		#ifdef RX_PLOAD_WIDTH
				NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����	
//				rxbuf[32]='\0';
//				printf("%s \r\n",rxbuf);
//		    memset(rxbuf,0,33);
		#else
				rf_len = NRF24L01_Read_Reg(R_RX_PL_WID);			//���Ĵ����õ����յ������ݳ���
				NRF24L01_Read_Buf(RD_RX_PLOAD, rf_rxbuf, rf_len);	//�ѽ��յ������ݴ������rf_rxbuf��
		#endif
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}		

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
		NRF24L01_CE(0);	  
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//��ս���FIFO������   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF24L01_CE (1); //CEΪ��,�������ģʽ 
		rf_read_modeflag=1;
}					

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE(0);	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//��ս���FIFO������    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE(1);//CEΪ��,10us����������
	rf_read_modeflag=0;
}



	 	 

