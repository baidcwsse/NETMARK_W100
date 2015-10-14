#ifndef __RTC_H__
#define __RTC_H__

#include "reg24le1.h" 
#include "string.h"

void rtc2_init(void);
void sendmsg18(void);  //RF发送断开的命令
void Is_Charge(void);  //判断是否在充电
void adc_square(void);  //adc方波采样 用于连接
void abc(void);

#endif