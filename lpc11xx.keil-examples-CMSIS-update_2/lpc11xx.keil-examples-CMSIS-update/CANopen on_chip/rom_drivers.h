/****************************************************************************
 *   $Id:: rom_drivers.h 5035 2010-09-24 23:48:55Z nxp12832             $
 *   Project: NXP LPC11xx CAN example
 *
 *   Description:
 *     This file is part of the CAN and CANopen on-chip driver examples.
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
#ifndef ROM_DRIVERS_H_
#define ROM_DRIVERS_H_

#define CAN	0
#define USB	1

#define PERIPHERAL		CAN

#if (PERIPHERAL == CAN)
	#include "rom_driver_CAN.h"
#endif /* (PERIPHERAL == CAN) */

#if (PERIPHERAL == USB)
	#include "usb.h" 
	typedef	struct _USB {
	  void (*init_clk_pins)(void);
	  void (*isr)(void);
	  void (*init)( USB_DEV_INFO * DevInfoPtr ); 
	  void (*connect)(BOOL  con);
	}USB;
#endif /* (PERIPHERAL == USB) */

typedef	struct _ROM {
   const unsigned p_usbd;
   const unsigned p_clib;
#if (PERIPHERAL == CAN)
   const CAND *pCAND;
#else
   const unsigned pCAND;
#endif /* (PERIPHERAL == CAN) */
   const unsigned p_pwrd;
   const unsigned p_dev1;
   const unsigned p_dev2;
   const unsigned p_dev3;
   const unsigned p_dev4; 
}  ROM;

#endif /* ROM_DRIVERS_H_ */
