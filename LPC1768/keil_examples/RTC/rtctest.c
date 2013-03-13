/****************************************************************************
 *   $Id:: rtctest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx RTC example
 *
 *   Description:
 *     This file contains RTC test modules, main entry, to test RTC APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "lpc17xx.h"
#include "type.h"
#include "rtc.h"

extern volatile uint32_t alarm_on;
RTCTime local_time, alarm_time, current_time;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{ 
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /* Initialize RTC module */
  RTCInit();

  local_time.RTC_Sec = 0;
  local_time.RTC_Min = 0;
  local_time.RTC_Hour = 0;
  local_time.RTC_Mday = 8;
  local_time.RTC_Wday = 3;
  local_time.RTC_Yday = 12;		/* current date 07/12/2006 */
  local_time.RTC_Mon = 7;
  local_time.RTC_Year = 2006;
  RTCSetTime( local_time );		/* Set local time */

  alarm_time.RTC_Sec = 0;
  alarm_time.RTC_Min = 0;
  alarm_time.RTC_Hour = 0;
  alarm_time.RTC_Mday = 1;
  alarm_time.RTC_Wday = 0;
  alarm_time.RTC_Yday = 1;		/* alarm date 01/01/2007 */
  alarm_time.RTC_Mon = 1;
  alarm_time.RTC_Year = 2007;
  RTCSetAlarm( alarm_time );		/* set alarm time */

  NVIC_EnableIRQ(RTC_IRQn);

  /* mask off alarm mask, turn on IMYEAR in the counter increment interrupt
  register */
  RTCSetAlarmMask(AMRSEC|AMRMIN|AMRHOUR|AMRDOM|AMRDOW|AMRDOY|AMRMON|AMRYEAR);
  LPC_RTC->CIIR = IMMIN | IMYEAR;
  /* 2007/01/01/00:00:00 is the alarm on */
    
  RTCStart();

  while (1) 
  {					/* Loop forever */
	current_time = RTCGetTime();
	if ( alarm_on != 0 )
	{
	  alarm_on = 0;
	  /* Get current time when alarm is on */
	  current_time = RTCGetTime();	
	}
  }

}

/******************************************************************************
**                            End Of File
******************************************************************************/
