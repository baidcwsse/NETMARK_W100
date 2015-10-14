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
INT8U codeflag; //是否加密的变量
volatile INT8U taskstart=1; //开关机状态变量，开机为1，关机为0

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
	CLKLFCTRL =  0x01;  //32k晶振 for RTC
	gpio_init();  //led	
	//ALL_LED_DOWN();
	
	uart_init();
	//rtc2_init();
	//RF_config();
	timer_init();
	timer1_init();
	Adc_GetPower(); //电平检测
	radmcof(); //随机数配置
	FlashInit(); //flash读入

	P10=1;
	delay(0xff);
 	//retentionpinset();		//设置 RFrx&RTC 睡眠
 	EA = 1; //总中断开
	
	watchdog_load(); //设置看门狗
	//powrdownmodeset(); //进入睡眠

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









