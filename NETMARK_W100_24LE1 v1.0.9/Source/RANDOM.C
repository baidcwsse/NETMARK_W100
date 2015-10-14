#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"
#include "uart.h"
#include "config.h"
#include "command.h"
#include "adc.h"
#include "rtc.h"
#include "hal_flash.h"

void radmcof(void)
{
	RNGCTL|=(0x80|0x40);
}

unsigned char readrdm(void)
{
	while(!RDMRD);//µÈ´ýÉú³É
	if(RDMRD)
		return RNGDAT;
	else
		return RNGDAT;
}

