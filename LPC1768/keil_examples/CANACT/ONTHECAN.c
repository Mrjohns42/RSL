/****************************************************************************
 *   $Id:: ONTHECAN.c 6092 2011-01-06 22:55:04Z nxp12832                    $
 *   Project: NXP LPC17xx CAN Wakeup example
 *
 *   Description:
 *     This file contains CAN wakeup test modules, main entry.
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
#include <stdio.h>
#include <string.h>
#include "lpc17xx.h"
#include "type.h"
#include "can.h"
#include "uart.h"

/* Debug information on UART0
	UART enabled	= 	1
	UART disabled	=	0
	Baud Rate = 57600, 8,N,1 */
#define UART_ENABLE 1

#define SLEEP		1
#define DEEPSLEEP	2
#define POWERDOWN	3

/* Specify the reduced power mode 
	Select from SLEEP, DEEPSLEEP, or POWERDOWN */
#define POWERMODE	POWERDOWN
/* Note that only the DEEPSLEEP and POWERDOWN will 
	cause the CANActivity interupt to be generated */

volatile uint32_t canactflag;
CAN_MSG MsgBuf_TX1, MsgBuf_TX2; /* TX and RX Buffers for CAN message */
CAN_MSG MsgBuf_RX1, MsgBuf_RX2; /* TX and RX Buffers for CAN message */

volatile uint32_t CAN1RxDone, CAN2RxDone;

/*****************************************************************************
** Function name:		CANActivity_IRQHandler
**
** Descriptions:		ISR for CAN Activity interrupt
**
** parameters:			None
** Returned value:		int
** 
*****************************************************************************/
void CANActivity_IRQHandler(void)
{
  canactflag = 1;	

  /* Restore CAN channel clocks */	
  LPC_SC->CANSLEEPCLR = (1<<1) | (1<<2);

  /* Wakeup CAN controllers */
  LPC_CAN1->MOD =
  LPC_CAN2->MOD &= ~(1<<4);
	 
  /* Clear CAN1 & CAN2 WAKE FLAG */
  LPC_SC->CANWAKEFLAGS = (1<<1) | (1<<2);

  /* Toggle LED to indicated that the CANActivity Handler was executed */
  LPC_GPIO1->FIOPIN  ^= (1<<29);
  return;
}

/*****************************************************************************
** Function name:		main
**
** Descriptions:		main routine for CAN module test
**
** parameters:			None
** Returned value:		int
** 
*****************************************************************************/
int main( void )
{
  uint32_t i;
#if (POWERMODE == SLEEP)
  uint32_t j;
#endif
#if UART_ENABLE
  uint8_t textarray[100];
#endif

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
		
#if UART_ENABLE
  UARTInit(0, 57600);
#endif
	
  /* Please note, the bit timing is based on the setting of the 
  PCLK, if different PCLK is used, please read can.h carefully 
  and set your CAN bit timing accordingly. */  
  CAN_Init( BITRATE125K18MHZ );

  /* Enable the CAN Activity Interrupt */
  NVIC_EnableIRQ(CANActivity_IRQn);
	      
  /* send one message from CAN1(TX) and verify received message on 
  CAN2(RX) if it's a match, both CAN TX and RX are working. 
  For more details on acceptance filter program, see Philips
  appnote AN10438 and the zip file associated with this appnote. */
									
  /* GPIOs as outputs */
  LPC_GPIO1->FIODIR |= (1<<28) | (1<<29);
  LPC_GPIO1->FIOCLR  = (1<<28) | (1<<29);
	
  LPC_GPIO2->FIODIR |= (1<<6);
  LPC_GPIO2->FIOCLR  = (1<<6);
	
  /* Clear MsgBuf */
  MsgBuf_RX1.Frame = 0x0;
  MsgBuf_RX1.MsgID = 0x0;
  MsgBuf_RX1.DataA = 0x0;
  MsgBuf_RX1.DataB = 0x0;
  CAN_SetACCF( ACCF_BYPASS );

#if UART_ENABLE
  UARTSend(0, "Initialized\r\n", sizeof("Initialized\r\n"));
#endif

  i = 0;
  canactflag = 0;
  /* Test bypass */
  while ( 1 )
  {
	/* Waken up from deep sleep */
	if (canactflag) 
	{
	  canactflag = 0;
	    
	  /* Re-intialized the PLL and CAN interface */
	  SystemInit();
#if UART_ENABLE
	  UARTInit(0, 57600);
#endif
	  CAN_Init( BITRATE125K18MHZ );
		
#if UART_ENABLE
	  UARTSend(0, "\r\nWake up Complete\r\n", sizeof("\r\nWake up Complete\r\n"));
#endif
	}

	/* IF CAN packet received, turn on LED P1.28 */
	if(CAN1RxDone == TRUE)
	{
	  CAN1RxDone = FALSE;
	  LPC_GPIO1->FIOPIN ^= (1<<28);
			
#if UART_ENABLE
	  UARTSend(0, "\r\nCAN Packet Rx'd\r\n", sizeof("\r\nCAN Packet Rx'd\r\n"));
	  memset(textarray, '\0', sizeof(textarray));
	  sprintf((char *)textarray,"Frame: 0x%08X\r\n"
		"ID:    0x%08X\r\n"
		"DataA: 0x%08X\r\n"
		"DataB: 0x%08X\r\n",
		MsgBuf_RX1.Frame,
		MsgBuf_RX1.MsgID,
		MsgBuf_RX1.DataA,
		MsgBuf_RX1.DataB);
	  UARTSend(0, textarray, sizeof(textarray));
#endif
			
	  /* Message Data */
	  MsgBuf_RX1.Frame = 0x0;
	  MsgBuf_RX1.MsgID = 0x0;
	  MsgBuf_RX1.DataA = 0x0;
	  MsgBuf_RX1.DataB = 0x0;
	}

	/* If P2.10 (INT0) is pressed, go into Deep Sleep */
	if(!(LPC_GPIO2->FIOPIN & (1<<10)))
	{
	  LPC_GPIO1->FIOCLR = (1<<28) | (1<<29);
	  LPC_GPIO2->FIOCLR = (1<<6);
											 				   
#if UART_ENABLE
#if 	(POWERMODE == DEEPSLEEP)
	  UARTSend(0, "\r\nGoing to DEEP SLEEP\r\n", 	sizeof("\r\nGoing to DEEP SLEEP\r\n"));
#elif	(POWERMODE == POWERDOWN)
	  UARTSend(0, "\r\nGoing to POWER DOWN\r\n", 	sizeof("\r\nGoing to POWER DOWN\r\n"));
#else /*(POWERMODE == SLEEP)*/
	  UARTSend(0, "\r\nGoing to SLEEP\r\n", 		sizeof("\r\nGoing to SLEEP\r\n"));
	  /* Delay needed so that UART0 can clear out to prevent unintentional wakeups */
	  for(j = 0; j < 0x1000; j++);
#endif /* POWERMODE */
#endif /* UART_ENABLE */
	
	/* Specify the Powerdown mode */
#if 	(POWERMODE == DEEPSLEEP)
	  /* Specify WFI to enter DEEPSLEEP mode */
	  SCB->SCR		= (1<<2);	
#elif	(POWERMODE == POWERDOWN)
	  /* Specify WFI to enter POWERDOWN mode */
	  SCB->SCR		= 	(1<<2);
	  LPC_SC->PCON   |=	(1<<0);
#else 	/*(POWERMODE == SLEEP)*/
	  /* Default is SLEEP MODE */
#endif
			
	  /* Switching to reduced power mode */
	  __WFI();
	}

	/* Toggle P2.6 LED while CPU is running */
	if(i >= (1<<18))
	{
	  LPC_GPIO2->FIOPIN ^= (1<<6);
	  i = 0;
	} 
	else
	{
	  i++;
	}	
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
