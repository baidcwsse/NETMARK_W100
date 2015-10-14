
#include "reg24le1.h"		   //头文件包含定义寄存器地址
#include "adc.h"			   //下面那些函数的包含头文件

void Adc_GetPower()  //AIN10通道采集,电平检测
{
	ADCCON1 = 0x29;								// 选择AIN10，参考电压为VDD
	ADCCON2 = 0X00;                 //设置成单步转换并使能，速度为2ksps
	ADCCON3 |= 0XE0;				   			//精度12bit，数据右对齐
	
	ADCDATH &= 0XF0;				   //转换结果寄存器清0
	ADCDATL &= 0X00;
// 	P0DIR|=0X07;				   //设置转换的输入通道为0，1,2
// 	P0&=0XF8;				       //端口初始化为低
	//MISC = 1;                //允许其他中断IEN1.4 = 1；
}

void Adc_AIN10()  //AIN10通道采集，电量检测
{
	ADCCON1 = 0x29;								// 选择AIN10，参考电压为VDD
	ADCCON2 = 0X00;                 //设置成单步转换并使能，速度为2ksps
	ADCCON3 |= 0XE0;				   			//精度12bit，数据右对齐
	
	ADCDATH &= 0XF0;				   //转换结果寄存器清0
	ADCDATL &= 0X00;
// 	P0DIR|=0X07;				   //设置转换的输入通道为0，1,2
// 	P0&=0XF8;				       //端口初始化为低
	//MISC = 1;                //允许其他中断IEN1.4 = 1；
}

/*读取AD转换结果的功能函数，pip_num保存通道号，返回AD转换的结果*/
/*通过定义的静态变量来循环读取0.1.2三个通道*/
unsigned int readadc(void)
{
	 unsigned int res=0;

	ADCCON1 = ADCCON1 | 0x80;                 	// 启动ADC
	
	 while(!(ADCCON1&BIT6));		//等待启动
	 while((ADCCON1&BIT6));			//等待完成转换
	
	 res = ADCDATL+(ADCDATH&0X0F)*256;//读取结果
	 return res;
}

void adc_convert(void)
{
	ADCCON1 = ADCCON1 | 0x80;                 	// 启动ADC
	while ((ADCCON1 && 0x40) == 0x40);     		// 等待转换结束
}

