/*----------------------------------------------------------------------------
 * Name:    memory.h
 * Purpose: USB Memory Storage Demo Definitions
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

#define LED_NUM     8               /* Number of user LEDs */

#define LED_MSK         0x000000FF  /* P1.28, P1.29, P1.31, P2.2..6 */
#define LED_RD          0x00000001  /* LED0 */
#define LED_WR          0x00000002  /* LED1 */
#define LED_CFG         0x00000004  /* LED2 */
#define LED_SUSP        0x00000008  /* LED3 */

extern void LED_On  (unsigned int value);
extern void LED_Off (unsigned int value);

/* MSC Disk Image Definitions */
#define MSC_ImageSize   0x00001000

extern const unsigned char DiskImage[MSC_ImageSize];   /* Disk Image */
