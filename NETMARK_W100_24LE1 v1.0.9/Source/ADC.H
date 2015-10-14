#include "reg24le1.h"

#ifndef CONF_H__
#define CONF_H__

/*宏定义用用取出对应的位 例如：判断变量A最低位的情况,即可，if(A&BIT0)*/

#define BIT0  0X01
#define BIT1  0X02
#define BIT2  0X04
#define BIT3  0X08
#define BIT4  0X10
#define BIT5  0X20
#define BIT6  0X40
#define BIT7  0X80

#define TOASC(dat) (dat+'0')	    	//将数字转成对应的ASC码

void Adc_GetPower();
void Adc_AIN10();
unsigned int readadc(void);
void adc_convert(void);

#endif