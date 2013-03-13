/*****************************************************************************
 *   ssp.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __SSP_H__
#define __SSP_H__

#define SSP0_SLAVE		0		/* 1 is SLAVE mode, 0 is master mode */
#define SSP1_SLAVE		1		/* 1 is SLAVE mode, 0 is master mode */

/* SPI read and write buffer size */
#define SSP_BUFSIZE		16
#define FIFOSIZE		8

/* SSP CR0 register */
#define SSPCR0_DSS		(1 << 0)
#define SSPCR0_FRF		(1 << 4)
#define SSPCR0_SPO		(1 << 6)
#define SSPCR0_SPH		(1 << 7)
#define SSPCR0_SCR		(1 << 8)

/* SSP CR1 register */
#define SSPCR1_LBM		(1 << 0)
#define SSPCR1_SSE		(1 << 1)
#define SSPCR1_MS		(1 << 2)
#define SSPCR1_SOD		(1 << 3)

/* If RX_INTERRUPT is enabled, the SSP RX will be handled in the ISR
SSPReceive() will not be needed. */
extern void SSP0Init( void );
extern void SSP1Init( void );

#endif  /* __SSP_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

