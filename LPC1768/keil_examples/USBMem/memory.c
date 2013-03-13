/*----------------------------------------------------------------------------
 * Name:    memory.c
 * Purpose: USB Memory Storage Demo
 * Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC family microcontroller devices only. Nothing 
 *      else gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "mscuser.h"
#include "memory.h"

extern uint8_t Memory[MSC_MemorySize];         /* MSC Memory in RAM */

/* Main Program */
int main (void) 
{
  uint32_t n;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  for (n = 0; n < MSC_ImageSize; n++) {     /* Copy Initial Disk Image */
    Memory[n] = DiskImage[n];               /*   from Flash to RAM     */
  }

  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1);                                /* Loop forever */
}
