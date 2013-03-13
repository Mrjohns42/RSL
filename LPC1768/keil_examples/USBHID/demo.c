/*----------------------------------------------------------------------------
 *      Name:    DEMO.C
 *      Purpose: USB HID Demo
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

#include "LPC17xx.H"                        /* LPC17xx definitions */

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "demo.h"

#if POWERDOWN_MODE_USB_WAKEUP
#include "timer.h"
extern volatile uint32_t SuspendFlag;
extern volatile uint32_t timer0_counter;
extern volatile uint32_t USBActivityInterruptFlag;
volatile uint32_t WakeupFlag = 0;
#endif

uint8_t InReport;                              /* HID Input Report    */
                                            /*   Bit0   : Buttons  */
                                            /*   Bit1..7: Reserved */

uint8_t OutReport;                             /* HID Out Report      */
                                            /*   Bit0..7: LEDs     */


/*
 *  Get HID Input Report -> InReport
 */

void GetInReport (void) {
  uint32_t kbd_val;

  kbd_val = (LPC_GPIO1->FIOPIN >> 20) & KBD_MASK;

  InReport = 0x00;
  if ((kbd_val & KBD_UP)     == 0) InReport |= 0x01;  /* up     pressed means 0 */
  if ((kbd_val & KBD_LEFT)   == 0) InReport |= 0x02;  /* left   pressed means 0 */
  if ((kbd_val & KBD_RIGHT)  == 0) InReport |= 0x04;  /* right  pressed means 0 */
  if ((kbd_val & KBD_SELECT) == 0) InReport |= 0x08;  /* select pressed means 0 */
  if ((kbd_val & KBD_DOWN)   == 0) InReport |= 0x10;  /* down   pressed means 0 */
}


/*
 *  Set HID Output Report <- OutReport
 */

void SetOutReport (void) {
  static unsigned long led_mask[] = { 1<<28, 1<<29, 1UL<<31, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6 };
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (OutReport & (1<<i)) {
      if (i < 3) LPC_GPIO1->FIOPIN |= led_mask[i];
      else       LPC_GPIO2->FIOPIN |= led_mask[i];
    } else {
      if (i < 3) LPC_GPIO1->FIOPIN &= ~led_mask[i];
      else       LPC_GPIO2->FIOPIN &= ~led_mask[i];
    }
  }
}

#if POWERDOWN_MODE_USB_WAKEUP
void TurnOffPLL0 (void)
{
  LPC_SC->PLL0CON   &= ~(0x1<<1);                 /* PLL0 Disabled               */
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;
  while ( (LPC_SC->PLL0STAT & (1<<25)) != 0x00 ); /* Wait for PLOCK0 disconnect  */

  LPC_SC->PLL0CON   &= ~(0x1<<0);                 /* PLL0 Disconnect             */
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;
  while ( (LPC_SC->PLL0STAT & (1<<24)) != 0x00 ); /* Wait for PLOCK0 shut down   */
  return;
}
#endif

/* Main Program */

int main (void) 
{
#if POWERDOWN_MODE_USB_WAKEUP
  uint32_t i, j;
#endif

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /* P1.28, P1.29, P1.31 is output (LED) */
  LPC_GPIO1->FIODIR   |= ((1UL<<28)|(1UL<<29)|(1UL<<31));  

  /* P2.2..6 is output (LED) */
  LPC_GPIO2->FIODIR   |= ((1UL<< 2)|(1UL<< 3)|
                      (1UL<< 4)|(1UL<< 5)|(1UL<< 6)); 

  /* P1.20, P1.23..26    is input  (Joystick) */
  LPC_GPIO1->FIODIR   &= ~((1UL<<20)|(1UL<<23)|
                       (1UL<<24)|(1UL<<25)|(1UL<<26)); 

  LPC_GPIO4->FIODIR   |=  (1UL<< 28);   /* Pin P2.28 is output (GLCD BAcklight)*/
  LPC_GPIO4->FIOPIN   &= ~(1UL<< 28);   /* Turn backlight off */

#if POWERDOWN_MODE_USB_WAKEUP
  init_timer(0, TIME_INTERVAL);		/* the timer is for USB suspend and resume */
#endif

  USB_Init();                           /* USB Initialization */
  USB_Connect(TRUE);                    /* USB Connect */

  while (1)                            /* Loop forever */
  {
#if POWERDOWN_MODE_USB_WAKEUP
	if ( USBActivityInterruptFlag )
	{
	  /* If the MCU is waked up from power down mode, the PLL needs to
	  be reconfigured, and USB block needs tn be reset and reconnect. */
	  USBActivityInterruptFlag = 0;
	  SystemInit();
	  USB_Init();                           /* USB Initialization */
	  USB_Connect(TRUE);                    /* USB Connect */
	  LPC_GPIO2->FIODIR = 0x000000FF;		/* P1.16..23 defined as Outputs */
	  LPC_GPIO2->FIOCLR = 0x000000FF;		/* turn off all the LEDs */
      /* Wake up, blink 20 times, and back to normal operation. 
	  If suspend and resume anagin, the same sequence will apply. */
	  for ( j = 0; j < 20; j++ )
	  {
		for ( i = 0; i < 0x200000; i++ );
		LPC_GPIO2->FIOSET = 0x000000FF;
		for ( i = 0; i < 0x200000; i++ );
		LPC_GPIO2->FIOCLR = 0x000000FF;
	  }
	}
	/* bit 8 on USN_INT_STAT is USB NeedClk bit. */
	if ( SuspendFlag == 1 )
	{
	  if ( timer0_counter > 200 )
	  {
		/* If it's a true suspend, turn off USB device bit the 
		USBNeed_clk will go away once USB device is disabled. */
		LPC_USB->USBClkCtrl = 0x00;
		while ( LPC_SC->USBIntSt & (1 << 8) );
		/* Regular USB interrupt is disabled to test USBActivity interrupt. 
		It will be enabled once it's waken up. */
		NVIC_DisableIRQ(USB_IRQn);

		/* See errata 3.3 on PLL0 disable and disconnect failure. */		
		TurnOffPLL0();
		 
		WakeupFlag = 1;
		/* USB Activity interrupt won't occur until Deepsleep bit is set in SCR.
		UM needs to be updated regarding this. */
		SCB->SCR |= 0x04;	/* Set SLEEPDEEP bit in SCR in Cortex M3 core. */	
		LPC_SC->PCON = 0x1;
		__WFI();
	  }
	}
#endif

  }
}
