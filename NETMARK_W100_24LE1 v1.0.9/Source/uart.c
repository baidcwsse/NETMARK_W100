#include "reg24le1.h" 
#include "nRF24L01.h"
#include "main.h"
#include "string.h" 
#include "uart.h" 
#include "command.h"
#include "config.h"

INT8U uartnum=0;
static INT8U rev_start = 0;     
static INT8U rev_stop  = 0;     
INT8U com_buf[31];  //串口数组

void uart_init(void)
{	
	IEN0 &= ~( 1<<4 );				//Disable UART0 interrupt while initializeing
	S0CON = 0x50; 
	ADCON = 0x80;					//Enable baudrate generator
	PCON |= 0x80;					//mode1
	
  S0RELL = 0xCC;              // BAUD_9600
  S0RELH = 0x03;	
	IEN0 |= ( 1<<4 );				//Enable interrupt
	P0DIR |= ( 1<<4 );              //P0.4, RXD, set as input
	P0DIR &= ~( 1<<3 );             //P0.3, TXD, set as output
 	P0|=0x18;	
}

void putch(unsigned char ch)
{
	S0BUF = ch;
	while(!TI0);
	TI0=0;
}

void putstrForUart(unsigned char *s)
{
		unsigned char *cs = s;
		char i;
	
		for(i = 0;i < 18;i++)
		{
			putch(*(cs++));
		}
}

void putstrForTick(unsigned char *s)
{
		unsigned char *cs = s;
		char i;
	
		putch(0x24); //'$'
		for(i = 0;i < 17;i++)
		{
			putch(*(cs++));
		}
		
// 		unsigned char temp[18];
// 		char i;
// 	
// 		temp[0] = 0x24; //'$'
// 		for(i = 0;i < 17;i++)
// 			temp[i+1] = *(s+i);
// 	
// 		for(i = 0;i < 18;i++)
// 		{
// 			putch(temp[i]);
// 		}
}

void putstrForRF(unsigned char *s)
{
		unsigned char *cs = s;
		char i;
	
		putch(0x24); //'$'
		for(i = 0;i < 17;i++)
		{
			putch(*(cs++));
		}
}

void UART0_ISR(void) interrupt INTERRUPT_UART0
{
 		unsigned char ch;

		ES0 =0; //EA = 0;
	
    if( S0CON & ( 1<<1 ) )  //TX ok
    {
			S0CON &= ~( 1<<1 );
			
			uartnum = 0; //在这里清接收的数据标记
    }
    else if( S0CON & ( 1<<0 ) )  //RX ok
    {
			S0CON &= ~( 1<<0 );
			ch = S0BUF;
			
			if('$'==ch && uartnum == 0)
			{
				rev_start = 1;
				rev_stop  = 0;
			}
			if (1==rev_start)  
			{
				com_buf[uartnum++] = ch;  
				
				if (uartnum == 18)  //串口最大长度为18
				{
					rev_start = 0;
					rev_stop  = 1;
					
					watchdog_load(); //设置看门狗
					Command2();
					
					uartnum = 0;
					
					//powrdownmodeset(); //进入睡眠		
				}
			}			
		
    }
			
		ES0 = 1; //EA = 1;
}


