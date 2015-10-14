
#include "reg24le1.h"		   //ͷ�ļ���������Ĵ�����ַ
#include "adc.h"			   //������Щ�����İ���ͷ�ļ�

void Adc_GetPower()  //AIN10ͨ���ɼ�,��ƽ���
{
	ADCCON1 = 0x29;								// ѡ��AIN10���ο���ѹΪVDD
	ADCCON2 = 0X00;                 //���óɵ���ת����ʹ�ܣ��ٶ�Ϊ2ksps
	ADCCON3 |= 0XE0;				   			//����12bit�������Ҷ���
	
	ADCDATH &= 0XF0;				   //ת������Ĵ�����0
	ADCDATL &= 0X00;
// 	P0DIR|=0X07;				   //����ת��������ͨ��Ϊ0��1,2
// 	P0&=0XF8;				       //�˿ڳ�ʼ��Ϊ��
	//MISC = 1;                //���������ж�IEN1.4 = 1��
}

void Adc_AIN10()  //AIN10ͨ���ɼ����������
{
	ADCCON1 = 0x29;								// ѡ��AIN10���ο���ѹΪVDD
	ADCCON2 = 0X00;                 //���óɵ���ת����ʹ�ܣ��ٶ�Ϊ2ksps
	ADCCON3 |= 0XE0;				   			//����12bit�������Ҷ���
	
	ADCDATH &= 0XF0;				   //ת������Ĵ�����0
	ADCDATL &= 0X00;
// 	P0DIR|=0X07;				   //����ת��������ͨ��Ϊ0��1,2
// 	P0&=0XF8;				       //�˿ڳ�ʼ��Ϊ��
	//MISC = 1;                //���������ж�IEN1.4 = 1��
}

/*��ȡADת������Ĺ��ܺ�����pip_num����ͨ���ţ�����ADת���Ľ��*/
/*ͨ������ľ�̬������ѭ����ȡ0.1.2����ͨ��*/
unsigned int readadc(void)
{
	 unsigned int res=0;

	ADCCON1 = ADCCON1 | 0x80;                 	// ����ADC
	
	 while(!(ADCCON1&BIT6));		//�ȴ�����
	 while((ADCCON1&BIT6));			//�ȴ����ת��
	
	 res = ADCDATL+(ADCDATH&0X0F)*256;//��ȡ���
	 return res;
}

void adc_convert(void)
{
	ADCCON1 = ADCCON1 | 0x80;                 	// ����ADC
	while ((ADCCON1 && 0x40) == 0x40);     		// �ȴ�ת������
}

