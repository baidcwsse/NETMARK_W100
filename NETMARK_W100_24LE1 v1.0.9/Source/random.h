#ifndef __RTC_H__
#define __RTC_H__

#define RDMRD     (RNGCTL&0x20)

void radmcof(void);
unsigned char readrdm(void);

#endif