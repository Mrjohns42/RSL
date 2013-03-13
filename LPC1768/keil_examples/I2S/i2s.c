/****************************************************************************
 *   $Id:: i2s.c 5797 2010-12-03 00:27:00Z usb00423                         $
 *   Project: NXP LPC17xx I2S example
 *
 *   Description:
 *     This file contains I2S code example which include I2S initialization, 
 *     I2C interrupt handler, and APIs for I2S access.
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
#include "i2s.h"
#include "dma.h"

/* treat I2S TX and RX as a constant address, make the code and buffer 
easier for both DMA and non-DMA test */
volatile uint8_t *I2STXBuffer = (uint8_t *)(DMA_SRC); 
volatile uint8_t *I2SRXBuffer = (uint8_t *)(DMA_DST);
volatile uint32_t I2SReadLength = 0;
volatile uint32_t I2SWriteLength = 0;
volatile uint32_t I2SRXDone = 0, I2STXDone = 0;

/*****************************************************************************
** Function name:		I2S_IRQHandler
**
** Descriptions:		I2S interrupt handler, only RX interrupt is enabled
**						for simplicity.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2S_IRQHandler (void)  
{
  uint32_t RxCount = 0;

  if ( LPC_I2S->STATE & 0x01 )
  {
	RxCount = (LPC_I2S->STATE >> 8) & 0xFF;
	if ( (RxCount != RXFIFO_EMPTY) && !I2SRXDone )
	{
	  while ( RxCount > 0 )
	  {
		if ( I2SReadLength == BUFSIZE )
		{
		  LPC_I2S->DAI |= ((0x01 << 3) | (0x01 << 4));
		  LPC_I2S->IRQ &= ~(0x01 << 0);	/* Disable RX */	
		  I2SRXDone = 1;
		  break;
		}
		else
		{
		  I2SRXBuffer[I2SReadLength++] = LPC_I2S->RXFIFO;
		}
		RxCount--;
	  }
	}
  }
  return;
}

/*****************************************************************************
** Function name:		I2SStart
**
** Descriptions:		Start I2S DAI and DAO
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2SStart( void )
{
  uint32_t DAIValue, DAOValue;
  
  /* Audio output is the master, audio input is the slave, */
  /* 16 bit data, stereo, reset, master mode, not mute. */
  DAOValue = LPC_I2S->DAO;
  DAIValue = LPC_I2S->DAI;
  LPC_I2S->DAO = DAOValue & (~((0x01 << 4)|(0x01 <<3)));
  /* 16 bit data, stereo, reset, slave mode, not mute. */
  LPC_I2S->DAI = DAIValue & (~((0x01 << 4)|(0x01 <<3)));
  return;
}

/*****************************************************************************
** Function name:		I2SStop
**
** Descriptions:		Stop I2S DAI and DAO
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2SStop( void )
{
  uint32_t DAIValue, DAOValue;

  /* Stop the I2S to start. Audio output is master, audio input is the slave. */
  /* 16 bit data, set STOP and RESET bits to reset the channels */
  DAOValue = LPC_I2S->DAO;
  /* Switch to master mode, TX channel, no mute */
  DAOValue &= ~((0x01 << 5)|(0x01 << 15));
  DAIValue = LPC_I2S->DAI;
  DAIValue &= ~(0x01 << 15);
  LPC_I2S->DAO = (0x01 << 4) | (0x01 << 3) | DAOValue;	/* Master */
  LPC_I2S->DAI = (0x01 << 4) | (0x01 << 3) | DAIValue;	/* Slave */
  return;
}

/*****************************************************************************
** Function name:		I2SInit
**
** Descriptions:		Initialize I2S controller
**
** parameters:			None
** Returned value:		true or false, return false if the I2S
**						interrupt handler was not installed correctly
** 
*****************************************************************************/
void I2SInit( void ) 
{

  /*enable I2S in the PCONP register. I2S is disabled on reset*/
  LPC_SC->PCONP |= (1 << 27);

  /*connect the I2S sigals to port pins(P0.4-P0.9)*/
  LPC_PINCON->PINSEL0 &= ~0x000FFF00;
  LPC_PINCON->PINSEL0 |= 0x00055500;

  /* Please note, in order to generate accurate TX/RX clock rate for I2S, 
  PCLK and CCLK needs to be carefully reconsidered. For this test 
  program, the TX is looped back to RX without external I2S device, 
  clock rate is not critical in this matter. */
  LPC_I2S->TXRATE = 0x241;
  LPC_I2S->RXRATE = 0x241;

  I2SStop();
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

