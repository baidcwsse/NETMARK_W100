#include "main.h"

static u8 rtc_cnt=0;
//static u8 lse_cnt=0;//�ȴ�LSE ready ����
/*
 * ��������SoftReset
 * ����  �������λ
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void SoftReset(void)
{
 __set_FAULTMASK(1);      // ??????
 NVIC_SystemReset();// ??
}
/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
static void RTC_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
static void RTC_Configuration(void)
{
	rtc_cnt=0;
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

//   /* Enable LSE */
//   RCC_LSEConfig(RCC_LSE_ON);
//   /* Wait till LSE is ready */
//   while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//   {
// 		lse_cnt++;
// // 		USART_SendData(USART1, (unsigned char) lse_cnt);
// // 			while (!(USART1->SR & USART_FLAG_TXE));
// 		Delay(100000);
// 		if(lse_cnt >= 250)
// 		{
// 			SoftReset();
// 		}
// 	}

  /* Select HSE/128 as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);  //62.5KHz

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Alarm */
  RTC_ITConfig(RTC_IT_ALR, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1/37.5 sec ;make sure that the time of one counter is a AIS slot*/
  RTC_SetPrescaler(1666); /* RTC period = RTCCLK/RTC_PR = (62.5 KHz)/(1666+1) */
	
	/* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
	
	/*Sets the RTC alarm value:Sets the RTC alarm value to 1800sec*/
	RTC_SetAlarm(SEND_INTERVAL);  // RTCʱ�Ӽ�������ʱ϶��
	
	/* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
	
	/*Sets the RTC counter value:Sets the RTC counter value to 0*/
	RTC_SetCounter(0x0);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/*
 * ��������RTC_Init
 * ����  ����ʼ��RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Init(void)
{
		/* ����RTC���ж����ȼ� */
	RTC_NVIC_Configuration();


    /* RTC Configuration */
    RTC_Configuration();

}

/**
  * @brief  Gets the RTC Low counter value.
  * @param  None
  * @retval : RTC counter value.
  */
uint16_t Get_RTC_Counter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (tmp) ;
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_ALR);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
		
    /* Reset RTC Counter */
		RTC_SetCounter(0x0);
		/* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
		
		rtc_cnt++;

// 		if( (charging_flag==off) && rtc_cnt>=8)   //ֻ���ڷǳ��״̬�²Ÿ�λ //�޸�rtc_cnt��ֵ�Ըı�����������
// 		{
// 			SoftReset();  //soft reset
// 		}
//     if(rtc_cnt>=9)
// 			rtc_cnt=0;
  }
}
