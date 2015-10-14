#include "timer.h"
#include "reg24le1.h"
#include "nRF24L01.h"
#include "wdt.h"
#include "config.h"
#include "main.h"
#include "uart.h" 
#include "hal_flash.h"
#include "command.h"
#include "rtc.h"

#define ONE_STEP  33332
volatile INT16U timinteval=7200;
INT8U timinteval_1;
INT8U timinteval_2;
volatile INT16U interval_num=0;
INT8U randomnum; //生成的随机数偏移值
extern volatile char cmd0x0f;
extern INT8U charge_state;
extern volatile char cmd0x29;

void timer_init(void)
{
	TMOD=0X01;
	TH0=(65536-ONE_STEP)/256;
	TL0=(65536-ONE_STEP)%256;
	ET0=1;
	//TR0=1;  //定时器开启
}

void sev_timer0() interrupt INTERRUPT_T0
{
	static unsigned int count = 0;
	
	ET0 = 0;
	
	TH0=(65536-ONE_STEP)/256;
	TL0=(65536-ONE_STEP)%256;
	
	watchdog_load();
	
	count++; //putch((INT8U)(timinteval>>8)); putch((INT8U)(timinteval));
	//if(count==400)	 //:30s           //1=25ms //40=1s  //400=10s  // 2400=1min  //7200=3min
	if(count == timinteval -128 + randomnum)
	{
		count=0;

		randomnum = readrdm(); //时隙随机化

		if(charge_state == 0)
		{
			P1DIR &= ~0x01;         //set p1.0 output
			P10=0;
			delay(0xff);
			P10=1; //上升沿唤醒
			delay(0xff);
		}
	}

	ET0 = 1;
}







