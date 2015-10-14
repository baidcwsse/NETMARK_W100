#include "hal_rtc.h"
#include "nordic_common.h"
#include "rtc.h"
#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"
#include "uart.h" 
#include "config.h"
#include "command.h"
#include "led.h"

#define TIM1CNT  33332

INT16U cunt=0; //rtc计数
INT16U adc_level=0;//电平值

INT16U adc_charge=0;//电量值
INT8U charge_state=0; //充电状态 0:非充电  1:充电
INT8U config_state=0; //配置状态 0：非配置状态  1：配置状态

INT8U msg0f_state[17] = {0x0F,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //开机指令
INT8U msg10_state[17] = {0x10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //关机指令
INT8U msg1A_state[17] = {0x1A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //进入/退出充电  msg1A_state[1] :  0:非充电  1:充电
	
INT8U flag;
	
#define ADCGATE  2480 //3600
INT8U connect_state_num=0; //判断连接上底座的计数
INT8U charge_state_num=0; //判断充电状态的计数
INT8U lowpower=0;
INT8U lowpowercnt=0;
	
char chargeval; //充电临时变量
	
extern char fullflag;

void timer1_init(void)
{
	TMOD=0X11;
	TH1=(65536-TIM1CNT)/256;
	TL1=(65536-TIM1CNT)%256;
	ET1=1;
	TR1=1;  //定时器开启
}

void sev_timer1() interrupt INTERRUPT_T1
{
  static unsigned char time_number;
	
	ET1 = 0;
	TH1=(65536-TIM1CNT)/256;
	TL1=(65536-TIM1CNT)%256;
	
	time_number++;
	if(time_number==40)
	{
		time_number=0;
		//P10=~P10;
		
		watchdog_load(); //设置看门狗
	
		Is_Charge(); //判断是否在充电和配置

		//Adc_AIN10();
		//adc_charge = readadc();
	  //putch((INT8U)(adc_charge >> 8)); putch((INT8U)(adc_charge));

		if(config_state == 1)   //配置
		{
		}
		else if(charge_state == 1)  //充电
		{
			
			//TR0 = 0;
		}
		else  //非充电非配置
		{
			//powrdownmodeset(); //进入睡眠

			if(lowpower == 1 && taskstart == 1) //电量低的情况下,红灯慢闪
			{
				if(lowpowercnt == 0)
				{
					LED_UP();
					lowpowercnt = 1;
				}
				else
				{
					LED_DOWN();
					lowpowercnt = 0;
				}
				
			}
			else
			{
				LED_DOWN();
			}
			
		}
		
		if(taskstart == 1)	
			TR0 = 1;
	}

	ET1 = 1;
}

void Is_Charge(void)  //判断是否在充电
{
	Adc_GetPower(); //电平检测	测量是否拔出
	adc_level = readadc(); //检测电平
	delay_ms3(50);
	adc_level = adc_level + readadc();
	delay_ms3(50);
	adc_level = adc_level + readadc();
	adc_level = adc_level/3;
	
	if(adc_level <= CHARGEGATE)  //未插在底座上 充电时为1.83v
	{
		if(charge_state == 1)
		{
			charge_state = 0;
			
			wakepin();
			msg1A_state[1] = 0;
			putstrForTick(msg1A_state);  //不在充电
			
			if(config_state == 1)   //配置
			{
				config_state = 0;
			}
			
			if(taskstart == 0) //若在关机时拿下  (W100不存在此情况)
			{
// 				wakepin();
// 				putstrForTick(msg10_state);
			}
			else if(taskstart == 1)  //若在开机时拿下
			{
// 				wakepin();
// 				putstrForTick(msg0f_state);
			}
		}
	}
	else  //>2.5v
	{
		if(charge_state == 0)
		{
			charge_state = 1;

			wakepin();
			msg1A_state[1] = 1;
			putstrForTick(msg1A_state);   //正在充电
			
			LED_UP();
			
			lowpower = 0;
			
			//TR0 = 0; //关闭定时器
		}
	}
}

