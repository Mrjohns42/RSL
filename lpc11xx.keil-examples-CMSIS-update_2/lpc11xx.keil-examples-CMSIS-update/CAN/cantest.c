/****************************************************************************
 *   $Id:: cantest.c 9362 2012-04-19 22:37:53Z nxp41306                     $
 *   Project: NXP LPC13xx CAN example
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

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
****************************************************************************/
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "type.h"
#include "gpio.h"
#include "can.h"

extern message_object can_buff[MSG_OBJ_MAX];
extern uint32_t CANRxDone[MSG_OBJ_MAX];

/******************************************************************************
**   TRANSIT ONLY or LOOPBACK mode
******************************************************************************/
void TX_Only_test( void )
{
#if LOOPBACK_MODE
  /* If loopback mode, set the same ID for both RX and TX MSG */ 
  can_buff[0].id      = RX_MSG_ID;	/* standard frame */
#else
  can_buff[0].id      = TX_MSG_ID;	/* standard frame */
#endif
  can_buff[0].dlc     = 0x08;		/* Length = 8 */
  can_buff[0].data[0] = 0x0001;
  can_buff[0].data[1] = 0x0203;
  can_buff[0].data[2] = 0x0405;
  can_buff[0].data[3] = 0x0607;

  while ( 1 )
  {
	CAN_Send( 0, FALSE, (uint32_t *)&can_buff[0] );
#if LOOPBACK_MODE
	/* In the loopback mode, it seems that the Bosch CAN IP can only 
	work if the TX and RX uses the same message box. If TX and RX are
	different, the RX won't work. */
	while ( !(LPC_CAN->STAT & STAT_TXOK) );
	LPC_CAN->STAT &= ~STAT_TXOK;
	CAN_MessageProcess( 0 );
#endif
  }
}

/******************************************************************************
**   Neither TRANSIT ONLY nor LOOPBACK mode, whenever CAN receives a message,
**   it bounces back/transmits a message with diferent msg ID back to where
**   it sends.
******************************************************************************/
void RX_TX_Bouncing( void )
{
  uint32_t i;

  /* Clear all CAN RX done flag. */
  for ( i = 0; i < MSG_OBJ_MAX; i++ )
  {
	CANRxDone[i] = FALSE;
  }

  while ( 1 )
  {
#if POLLING
#if BASIC_MODE
	while ( !(LPC_CAN->STAT & STAT_RXOK) );
	LPC_CAN->STAT &= ~STAT_RXOK;
	/* In BASIC mode, the message Object number doesn't apply. */
	CAN_MessageProcess( 0 );
#else
	/* first half of message object(0~15) or message object 1 to 16 */
	for ( i = 0; i < MSG_OBJ_MAX/2; i++ )
	{
	  if ( LPC_CAN->ND1 & (0x01<<i) )
	  {
		LPC_CAN->STAT &= ~STAT_RXOK;
		CAN_MessageProcess( i );
		/* Use the received data as the TX data field, and the next 
		message object slot for TX. */ 
		CAN_Send(i, TRUE, (uint32_t *)&can_buff[i]);
	  }
	}
	/* second half of message obj(16~31), or message object 17 to 32 */
	for ( i = MSG_OBJ_MAX/2; i < MSG_OBJ_MAX; i++ )
	{
	  if ( LPC_CAN->ND2 & (0x01<<(i-MSG_OBJ_MAX/2)) )
	  {
		LPC_CAN->STAT &= ~STAT_RXOK;
		CAN_MessageProcess( i );
		/* Use the received data as the TX data field, and the next 
		message object slot for TX. */
		CAN_Send(i, TRUE, (uint32_t *)&can_buff[i]);
	  }
	}
#endif		/* Endif BASIC_MODE */
#else		/* Not POLLING, it's interrupt driven. */
	/* The CANRxDone[x] flag is set in the interrupt handler where x indicates
	which message object receives the message. */
	for ( i = 0; i < MSG_OBJ_MAX; i++ )
	{
	  if ( CANRxDone[i] == TRUE )
	  {
		CAN_Send(i, TRUE, (uint32_t *)&can_buff[i]);
		CANRxDone[i] = FALSE;
	  }
	}
#endif		/* End POLLING */
  }
}

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t *add;

  SystemCoreClockUpdate();

  /* Clear all message buffers allocated for 32 CAN message objects. */
  add = (uint32_t *)can_buff;
  while ( add < (uint32_t *)(&can_buff[MSG_OBJ_MAX-1] + 1))
  {
	*add++ = 0x00;
  }

  /* initialize CAN clock, NVIC and CAN control register */
  /* Once USE_DEFAULT_BIT_TIMING is set, it uses the default setting 
  in the CAN bit timing configuration value. In this case, 
  CAN clock should be 8MHz CAN clock bitrate should be 500kbps!!!
  Below Bit Clock value doesn't apply. Please set the clock on the PC
  side accordingly. */
#if USE_DEFAULT_BIT_TIMING
  CAN_Init( BITRATE500K8MHZ );
#else
  CAN_Init( BITRATE125K8MHZ );			
#endif
						
#if (TRANSMIT_ONLY | LOOPBACK_MODE)
  TX_Only_test();
#else			/* Neither LOOPBACK nor TRANSMIT_ONLY */
  /* Below is the RX_TX bouncing example, use the CAN traffic analyzer
  or CAN message generator(e.g. PCAN Viewer or CANalyzer) on a 
  PC workstation to send messages to LPC11xx, once the firmware 
  receives the matching message, it will send a corresponding message 
  back to the PC. */
  RX_TX_Bouncing();
#endif			/* End if (TRANSMIT_ONLY | LOOPBACK_MODE) */

  /* Leave the test mode if test mode is set. */
  if ( LPC_CAN->CNTL & CTRL_TEST )
  {
	LPC_CAN->CNTL &= ~CTRL_TEST;
  }
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

