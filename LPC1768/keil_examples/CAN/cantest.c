/****************************************************************************
 *   $Id:: cantest.c 6092 2011-01-06 22:55:04Z nxp12832                     $
 *   Project: NXP LPC17xx CAN example
 *
 *   Description:
 *     This file contains CAN test modules, main entry, to test CAN APIs.
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
#include "can.h"

CAN_MSG MsgBuf_TX1, MsgBuf_TX2; /* TX and RX Buffers for CAN message */
CAN_MSG MsgBuf_RX1, MsgBuf_RX2; /* TX and RX Buffers for CAN message */

volatile uint32_t CAN1RxDone = FALSE, CAN2RxDone = FALSE;

#if CAN_WAKEUP
extern volatile uint32_t CANActivityInterruptFlag;
#endif

#if CAN_WAKEUP
/*****************************************************************************
** Function name:		CAN_WakeupTest
**
** Descriptions:		main routine for CAN wakeup test
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void CAN_WakeupTest( void )
{
  uint32_t i, j;

  CAN_SetACCF( ACCF_BYPASS );
  NVIC_EnableIRQ(CANActivity_IRQn);       /* enable CAN activity interrupt */

  /* Put the MCU into deep sleep mode first. Any CAN message on RX 
  should wake up the MCU. CAN Activity interrupt won't occur until Deepsleep 
  bit is set in SCR. UM needs to be updated regarding this. */
  SCB->SCR |= 0x04;	/* Set SLEEPDEEP bit in SCR in Cortex M3 core. */	
  LPC_SC->PCON = 0x1;
  __WFI();

  while (1)                            /* Loop forever */
  {
	if ( CANActivityInterruptFlag )
	{
	  /* If the MCU is waked up from power down mode, the PLL needs to
	  be reconfigured, and USB block needs tn be reset and reconnect. */
	  CANActivityInterruptFlag = 0;
	  while ( CAN2RxDone != TRUE );
	  CAN2RxDone = FALSE;

	  SystemInit();

	  CAN_Init( BITRATE125K18MHZ );
	  CAN_SetACCF( ACCF_BYPASS );

	  LPC_GPIO2->FIODIR = 0x000000FF;		/* P1.16..23 defined as Outputs */
	  LPC_GPIO2->FIOCLR = 0x000000FF;		/* turn off all the LEDs */
      
	  /* Wake up first time, now, blink 10 times. */
	  for ( j = 0; j < 10; j++ )
	  {
		for ( i = 0; i < 0x200000; i++ );
		LPC_GPIO2->FIOSET = 0x000000FF;
		for ( i = 0; i < 0x200000; i++ );
		LPC_GPIO2->FIOCLR = 0x000000FF;
	  }

	  /* Once the LED blink number of times, the MCU will go to sleep
	  or power down mode again. Another incoming CAN message will wake up
	  the MCU again. */
	  SCB->SCR |= 0x04;	/* Set SLEEPDEEP bit in SCR in Cortex M3 core. */	
	  LPC_SC->PCON = 0x1;
	  __WFI();
	}
  }
}
#endif

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
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /* Please note, the bit timing is based on the setting of the 
  PCLK, if different PCLK is used, please read can.h carefully 
  and set your CAN bit timing accordingly. */  
  //CAN_Init( BITRATE100K24MHZ );
  CAN_Init( BITRATE125K18MHZ );	

#if CAN_WAKEUP
  CAN_WakeupTest();
#endif
  
  /* send one message from CAN1(TX) and verify received message on 
  CAN2(RX) if it's a match, both CAN TX and RX are working. 
  For more details on acceptance filter program, see Philips
  appnote AN10438 and the zip file associated with this appnote. */

#if !ACCEPTANCE_FILTER_ENABLED
  /* Initialize MsgBuf */
  MsgBuf_TX1.Frame = 0x80080000; /* 29-bit, no RTR, DLC is 8 bytes */
  MsgBuf_TX1.MsgID = 0x00012345; /* CAN ID */
  MsgBuf_TX1.DataA = 0x3C3C3C3C;
  MsgBuf_TX1.DataB = 0xC3C3C3C3;

  MsgBuf_RX2.Frame = 0x0;
  MsgBuf_RX2.MsgID = 0x0;
  MsgBuf_RX2.DataA = 0x0;
  MsgBuf_RX2.DataB = 0x0;
  CAN_SetACCF( ACCF_BYPASS );

  /* Test bypass */
  while ( 1 )
  {
	/* Transmit initial message on CAN 1 */
	while ( !(LPC_CAN1->GSR & (1 << 3)) );
	if ( CAN1_SendMessage( &MsgBuf_TX1 ) == FALSE )
	{
	  continue;
	}
 	if ( CAN2RxDone == TRUE )
	{
	  CAN2RxDone = FALSE;
	  if ( MsgBuf_RX2.Frame & (1 << 10) )	/* by pass mode */
	  {
		MsgBuf_RX2.Frame &= ~(1 << 10 );
	  }
	  if ( ( MsgBuf_TX1.Frame != MsgBuf_RX2.Frame ) ||
			( MsgBuf_TX1.MsgID != MsgBuf_RX2.MsgID ) ||
			( MsgBuf_TX1.DataA != MsgBuf_RX2.DataA ) ||
			( MsgBuf_TX1.DataB != MsgBuf_RX2.DataB ) )
	  {
		while ( 1 );
	  }
	  /* Everything is correct, reset buffer */
	  MsgBuf_RX2.Frame = 0x0;
	  MsgBuf_RX2.MsgID = 0x0;
	  MsgBuf_RX2.DataA = 0x0;
	  MsgBuf_RX2.DataB = 0x0;
	} /* Message on CAN 2 received */
  }
#else
  /* Test Acceptance Filter */
  /* Even though the filter RAM is set for all type of identifiers,
  the test module tests explicit standard identifier only */
  MsgBuf_TX1.Frame = 0x00080000; /* 11-bit, no RTR, DLC is 8 bytes */
  MsgBuf_TX1.MsgID = EXP_STD_ID; /* Explicit Standard ID */
  MsgBuf_TX1.DataA = 0x55AA55AA;
  MsgBuf_TX1.DataB = 0xAA55AA55;

  MsgBuf_RX2.Frame = 0x0;
  MsgBuf_RX2.MsgID = 0x0;
  MsgBuf_RX2.DataA = 0x0;
  MsgBuf_RX2.DataB = 0x0;
  CAN_SetACCF( ACCF_ON );

  while ( 1 )
  {
	/* Transmit initial message on CAN 1 */
	while ( !(LPC_CAN1->GSR & (1 << 3)) );
	if ( CAN1_SendMessage( &MsgBuf_TX1 ) == FALSE )
	{
	  continue;
	}

	/* please note: FULLCAN identifier will NOT be received as it's not set 
	in the acceptance filter. */
 	if ( CAN2RxDone == TRUE )
	{
	  CAN2RxDone = FALSE;
	  /* The frame field is not checked, as ID index varies based on the
	  entries set in the filter RAM. */
	  if ( ( MsgBuf_TX1.MsgID != MsgBuf_RX2.MsgID ) ||
			( MsgBuf_TX1.DataA != MsgBuf_RX2.DataA ) ||
			( MsgBuf_TX1.DataB != MsgBuf_RX2.DataB ) )
	  {
		while ( 1 );
	  }
	  /* Everything is correct, reset buffer */
	  MsgBuf_RX2.Frame = 0x0;
	  MsgBuf_RX2.MsgID = 0x0;
	  MsgBuf_RX2.DataA = 0x0;
	  MsgBuf_RX2.DataB = 0x0;
	} /* Message on CAN 2 received */
  }
#endif
}

/******************************************************************************
**                            End Of File
******************************************************************************/
