/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbuser.c
 * Purpose: USB Custom User Module
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

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"

#include "mscuser.h"
#include "memory.h"

#if USB_DMA
extern uint8_t BulkBuf[MSC_MAX_PACKET]; /* Bulk In/Out Buffer */
#endif

/*
 *  USB Power Event Callback
 *   Called automatically on USB Power Event
 *    Parameter:       power: On(TRUE)/Off(FALSE)
 */

#if USB_POWER_EVENT
void USB_Power_Event (uint32_t  power) {
}
#endif


/*
 *  USB Reset Event Callback
 *   Called automatically on USB Reset Event
 */

#if USB_RESET_EVENT
void USB_Reset_Event (void) {
  USB_ResetCore();
}
#endif


/*
 *  USB Suspend Event Callback
 *   Called automatically on USB Suspend Event
 */

#if USB_SUSPEND_EVENT
void USB_Suspend_Event (void) {
}
#endif


/*
 *  USB Resume Event Callback
 *   Called automatically on USB Resume Event
 */

#if USB_RESUME_EVENT
void USB_Resume_Event (void) {
}
#endif


/*
 *  USB Remote Wakeup Event Callback
 *   Called automatically on USB Remote Wakeup Event
 */

#if USB_WAKEUP_EVENT
void USB_WakeUp_Event (void) {
}
#endif


/*
 *  USB Start of Frame Event Callback
 *   Called automatically on USB Start of Frame Event
 */

#if USB_SOF_EVENT
void USB_SOF_Event (void) {
}
#endif


/*
 *  USB Error Event Callback
 *   Called automatically on USB Error Event
 *    Parameter:       error: Error Code
 */

#if USB_ERROR_EVENT
void USB_Error_Event (uint32_t error) {
}
#endif


/*
 *  USB Set Configuration Event Callback
 *   Called automatically on USB Set Configuration Request
 */

#if USB_CONFIGURE_EVENT
void USB_Configure_Event (void) {

  if (USB_Configuration) {                  /* Check if USB is configured */
    /* add your code here */
  }
}
#endif


/*
 *  USB Set Interface Event Callback
 *   Called automatically on USB Set Interface Request
 */

#if USB_INTERFACE_EVENT
void USB_Interface_Event (void) {
}
#endif


/*
 *  USB Set/Clear Feature Event Callback
 *   Called automatically on USB Set/Clear Feature Request
 */

#if USB_FEATURE_EVENT
void USB_Feature_Event (void) {
}
#endif


#define P_EP(n) ((USB_EP_EVENT & (1 << (n))) ? USB_EndPoint##n : NULL)

/* USB Endpoint Events Callback Pointers */
void (* const USB_P_EP[16]) (uint32_t event) = {
  P_EP(0),
  P_EP(1),
  P_EP(2),
  P_EP(3),
  P_EP(4),
  P_EP(5),
  P_EP(6),
  P_EP(7),
  P_EP(8),
  P_EP(9),
  P_EP(10),
  P_EP(11),
  P_EP(12),
  P_EP(13),
  P_EP(14),
  P_EP(15),
};


/*
 *  USB Endpoint 1 Event Callback
 *   Called automatically on USB Endpoint 1 Event
 *    Parameter:       event
 */

void USB_EndPoint1 (uint32_t event) {
}


/*
 *  USB Endpoint 2 Event Callback
 *   Called automatically on USB Endpoint 2 Event
 *    Parameter:       event
 */

void USB_EndPoint2 (uint32_t event) {
#if USB_DMA
  USB_DMA_DESCRIPTOR DD;
#endif

  switch (event) {
    case USB_EVT_OUT:
      MSC_BulkOut();
      break;
    case USB_EVT_IN:
      MSC_BulkIn();
      break;

#if USB_DMA
    case USB_EVT_OUT_DMA_EOT: 
      MSC_BulkOut();

//      USB_DMA_Enable(MSC_EP_OUT);   /* 11.08.09  USB_DMA_Enable is not necessary at this point */
      break;

    case USB_EVT_OUT_DMA_NDR:                    // new DMA Descriptor request
      DD.BufAdr  = (uint32_t)BulkBuf;            // DMA Buffer Address
      DD.BufLen  = MSC_MAX_PACKET;               // DMA Buffer Length in bytes
      DD.MaxSize = MSC_MAX_PACKET;               // Maximum packet size
      DD.Cfg.Val = 0;                            // Initial DMA Configuration
      DD.Cfg.Type.IsoEP = 0;                     // Non-Iso Endpoint

      USB_DMA_Setup (MSC_EP_OUT, &DD);           // Setup DMA
      USB_DMA_Enable(MSC_EP_OUT);                // Enable DMA
      break;


    case USB_EVT_IN_DMA_NDR:                     // new DMA Descriptor request
      MSC_BulkIn();
      break;
    case USB_EVT_IN_DMA_EOT:                     // End of Transfer request 
//      MSC_BulkIn();                 /* 11.08.09 MSC_BulkIn must only be called upon a DMA_NDR interrupt */
      break;
#endif
  }
}


/*
 *  USB Endpoint 3 Event Callback
 *   Called automatically on USB Endpoint 3 Event
 *    Parameter:       event
 */

void USB_EndPoint3 (uint32_t event) {
}


/*
 *  USB Endpoint 4 Event Callback
 *   Called automatically on USB Endpoint 4 Event
 *    Parameter:       event
 */

void USB_EndPoint4 (uint32_t event) {
}


/*
 *  USB Endpoint 5 Event Callback
 *   Called automatically on USB Endpoint 5 Event
 *    Parameter:       event
 */

void USB_EndPoint5 (uint32_t event) {
}


/*
 *  USB Endpoint 6 Event Callback
 *   Called automatically on USB Endpoint 6 Event
 *    Parameter:       event
 */

void USB_EndPoint6 (uint32_t event) {
}


/*
 *  USB Endpoint 7 Event Callback
 *   Called automatically on USB Endpoint 7 Event
 *    Parameter:       event
 */

void USB_EndPoint7 (uint32_t event) {
}


/*
 *  USB Endpoint 8 Event Callback
 *   Called automatically on USB Endpoint 8 Event
 *    Parameter:       event
 */

void USB_EndPoint8 (uint32_t event) {
}


/*
 *  USB Endpoint 9 Event Callback
 *   Called automatically on USB Endpoint 9 Event
 *    Parameter:       event
 */

void USB_EndPoint9 (uint32_t event) {
}


/*
 *  USB Endpoint 10 Event Callback
 *   Called automatically on USB Endpoint 10 Event
 *    Parameter:       event
 */

void USB_EndPoint10 (uint32_t event) {
}


/*
 *  USB Endpoint 11 Event Callback
 *   Called automatically on USB Endpoint 11 Event
 *    Parameter:       event
 */

void USB_EndPoint11 (uint32_t event) {
}


/*
 *  USB Endpoint 12 Event Callback
 *   Called automatically on USB Endpoint 12 Event
 *    Parameter:       event
 */

void USB_EndPoint12 (uint32_t event) {
}


/*
 *  USB Endpoint 13 Event Callback
 *   Called automatically on USB Endpoint 13 Event
 *    Parameter:       event
 */

void USB_EndPoint13 (uint32_t event) {
}


/*
 *  USB Endpoint 14 Event Callback
 *   Called automatically on USB Endpoint 14 Event
 *    Parameter:       event
 */

void USB_EndPoint14 (uint32_t event) {
}


/*
 *  USB Endpoint 15 Event Callback
 *   Called automatically on USB Endpoint 15 Event
 *    Parameter:       event
 */

void USB_EndPoint15 (uint32_t event) {
}
