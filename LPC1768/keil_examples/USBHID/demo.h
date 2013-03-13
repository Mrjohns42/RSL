/*----------------------------------------------------------------------------
 *      Name:    DEMO.H
 *      Purpose: USB HID Demo Definitions
 *      Version: V1.20
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

/* Push Button Definitions */
#define KBD_SELECT      0x01               
#define KBD_LEFT        0x08               
#define KBD_UP          0x10               
#define KBD_RIGHT       0x20               
#define KBD_DOWN        0x40
#define KBD_MASK        0x79  

/* LED Definitions */
#define LED_NUM     8               /* Number of user LEDs */

#define LED_MASK        0x000000FF  /* P1.28, P1.29, P1.31, P2.2..6 */
#define LED_0           0x00000001  /* P1.28 */
#define LED_1           0x00000002  /* P1.29 */
#define LED_2           0x00000004  /* P1.31 */
#define LED_3           0x00000008  /* P2.2 */
#define LED_4           0x00000010  /* P2.3 */
#define LED_5           0x00000020  /* P2.4 */
#define LED_6           0x00000040  /* P2.5 */
#define LED_7           0x00000080  /* P2.6 */

/* HID Demo Variables */
extern uint8_t InReport;
extern uint8_t OutReport;

/* HID Demo Functions */
extern void GetInReport  (void);
extern void SetOutReport (void);
