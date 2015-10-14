/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 5717 $
 */ 

/** @file
 *@brief Implementation of hal_flash
 */

#include "reg24le1.h" 
#include "hal_flash.h"
#include "main.h"
#include "uart.h"

void hal_flash_page_erase(uint8_t pn)
{ 
  // Save interrupt enable state and disable interrupts:
  F0 = EA;
  EA = 0;

  // Enable flash write operation:
  WEN = 1;
 
  // Write the page address to FCR to start the page erase operation. This
  // operation is "self timed" when executing from the flash; the CPU will
  // halt until the operation is finished:
  FCR = pn;

  // When running from XDATA RAM we need to wait for the operation to finish:
  while(RDYN == 1)
    ;
	
  WEN = 0;
  
  EA = F0; // Restore interrupt enable state  
}

// void hal_flash_byte_write(uint16_t a, uint8_t b)
// {
//   uint8_t xdata *data pb;
//     
//   // Save interrupt enable state and disable interrupts:
//   F0 = EA;
//   EA = 0;
//   
//   // Enable flash write operation:
//   WEN = 1;
//   
//   // Write the byte directly to the flash. This operation is "self timed" when
//   // executing from the flash; the CPU will halt until the operation is
//   // finished:
//   pb = (uint8_t xdata *)a;
//   *pb = b;

//   // When running from XDATA RAM we need to wait for the operation to finish:
//   while(RDYN == 1)
//     ;

//   WEN = 0;

//   EA = F0; // Restore interrupt enable state
// }

void hal_flash_bytes_write(uint16_t a, uint8_t *p, uint16_t n)
{
  uint8_t xdata *data pb;

  // Save interrupt enable state and disable interrupts:
  F0 = EA;
  EA = 0;

  // Enable flash write operation:
  WEN = 1;

  // Write the bytes directly to the flash. This operation is
  // "self timed"; the CPU will halt until the operation is
  // finished:
  pb = (uint8_t xdata *)a;
  while(n--)
  {
    *pb++ = *p++;
    //
    // When running from XDATA RAM we need to wait for the operation to
    // finish:
    while(RDYN == 1)
      ;
  }

  WEN = 0;

  EA = F0; // Restore interrupt enable state
}

// uint8_t hal_flash_byte_read(uint16_t a)
// {
//   uint8_t xdata *pb = (uint8_t xdata *)a;
//   return *pb;
// }

void hal_flash_bytes_read(uint16_t a, uint8_t *p, uint16_t n)
{  
  uint8_t xdata *pb = (uint8_t xdata *)a;
  while(n--)
  {
    *p = *pb;
    pb++;
    p++;
  }
}

void WriteflashInterval(void)  //写入间隔
{
	ReadflashBoatnum(); //先读出船名，再擦写页
	ReadflashCode();
	ReadStartTask();
	
	hal_flash_page_erase(34);
	PCON &= ~0x10;//PMW

	timinteval_1 = com_buf[4];  //putch(timinteval_1);
	timinteval_2 = com_buf[5];  //putch(timinteval_2);
	timinteval = timinteval_1; timinteval = (timinteval << 8); timinteval = timinteval + timinteval_2;

	hal_flash_bytes_write(0xFC00,&timinteval_1,1);
	hal_flash_bytes_write(0xFC01,&timinteval_2,1);

	hal_flash_bytes_write(0xFC02,boatnum,16);  //再写入船名
	hal_flash_bytes_write(0xFC30,&codeflag,1);
//	hal_flash_bytes_write(0xFC31,&taskstart,1);
}

void ReadflashInterval(void)  //读出间隔
{
	PCON &= ~0x10;//PMW
	hal_flash_bytes_read(0xFC00,&timinteval_1,1);  //读入发射间隔
	hal_flash_bytes_read(0xFC01,&timinteval_2,1);  //读入发射间隔
	
	timinteval = timinteval_1; timinteval = (timinteval << 8); timinteval = timinteval + timinteval_2;
}

void WriteflashBoatnum(void)  //写入船名
{
	ReadflashInterval();
	ReadflashCode();
	ReadStartTask();
	
	hal_flash_page_erase(34);
	PCON &= ~0x10;//PMW
	
	hal_flash_bytes_write(0xFC02,&(com_buf[2]),16);
	delay(1000);
	hal_flash_bytes_read (0xFC02,boatnum,16);
	
	hal_flash_bytes_write(0xFC00,&timinteval_1,1); //再写入发射间隔
	hal_flash_bytes_write(0xFC01,&timinteval_2,1);
	hal_flash_bytes_write(0xFC30,&codeflag,1);
//	hal_flash_bytes_write(0xFC31,&taskstart,1);
}

void ReadflashBoatnum(void)  //读出船名
{
	PCON &= ~0x10;//PMW
	hal_flash_bytes_read(0xFC02,boatnum,16);
}

void WriteflashCode()  //写密码
{
	ReadflashInterval();
	ReadflashBoatnum();
	ReadStartTask();
	
	hal_flash_page_erase(34);
	PCON &= ~0x10;//PMW

	hal_flash_bytes_write(0xFC30,&codeflag,1);
	
	hal_flash_bytes_write(0xFC00,&timinteval_1,1);
	hal_flash_bytes_write(0xFC01,&timinteval_2,1);
	hal_flash_bytes_write(0xFC02,boatnum,16);
//	hal_flash_bytes_write(0xFC31,&taskstart,1);
}
	
void ReadflashCode()  //读密码
{
	PCON &= ~0x10;//PMW
	hal_flash_bytes_read(0xFC30,&codeflag,1);
}

void FlashInit(void)  //flash读出初始化
{
	ReadflashBoatnum();
	ReadflashInterval();
	ReadflashCode();
	ReadStartTask();
}

void WriteStartTask()  //写开机状态
{
	ReadflashBoatnum();
	ReadflashInterval();
	ReadflashCode();
	
	hal_flash_page_erase(34);
	PCON &= ~0x10;//PMW

//	hal_flash_bytes_write(0xFC31,&taskstart,1);
	delay(1000);
//	hal_flash_bytes_read(0xFC31,&taskstart,1);

	hal_flash_bytes_write(0xFC00,&timinteval_1,1);
	hal_flash_bytes_write(0xFC01,&timinteval_2,1);
	hal_flash_bytes_write(0xFC02,boatnum,16);
	hal_flash_bytes_write(0xFC30,&codeflag,1);
}

void ReadStartTask()  //读开机状态
{
	PCON &= ~0x10;//PMW
//	hal_flash_bytes_read(0xFC31,&taskstart,1);
}



