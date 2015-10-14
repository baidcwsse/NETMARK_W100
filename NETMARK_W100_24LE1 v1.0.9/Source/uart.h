#ifndef __UART_H__
#define __UART_H__

#include "reg24le1.h" 
#include "string.h"

void uart_init(void);
void putch(unsigned char ch);
void putstrForUart(unsigned char *s);
void putstrForTick(unsigned char *s);
void putstrForRF(unsigned char *s);
void putstr18(unsigned char *s);
extern INT8U com_buf[31];  //´®¿ÚÊý×é

#endif