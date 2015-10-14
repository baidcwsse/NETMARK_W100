#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"
#include "uart.h"
#include "config.h"
#include "command.h"
#include "adc.h"
#include "rtc.h"
#include "hal_flash.h"

void usart2rf(void);
 
typedef enum
{ 
	off=0,
	on
}ais_status;

static ais_status str_cmp(INT8U* str_src , INT8U* str_dst , INT8U num)  //比较两个数组是否相等  :判断船名钥匙
{
	ais_status is_equal=on;
	while(num--)
	{
		if((*str_src) != (*str_dst))
			is_equal=off;
		str_src++;
		str_dst++;
	}
	return is_equal;
}

INT8U txbuf[30] = {0x00};
INT8U rxbuf[30] = {0x00};
static ais_status str_cmp_flag;//数组是否相等结果，on标志相同；off标志不同。
static INT8U *ptr_cmp1;  //用于比较网位仪船名与钥匙船名的指针
static INT8U *ptr_cmp2;  //用于比较船名的指针
static INT8U cnt_cmp; //用于比较船名的计数值

extern INT16U adc_level;
extern INT16U adc_charge;
extern INT8U charge_state;
extern INT8U config_state;
static INT8U i,j;
extern volatile INT16U timinteval;
extern INT8U timinteval_1;
extern INT8U timinteval_2;
extern INT8U lowpower;
INT8U msg03_state[17] = {0x03,0x01,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //注入成功
	
void wakepin(void) //唤醒管脚函数
{
	P1DIR &= ~0x01;
	P10=0;
	delay(0xff);delay(0xff);
	P10=1;
	delay(0xff);delay(0xff);
	P10=0;
	delay(0xff);delay(0xff);
	P10=1;
	delay(0xff);delay(0xff);
	P10=0;
	delay(0xff);delay(0xff);
	P10=1;
	delay(0xff);delay(0xff);delay(0xff);delay(0xff);
}
	
void Command(void)  //RF接收后由串口发送
{
	switch(rxbuf[0])
	{
		/*case 0x03://注入钥匙
		{
			if(config_state == 1) //防止未充电的网位仪，收到该命令
			{
				putstrForRF(msg03_state);
			}
		}
		break;
		case 0x06://读取钥匙
		{
			if(config_state == 1) //防止未充电的网位仪，收到该命令
			{
				putstrForRF(rxbuf);
			}
		}
		break;
		case 0x0f://钥匙开启
		if(config_state == 0 && charge_state == 0)
 		{
			ptr_cmp1=&rxbuf[2];  //钥匙船名第一位指针
			ptr_cmp2=&boatnum[1];     //网位仪船名第一位指针
			//if(boatnum[boatnum[0]/6-1]=='-')  //判断网号个数 1位
			//{
				if(rxbuf[1]==boatnum[0]-6)
				{
					cnt_cmp=(boatnum[0]-6)/8;
					str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
					
					if(str_cmp_flag==on)     //船名判断一致
					{
						wakepin();
						putstrForRF(rxbuf);
					
						taskstart = 1;
						TR0 = 1;
					}
				}
				else if(rxbuf[1]==boatnum[0]-12)
				{
					cnt_cmp=(boatnum[0]-12)/8;
					str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
					
					if(str_cmp_flag==on)     //船名判断一致
					{
						wakepin();
						putstrForRF(rxbuf);
					
						taskstart = 1;
						TR0 = 1;
					}
				}
			//}
		}
		break;
		case 0x10://钥匙关闭
		if(config_state == 0 && charge_state == 0)
 		{
			ptr_cmp1=&rxbuf[2];  //钥匙船名第一位指针
			ptr_cmp2=&boatnum[1];     //网位仪船名第一位指针
			if(rxbuf[1]==boatnum[0]-6)
			{
				cnt_cmp=(boatnum[0]-6)/8;
				str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
				if(str_cmp_flag==on)     //船名判断一致
				{
					wakepin();
					putstrForRF(rxbuf);
					
					TR0 = 0;
					taskstart = 0;
				}
			}
			else if(rxbuf[1]==boatnum[0]-12)
			{
				cnt_cmp=(boatnum[0]-12)/8;
				str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
				if(str_cmp_flag==on)     //船名判断一致
				{
					wakepin();
					putstrForRF(rxbuf);
					
					TR0 = 0;
					taskstart = 0;
				}
			}
		}
		break;*/
		default:
		break;
	}	
}

void Command2(void)  //串口接收
{
	switch(com_buf[1])
	{
		case 0x01://向网位仪写船名
		{
			if(config_state == 1)
			{
				for(i=0;i<16;i++)
					boatnum[i] = com_buf[i+2];
				
				WriteflashBoatnum();

				com_buf[2] = 1;
				putstrForUart(com_buf);
			}
		}
		break;
		/*case 0x03://注入钥匙
		{
			usart2rf();
		}
		break;*/
		case 0x04://向网位仪读船名
		{
			if(config_state == 1)
			{
				ReadflashBoatnum();
				
				for(i=0;i<16;i++)
					com_buf[i+2] = boatnum[i];
				
				putstrForUart(com_buf);
			}
		}
		break;
		/*case 0x06://读取钥匙
		{
			usart2rf();
		}
		break;*/
		case 0x0C://恢复出厂设置，清除密码
		{
			//hal_flash_page_erase(34);
			//PCON &= ~0x10;

			taskstart = 1;
			TR0 = 1;

			com_buf[2] = 1;
			putstrForUart(com_buf);
		}
		break;
		case 0x12://发射间隔设置
		{
			if(config_state == 1)
			{
				WriteflashInterval();
				
				com_buf[2] = 1;
				putstrForUart(com_buf);
			}
		}
		break;
		case 0x17://网位仪连接
		{
			wakepin();
			com_buf[2] = 1;
			putstrForUart(com_buf);
			
			config_state = 1;
			charge_state = 1;
		}
		break;
		case 0x18://网位仪断开
		{
			config_state = 0;
		}
		break;
		case 0x19://电量低
		{
			if(charge_state == 0)
			{
				if(com_buf[2] == 1) //电量高
				{
					lowpower = 0;
				}
				else //电量低
				{
					lowpower = 1;
				}
			}
		}
		break;
		case 0x1A://充电状态ack
		{
			/*if(com_buf[2] == 1) //充电状态
			{
				config_state = 1;
			}
			else
			{
				config_state = 0;
			}*/
		}
		break;
		default:
		break;	
	}	
}

void usart2rf(void)
{
	for(j=0;j<17;j++)
		txbuf[j] = com_buf[j+1];

	L01_Init( );	              	//Initialize the internal 24L01P    
	L01_SetTXMode( );               //Set as TX mode
	L01_FlushTX( );
	L01_FlushRX( );
	rf_flag = 0;
	delay(1000);
	delay(1000);
	delay(1000);

	for(itmp=2;itmp>1;itmp--)
	{
			L01_WriteTXPayload_Ack( txbuf, 17 );
			delay(1000);
			delay(1000);
			delay(1000);
			delay(1000);
			delay(1000);
			delay(1000);
	}
	
	L01_Init( );	
	L01_SetRXMode( );               //Set as RX mode
	delay(1000);
	delay(1000);
	delay(1000);
	L01_FlushRX( );	
	rf_flag = 0;	
}
