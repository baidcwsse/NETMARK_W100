#include "reg24le1.h"
#include "nRF24L01.h"
#include "config.h"
#include "main.h"
#include "uart.h"
#include "hal_flash.h"
#include "command.h"
#include "rtc.h"
#include "timer.h"

INT8U interval_s;
INT8U boatnum[16];
INT8U codeflag; //�Ƿ���ܵı���
volatile INT8U taskstart=1; //���ػ�״̬����������Ϊ1���ػ�Ϊ0

void rtc2_init(void);
extern char chargeval;
extern INT8U lowpower;

extern INT8U msg0f_state[17];
	
void delay(unsigned int dj)
{
	unsigned char di;
	for(;dj>0;dj--)
   for(di=120;di>0;di--);
	
	//watchdog_load();
}

void main( void )
{
	EA = 0;  

	CLKCTRL = 0x28;  
	CLKLFCTRL =  0x01;  //32k���� for RTC
	gpio_init();  //led	
	//ALL_LED_DOWN();
	
	uart_init();
	//rtc2_init();
	//RF_config();
	timer_init();
	timer1_init();
	Adc_GetPower(); //��ƽ���
	radmcof(); //���������
	FlashInit(); //flash����

	P10=1;
	delay(0xff);
 	//retentionpinset();		//���� RFrx&RTC ˯��
 	EA = 1; //���жϿ�
	
	watchdog_load(); //���ÿ��Ź�
	//powrdownmodeset(); //����˯��

	TR0 = 1;
	taskstart = 1;
	
 	wakepin();
// 	putstrForRF(msg0f_state);
// 	putstrForRF(msg0f_state);
// 	putstrForRF(msg0f_state);
// 	putstrForRF(msg0f_state);
// 	putstrForRF(msg0f_state);
// 	putstrForRF(msg0f_state);
	
	while(1)
	{	
	}
}









