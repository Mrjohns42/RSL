/****************************************************************************
 *   $Id:: ssptest.c 6098 2011-01-08 02:26:20Z nxp12832                     $
 *   Project: NXP LPC17xx SSP example
 *
 *   Description:
 *     This file contains SSP test modules, main entry, to test SSP APIs.
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
#include "LPC17xx.h"                        /* LPC13xx definitions */
#include "ssp.h"

/* Be careful with the port number and location number, because
some of the location may not exist in that port. */
#define PORT_NUM			0
#define LOCATION_NUM		0

uint8_t src_addr[SSP_BUFSIZE]; 
uint8_t dest_addr[SSP_BUFSIZE];

/*****************************************************************************
** Function name:		LoopbackTest
**
** Descriptions:		Loopback test
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void LoopbackTest( uint32_t portnum, uint32_t location )
{
  uint32_t i;

#if !USE_CS
  /* Set SSEL pin to output low. */
  SSP_SSELToggle( portnum, 0 );
#endif
  i = 0;
  while ( i <= SSP_BUFSIZE )
  {
	/* to check the RXIM and TXIM interrupt, I send a block data at one time 
	based on the FIFOSIZE(8). */
	SSPSend( portnum, (uint8_t *)&src_addr[i], FIFOSIZE );
	/* If RX interrupt is enabled, below receive routine can be
	also handled inside the ISR. */
	SSPReceive( portnum, (uint8_t *)&dest_addr[i], FIFOSIZE );
	i += FIFOSIZE;
  }
#if !USE_CS
  /* Set SSEL pin to output high. */
  SSP_SSELToggle( portnum, 1 );
#endif
   
  /* verifying write and read data buffer. */
  for ( i = 0; i < SSP_BUFSIZE; i++ )
  {
	if ( src_addr[i] != dest_addr[i] )
	{
	  while( 1 );			/* Verification failed */
	}
  }
  return;
}

/*****************************************************************************
** Function name:		SEEPROMTest
**
** Descriptions:		Serial EEPROM(Atmel 25xxx) test
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SEEPROMTest( uint32_t portnum, uint32_t location )
{
  uint32_t i, timeout;
#if SSP_DEBUG
  uint8_t temp[2];
#endif

  if ( portnum == 1 )
  {
	LPC_GPIO0->FIODIR |= (0x1<<16);		/* SSP1, P0.16 defined as Outputs */
  }
  else
  {
	LPC_GPIO0->FIODIR |= (0x1<<6);		/* SSP0 P0.6 defined as Outputs */
  }
  SSP_SSELToggle( portnum, 0 );

  /* Test Atmel 25016 SPI SEEPROM. */
  src_addr[0] = WREN;			/* set write enable latch */
  SSPSend( portnum, (uint8_t *)src_addr, 1 );
  SSP_SSELToggle( portnum, 1 );

  for ( i = 0; i < DELAY_COUNT; i++ );	/* delay minimum 250ns */

  SSP_SSELToggle( portnum, 0 );
  src_addr[0] = RDSR;	/* check status to see if write enabled is latched */
  SSPSend( portnum, (uint8_t *)src_addr, 1 );
  SSPReceive( portnum, (uint8_t *)dest_addr, 1 );
  SSP_SSELToggle( portnum, 1 );
  if ( dest_addr[0] & (RDSR_WEN|RDSR_RDY) != RDSR_WEN ) 
  /* bit 0 to 0 is ready, bit 1 to 1 is write enable */
  {
	while ( 1 );
  }

  for ( i = 0; i < SSP_BUFSIZE; i++ )	/* Init RD and WR buffer */    
  {
	src_addr[i+3] = i;	/* leave three bytes for cmd and offset(16 bits) */
	dest_addr[i] = 0;
  }

  /* please note the first two bytes of WR and RD buffer is used for
  commands and offset, so only 2 through SSP_BUFSIZE is used for data read,
  write, and comparison. */
  SSP_SSELToggle( portnum, 0 );
  src_addr[0] = WRITE;	/* Write command is 0x02, low 256 bytes only */
  src_addr[1] = 0x00;	/* write address offset MSB is 0x00 */
  src_addr[2] = 0x00;	/* write address offset LSB is 0x00 */
  SSPSend( portnum, (uint8_t *)src_addr, SSP_BUFSIZE );
  SSP_SSELToggle( portnum, 1 );

  for ( i = 0; i < 0x30000; i++ );	/* delay, minimum 3ms */
  
  timeout = 0;
  while ( timeout < MAX_TIMEOUT )
  {
	SSP_SSELToggle( portnum, 0 );
	src_addr[0] = RDSR;	/* check status to see if write cycle is done or not */
	SSPSend( portnum, (uint8_t *)src_addr, 1);
	SSPReceive( portnum, (uint8_t *)dest_addr, 1 );
	SSP_SSELToggle( portnum, 1 );
	if ( (dest_addr[0] & RDSR_RDY) == 0x00 )	/* bit 0 to 0 is ready */
	{
	    break;
	}
	timeout++;
  }
  if ( timeout == MAX_TIMEOUT )
  {
	while ( 1 );
  }

  for ( i = 0; i < DELAY_COUNT; i++ );	/* delay, minimum 250ns */

  SSP_SSELToggle( portnum, 0 );
  src_addr[0] = READ;		/* Read command is 0x03, low 256 bytes only */
  src_addr[1] = 0x00;		/* Read address offset MSB is 0x00 */
  src_addr[2] = 0x00;		/* Read address offset LSB is 0x00 */
  SSPSend( portnum, (uint8_t *)src_addr, 3 ); 
  SSPReceive( portnum, (uint8_t *)&dest_addr[3], SSP_BUFSIZE-3 );
  SSP_SSELToggle( portnum, 1 );

  /* verifying, ignore the difference in the first two bytes */
  for ( i = 3; i < SSP_BUFSIZE; i++ )
  {
	if ( src_addr[i] != dest_addr[i] )
	{
	  while( 1 );			/* Verification failed */
	}
  }
  return;
}

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t i, portnum = PORT_NUM;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  if ( portnum == 0 )
	SSP0Init();			/* initialize SSP port */
  else if ( portnum == 1 )
	SSP1Init();
  
  for ( i = 0; i < SSP_BUFSIZE; i++ )
  {
	src_addr[i] = (uint8_t)i;
	dest_addr[i] = 0;
  }

#if TX_RX_ONLY
  /* For the inter-board communication, one board is set as
  master transmit, the other is set to slave receive. */
#if SSP_SLAVE
  /* Slave receive */
  SSPReceive( portnum, (uint8_t *)dest_addr, SSP_BUFSIZE );
  for ( i = 0; i < SSP_BUFSIZE; i++ )
  {
	if ( src_addr[i] != dest_addr[i] )
	{
	  while ( 1 );				/* Verification failure, fatal error */
	} 
  }
#else
  /* Master transmit */
  SSPSend( portnum, (uint8_t *)src_addr, SSP_BUFSIZE);
#endif
#else
  /* TX_RX_ONLY=0, it's either an internal loopback test
  within SSP peripheral or communicate with a serial EEPROM. */
#if LOOPBACK_MODE
  LoopbackTest( portnum, LOCATION_NUM );
#else
  SEEPROMTest( portnum, LOCATION_NUM );
#endif			/* endif NOT LOOPBACK_MODE */
#endif			/* endif NOT TX_RX_ONLY */
  /* Never exit from main(), for easy debugging. */
  while ( 1 );
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

