#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"



void powrdownmodeset(void)
{
	P0DIR |= 0XA7;       //�ر�GPIO
	P1DIR |= 0X7B;       
	
	CLKCTRL = 0X01;
	PWRDWN |= 0x07;
}

void retentionpinset(void)
{	
	//WUCON &= 0X3f;  //RF sleep
	//WUCON &= 0xCF;  //RTC sleep
	
	WUCON &= 0x0F;  //RF & RTC sleep
}

void RF_config(void)
{
	RFCON = 0x07;
  RFCE = 0;
  RFCKEN = 1;      // ����RFʱ��
 	RF = 1;          // ����RF�ж�
	RFCE = 1;
}