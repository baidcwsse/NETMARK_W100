#include "reg24le1.h"
#include "nRF24L01.h"
#include "config.h"
#include "main.h"
#include "uart.h"
#include "hal_flash.h"
#include "command.h"
#include "rtc.h"
#include "wdt.h"

extern INT8U charge_state;
extern INT8U config_state;

// char fullflag=0; //�ڰ�յ�ƶ�ʱ ��Ǳ���
// int fullflagnum=0; //����

void gpio_init(void) //LED
{
	P0CON = 0X00;
	P1CON = 0X00;
	//P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	//P1DIR &= ~0X7B;       //set p1.0 p1.1 p1.3 p1.4 p1.5 p1.6 output
	P1DIR &= ~0x01;         //set p1.0 output
}

void Get_Power(void) //������ʾ
{
	Adc_AIN10(); //�������
	adc_charge = readadc(); //������

}
////////////////////////////////////////
void delay_ms2(INT16U dj) 
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

void delay_ms3(INT16U dj) 
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

void LED_UP(void)          //LED����
{
	P0DIR &= ~0X80;       //set p0.7 output
	
  LED3_ON();
	
 	//P0DIR |= 0X80;       //�ر�GPIO
}

void LED_DOWN(void)          //LED��
{
	P0DIR &= ~0X80;       //set p0.7 output
	
  LED3_OFF();
	
 	P0DIR |= 0X80;       //�ر�GPIO
}

void LED_SLOW(void)  //LED����
{
	P0DIR &= ~0X80;       //set p0.7 output
	
  LED3_OFF();
	
 	P0DIR |= 0X80;       //�ر�GPIO
}
	
// void ALL_LED_UP(void)          //����LED����
// {
// 	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
// 	P1DIR &= ~0X7A; 	
// 	
//   LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();	 
// 	
// // 	P0DIR |= 0XA7;       //�ر�GPIO
// // 	P1DIR |= 0X7A;       
// }





