/****************************************************************************
 *   $Id:: i2c.c 5865 2010-12-08 21:42:21Z usb00423                         $
 *   Project: NXP LPC17xx I2C example
 *
 *   Description:
 *     This file contains I2C code example which include I2C initialization, 
 *     I2C interrupt handler, and APIs for I2C access.
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
#include "i2c.h"

volatile uint32_t I2CMasterState[I2C_PORT_NUM] = {I2C_IDLE,I2C_IDLE,I2C_IDLE};
volatile uint32_t timeout[I2C_PORT_NUM] = {0, 0, 0};

volatile uint8_t I2CMasterBuffer[I2C_PORT_NUM][BUFSIZE];
volatile uint8_t I2CSlaveBuffer[I2C_PORT_NUM][BUFSIZE];
volatile uint32_t I2CCount[I2C_PORT_NUM] = {0, 0, 0};
volatile uint32_t I2CReadLength[I2C_PORT_NUM];
volatile uint32_t I2CWriteLength[I2C_PORT_NUM];

volatile uint32_t RdIndex0 = 0, RdIndex1 = 0, RdIndex2 = 0;
volatile uint32_t WrIndex0 = 0, WrIndex1 = 0, WrIndex2 = 0;

/* 
From device to device, the I2C communication protocol may vary, 
in the example below, the protocol uses repeated start to read data from or 
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
for master write: the sequence is: STA,Addr(W),offset,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could 
be READ or WRITE depending on the I2C command.
*/   

/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C0_IRQHandler(void) 
{
  uint8_t StatValue;

  timeout[0] = 0;
  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C0->STAT;
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
	WrIndex0 = 0;
	LPC_I2C0->DAT = I2CMasterBuffer[0][WrIndex0++];
	LPC_I2C0->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x10:			/* A repeated started is issued */
	RdIndex0 = 0;
	/* Send SLA with R bit set, */
	LPC_I2C0->DAT = I2CMasterBuffer[0][WrIndex0++];
	LPC_I2C0->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x18:			/* Regardless, it's a ACK */
	if ( I2CWriteLength[0] == 1 )
	{
	  LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
	  I2CMasterState[0] = I2C_NO_DATA;
	}
	else
	{
	  LPC_I2C0->DAT = I2CMasterBuffer[0][WrIndex0++];
	}
	LPC_I2C0->CONCLR = I2CONCLR_SIC;
	break;

	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	if ( WrIndex0 < I2CWriteLength[0] )
	{   
	  LPC_I2C0->DAT = I2CMasterBuffer[0][WrIndex0++]; /* this should be the last one */
	}
	else
	{
	  if ( I2CReadLength[0] != 0 )
	  {
		LPC_I2C0->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
	  }
	  else
	  {
		LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
		I2CMasterState[0] = I2C_OK;
	  }
	}
	LPC_I2C0->CONCLR = I2CONCLR_SIC;
	break;

	case 0x30:
	LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
	I2CMasterState[0] = I2C_NACK_ON_DATA;
	LPC_I2C0->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x40:	/* Master Receive, SLA_R has been sent */
	if ( (RdIndex0 + 1) < I2CReadLength[0] )
	{
	  /* Will go to State 0x50 */
	  LPC_I2C0->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  /* Will go to State 0x58 */
	  LPC_I2C0->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
	}
	LPC_I2C0->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	I2CSlaveBuffer[0][RdIndex0++] = LPC_I2C0->DAT;
	if ( (RdIndex0 + 1) < I2CReadLength[0] )
	{   
	  LPC_I2C0->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  LPC_I2C0->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
	}
	LPC_I2C0->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x58:
	I2CSlaveBuffer[0][RdIndex0++] = LPC_I2C0->DAT;
	I2CMasterState[0] = I2C_OK;
	LPC_I2C0->CONSET = I2CONSET_STO;	/* Set Stop flag */ 
	LPC_I2C0->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
	break;

	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
	LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
	I2CMasterState[0] = I2C_NACK_ON_ADDRESS;
	LPC_I2C0->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	I2CMasterState[0] = I2C_ARBITRATION_LOST;
	LPC_I2C0->CONCLR = I2CONCLR_SIC;	
	break;
  }
  return;
}

/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C1_IRQHandler(void) 
{
  uint8_t StatValue;

  timeout[1] = 0;
  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C1->STAT;
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
	WrIndex1 = 0;
	LPC_I2C1->DAT = I2CMasterBuffer[1][WrIndex1++];
	LPC_I2C1->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x10:			/* A repeated started is issued */
	RdIndex1 = 0;
	/* Send SLA with R bit set, */
	LPC_I2C1->DAT = I2CMasterBuffer[1][WrIndex1++];
	LPC_I2C1->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x18:			/* Regardless, it's a ACK */
	if ( I2CWriteLength[1] == 1 )
	{
	  LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
	  I2CMasterState[1] = I2C_NO_DATA;
	}
	else
	{
	  LPC_I2C1->DAT = I2CMasterBuffer[1][WrIndex1++];
	}
	LPC_I2C1->CONCLR = I2CONCLR_SIC;
	break;

	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	if ( WrIndex1 < I2CWriteLength[1] )
	{   
	  LPC_I2C1->DAT = I2CMasterBuffer[1][WrIndex1++]; /* this should be the last one */
	}
	else
	{
	  if ( I2CReadLength[1] != 0 )
	  {
		LPC_I2C1->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
	  }
	  else
	  {
		LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
		I2CMasterState[1] = I2C_OK;
	  }
	}
	LPC_I2C1->CONCLR = I2CONCLR_SIC;
	break;

	case 0x30:
	LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
	I2CMasterState[1] = I2C_NACK_ON_DATA;
	LPC_I2C1->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x40:	/* Master Receive, SLA_R has been sent */
	if ( (RdIndex1 + 1) < I2CReadLength[1] )
	{
	  /* Will go to State 0x50 */
	  LPC_I2C1->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  /* Will go to State 0x58 */
	  LPC_I2C1->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
	}
	LPC_I2C1->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	I2CSlaveBuffer[1][RdIndex1++] = LPC_I2C1->DAT;
	if ( (RdIndex1 + 1) < I2CReadLength[1] )
	{   
	  LPC_I2C1->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  LPC_I2C1->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
	}
	LPC_I2C1->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x58:
	I2CSlaveBuffer[1][RdIndex1++] = LPC_I2C1->DAT;
	I2CMasterState[1] = I2C_OK;
	LPC_I2C1->CONSET = I2CONSET_STO;	/* Set Stop flag */ 
	LPC_I2C1->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
	break;

	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
	LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
	I2CMasterState[1] = I2C_NACK_ON_ADDRESS;
	LPC_I2C1->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	I2CMasterState[1] = I2C_ARBITRATION_LOST;
	LPC_I2C1->CONCLR = I2CONCLR_SIC;	
	break;
  }
  return;
}

/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C2_IRQHandler(void) 
{
  uint8_t StatValue;

  timeout[2] = 0;
  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C2->STAT;
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
	WrIndex2 = 0;
	LPC_I2C2->DAT = I2CMasterBuffer[2][WrIndex2++];
	LPC_I2C2->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x10:			/* A repeated started is issued */
	RdIndex2 = 0;
	/* Send SLA with R bit set, */
	LPC_I2C2->DAT = I2CMasterBuffer[2][WrIndex2++];
	LPC_I2C2->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x18:			/* Regardless, it's a ACK */
	if ( I2CWriteLength[2] == 1 )
	{
	  LPC_I2C2->CONSET = I2CONSET_STO;      /* Set Stop flag */
	  I2CMasterState[2] = I2C_NO_DATA;
	}
	else
	{
	  LPC_I2C2->DAT = I2CMasterBuffer[2][WrIndex2++];
	}
	LPC_I2C2->CONCLR = I2CONCLR_SIC;
	break;

	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	if ( WrIndex2 < I2CWriteLength[2] )
	{   
	  LPC_I2C2->DAT = I2CMasterBuffer[2][WrIndex2++]; /* this should be the last one */
	}
	else
	{
	  if ( I2CReadLength[2] != 0 )
	  {
		LPC_I2C2->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
	  }
	  else
	  {
		LPC_I2C2->CONSET = I2CONSET_STO;      /* Set Stop flag */
		I2CMasterState[2] = I2C_OK;
	  }
	}
	LPC_I2C2->CONCLR = I2CONCLR_SIC;
	break;

	case 0x30:
	LPC_I2C2->CONSET = I2CONSET_STO;      /* Set Stop flag */
	I2CMasterState[2] = I2C_NACK_ON_DATA;
	LPC_I2C2->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x40:	/* Master Receive, SLA_R has been sent */
	if ( (RdIndex2 + 1) < I2CReadLength[2] )
	{
	  /* Will go to State 0x50 */
	  LPC_I2C2->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  /* Will go to State 0x58 */
	  LPC_I2C2->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
	}
	LPC_I2C2->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	I2CSlaveBuffer[2][RdIndex2++] = LPC_I2C2->DAT;
	if ( (RdIndex2 + 1) < I2CReadLength[2] )
	{   
	  LPC_I2C2->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	}
	else
	{
	  LPC_I2C2->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
	}
	LPC_I2C2->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x58:
	I2CSlaveBuffer[2][RdIndex2++] = LPC_I2C2->DAT;
	I2CMasterState[2] = I2C_OK;
	LPC_I2C2->CONSET = I2CONSET_STO;	/* Set Stop flag */ 
	LPC_I2C2->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
	break;

	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
	LPC_I2C2->CONSET = I2CONSET_STO;      /* Set Stop flag */
	I2CMasterState[2] = I2C_NACK_ON_ADDRESS;
	LPC_I2C2->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	I2CMasterState[2] = I2C_ARBITRATION_LOST;
	LPC_I2C2->CONCLR = I2CONCLR_SIC;	
	break;
  }
  return;
}

/*****************************************************************************
** Function name:		I2CStart
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error. 
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
uint32_t I2CStart( uint32_t portNum )
{
  uint32_t retVal = FALSE;
 
  timeout[portNum] = 0;
  /*--- Issue a start condition ---*/
  LPC_I2C[portNum]->CONSET = I2CONSET_STA;	/* Set Start flag */
    
  /*--- Wait until START transmitted ---*/
  while( 1 )
  {
	if ( I2CMasterState[portNum] == I2C_STARTED )
	{
	  retVal = TRUE;
	  break;	
	}
	if ( timeout[portNum] >= MAX_TIMEOUT )
	{
	  retVal = FALSE;
	  break;
	}
	timeout[portNum]++;
  }
  return( retVal );
}

/*****************************************************************************
** Function name:		I2CStop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
** 
*****************************************************************************/
uint32_t I2CStop( uint32_t portNum )
{
  LPC_I2C[portNum]->CONSET = I2CONSET_STO;      /* Set Stop flag */ 
  LPC_I2C[portNum]->CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
            
  /*--- Wait for STOP detected ---*/
  while( LPC_I2C[portNum]->CONSET & I2CONSET_STO );
  return TRUE;
}

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller as a master
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C0Init( void ) 
{
  LPC_SC->PCONP |= (1 << 7);

  /* set PIO0.27 and PIO0.28 to I2C0 SDA and SCL */
  /* function to 01 on both SDA and SCL. */
  LPC_PINCON->PINSEL1 &= ~((0x03<<22)|(0x03<<24));
  LPC_PINCON->PINSEL1 |= ((0x01<<22)|(0x01<<24));	
 
  /*--- Clear flags ---*/
  LPC_I2C0->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
#if FAST_MODE_PLUS
  LPC_PINCON->I2CPADCFG |= ((0x1<<0)|(0x1<<2));
  LPC_I2C0->SCLL   = I2SCLL_HS_SCLL;
  LPC_I2C0->SCLH   = I2SCLH_HS_SCLH;
#else
  LPC_PINCON->I2CPADCFG &= ~((0x1<<0)|(0x1<<2));
  LPC_I2C0->SCLL   = I2SCLL_SCLL;
  LPC_I2C0->SCLH   = I2SCLH_SCLH;
#endif

  /* Install interrupt handler */
  NVIC_EnableIRQ(I2C0_IRQn);

  LPC_I2C0->CONSET = I2CONSET_I2EN;
  return;
}

/*****************************************************************************
** Function name:		I2C1Init
**
** Descriptions:		Initialize I2C controller as a master
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C1Init( void ) 
{
  LPC_SC->PCONP |= (1 << 19);

#if 0
  /* set PIO0.0 and PIO0.1 to I2C1 SDA and SCL */
  /* function to 11 on both SDA and SCL. */
  LPC_PINCON->PINSEL0 &= ~((0x3<<0)|(0x3<<2));
  LPC_PINCON->PINSEL0 |= ((0x3<<0)|(0x3<<2));
  LPC_PINCON->PINMODE0 &= ~((0x3<<0)|(0x3<<2));
  LPC_PINCON->PINMODE0 |= ((0x2<<0)|(0x2<<2));		/* No pull-up no pull-down */
  LPC_PINCON->PINMODE_OD0 |= ((0x01<<0)|(0x1<<1));	/* Open drain */	
#endif
#if 1
  /* set PIO0.19 and PIO0.20 to I2C1 SDA and SCL */
  /* function to 11 on both SDA and SCL. */
  LPC_PINCON->PINSEL1 &= ~((0x3<<6)|(0x3<<8));
  LPC_PINCON->PINSEL1 |= ((0x3<<6)|(0x3<<8));
  LPC_PINCON->PINMODE1 &= ~((0x3<<6)|(0x3<<8));
  LPC_PINCON->PINMODE1 |= ((0x2<<6)|(0x2<<8));	/* No pull-up no pull-down */
  LPC_PINCON->PINMODE_OD0 |= ((0x1<<19)|(0x1<<20));	
#endif
 
  /*--- Clear flags ---*/
  LPC_I2C1->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
  LPC_I2C1->SCLL   = I2SCLL_SCLL;
  LPC_I2C1->SCLH   = I2SCLH_SCLH;

  /* Install interrupt handler */
  NVIC_EnableIRQ(I2C1_IRQn);

  LPC_I2C1->CONSET = I2CONSET_I2EN;
  return;
}

/*****************************************************************************
** Function name:		I2C2Init
**
** Descriptions:		Initialize I2C controller as a master
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C2Init( void ) 
{
  LPC_SC->PCONP |= (1 << 26);

  /* set PIO0.10 and PIO0.11 to I2C2 SDA and SCL */
  /* function to 10 on both SDA and SCL. */
  LPC_PINCON->PINSEL0 &= ~((0x03<<20)|(0x03<<22));
  LPC_PINCON->PINSEL0 |= ((0x02<<20)|(0x02<<22));
  LPC_PINCON->PINMODE0 &= ~((0x03<<20)|(0x03<<22));
  LPC_PINCON->PINMODE0 |= ((0x02<<20)|(0x2<<22));	/* No pull-up no pull-down */
  LPC_PINCON->PINMODE_OD0 |= ((0x01<<10)|(0x1<<11));	
 
  /*--- Clear flags ---*/
  LPC_I2C2->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
  LPC_I2C2->SCLL   = I2SCLL_SCLL;
  LPC_I2C2->SCLH   = I2SCLH_SCLH;

  /* Install interrupt handler */
  NVIC_EnableIRQ(I2C2_IRQn);

  LPC_I2C2->CONSET = I2CONSET_I2EN;
  return;
}

/*****************************************************************************
** Function name:		I2CEngine
**
** Descriptions:		The routine to complete a I2C transaction
**						from start to stop. All the intermitten
**						steps are handled in the interrupt handler.
**						Before this routine is called, the read
**						length, write length, I2C master buffer,
**						and I2C command fields need to be filled.
**						see i2cmst.c for more details. 
**
** parameters:			I2C port number
** Returned value:		master state of current I2C port.
** 
*****************************************************************************/
uint32_t I2CEngine( uint32_t portNum ) 
{
  /*--- Issue a start condition ---*/
  LPC_I2C[portNum]->CONSET = I2CONSET_STA;	/* Set Start flag */

  I2CMasterState[portNum] = I2C_BUSY;	

  while ( I2CMasterState[portNum] == I2C_BUSY )
  {
	if ( timeout[portNum] >= MAX_TIMEOUT )
	{
	  I2CMasterState[portNum] = I2C_TIME_OUT;
	  break;
	}
	timeout[portNum]++;
  }
  LPC_I2C[portNum]->CONCLR = I2CONCLR_STAC;

  return ( I2CMasterState[portNum] );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

