#include "hal_rtc.h"
#include "nordic_common.h"
#include "rtc.h"
#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"

void watchdog_load()   //5sec
{

// 	WDSV = (640%256);          //����Ͱ�λ��640%256 = 128��������λ1000 0000��
// 	delay(10);
// 	WDSV += (640/256);          //�ٸ�ֵ�߰�λ��640/256 = 2��������λ0000 0010��


	//puts("\n  the le1 will restart in 5 seconds! \n");
}
