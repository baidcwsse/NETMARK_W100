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

INT16U cunt=0; //rtc����
INT16U adc_level=0;//��ƽֵ

INT16U adc_charge=0;//����ֵ
INT8U charge_state=0; //���״̬ 0:�ǳ��  1:���
INT8U config_state=0; //����״̬ 0��������״̬  1������״̬

INT8U msg0f_state[17] = {0x0F,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //����ָ��
INT8U msg10_state[17] = {0x10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //�ػ�ָ��
INT8U msg1A_state[17] = {0x1A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //����/�˳����  msg1A_state[1] :  0:�ǳ��  1:���
	
INT8U flag;
	
#define ADCGATE  2480 //3600
INT8U connect_state_num=0; //�ж������ϵ����ļ���
INT8U charge_state_num=0; //�жϳ��״̬�ļ���
INT8U lowpower=0;
INT8U lowpowercnt=0;
	
char chargeval; //�����ʱ����
	
extern char fullflag;

void timer1_init(void)
{
	TMOD=0X11;
	TH1=(65536-TIM1CNT)/256;
	TL1=(65536-TIM1CNT)%256;
	ET1=1;
	TR1=1;  //��ʱ������
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
		
		watchdog_load(); //���ÿ��Ź�
	
		Is_Charge(); //�ж��Ƿ��ڳ�������

		//Adc_AIN10();
		//adc_charge = readadc();
	  //putch((INT8U)(adc_charge >> 8)); putch((INT8U)(adc_charge));

		if(config_state == 1)   //����
		{
		}
		else if(charge_state == 1)  //���
		{
			
			//TR0 = 0;
		}
		else  //�ǳ�������
		{
			//powrdownmodeset(); //����˯��

			if(lowpower == 1 && taskstart == 1) //�����͵������,�������
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

void Is_Charge(void)  //�ж��Ƿ��ڳ��
{
	Adc_GetPower(); //��ƽ���	�����Ƿ�γ�
	adc_level = readadc(); //����ƽ
	delay_ms3(50);
	adc_level = adc_level + readadc();
	delay_ms3(50);
	adc_level = adc_level + readadc();
	adc_level = adc_level/3;
	
	if(adc_level <= CHARGEGATE)  //δ���ڵ����� ���ʱΪ1.83v
	{
		if(charge_state == 1)
		{
			charge_state = 0;
			
			wakepin();
			msg1A_state[1] = 0;
			putstrForTick(msg1A_state);  //���ڳ��
			
			if(config_state == 1)   //����
			{
				config_state = 0;
			}
			
			if(taskstart == 0) //���ڹػ�ʱ����  (W100�����ڴ����)
			{
// 				wakepin();
// 				putstrForTick(msg10_state);
			}
			else if(taskstart == 1)  //���ڿ���ʱ����
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
			putstrForTick(msg1A_state);   //���ڳ��
			
			LED_UP();
			
			lowpower = 0;
			
			//TR0 = 0; //�رն�ʱ��
		}
	}
}

