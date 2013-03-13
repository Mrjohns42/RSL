/******************************************************************
 *****                                                        *****
 *****  Name: easyweb.c                                       *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 07/05/2001                                      *****
 *****  Auth: Andreas Dannenberg                              *****
 *****        HTWK Leipzig                                    *****
 *****        university of applied sciences                  *****
 *****        Germany                                         *****
 *****  Func: implements a dynamic HTTP-server by using       *****
 *****        the easyWEB-API                                 *****
 *****                                                        *****
 ******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lpc17xx.h"
#include "type.h"

#define extern            // Keil: Line added for modular project management

#include "easyweb.h"

#include "EMAC.h"         // Keil: *.c -> *.h    // ethernet packet driver
#include "tcpip.h"        // Keil: *.c -> *.h    // easyWEB TCP/IP stack
#include "webpage.h"                             // webside for our HTTP server (HTML)



int main(void)
{ 
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  TCPLowLevelInit();

/*
  *(unsigned char *)RemoteIP = 24;               // uncomment those lines to get the
  *((unsigned char *)RemoteIP + 1) = 8;          // quote of the day from a real
  *((unsigned char *)RemoteIP + 2) = 69;         // internet server! (gateway must be
  *((unsigned char *)RemoteIP + 3) = 7;          // set to your LAN-router)

  TCPLocalPort = 2025;
  TCPRemotePort = TCP_PORT_QOTD;

  TCPActiveOpen();

  while (SocketStatus & SOCK_ACTIVE)             // read the quote from memory
  {                                              // by using the hardware-debugger
    DoNetworkStuff();
  }
*/

  HTTPStatus = 0;                   // clear HTTP-server's flag register
  TCPLocalPort = TCP_PORT_HTTP;     // set port we want to listen to

  while (1)                         // repeat forever
  {
    if (!(SocketStatus & SOCK_ACTIVE))
    { 
      TCPPassiveOpen();   // listen for incoming TCP-connection
    }
    DoNetworkStuff();     // handle network and easyWEB-stack
                          // events
    HTTPServer();
  }
}

// This function implements a very simple dynamic HTTP-server.
// It waits until connected, then sends a HTTP-header and the
// HTML-code stored in memory. Before sending, it replaces
// some special strings with dynamic values.
// NOTE: For strings crossing page boundaries, replacing will
// not work. In this case, simply add some extra lines
// (e.g. CR and LFs) to the HTML-code.

void HTTPServer(void)
{
  if (SocketStatus & SOCK_CONNECTED)             // check if somebody has connected to our TCP
  {
    if (SocketStatus & SOCK_DATA_AVAILABLE)      // check if remote TCP sent data
      TCPReleaseRxBuffer();                      // and throw it away

    if (SocketStatus & SOCK_TX_BUF_RELEASED)     // check if buffer is free for TX
    {
      if (!(HTTPStatus & HTTP_SEND_PAGE))        // init byte-counter and pointer to webside
      {                                          // if called the 1st time
        HTTPBytesToSend = sizeof(WebSide) - 1;   // get HTML length, ignore trailing zero
        PWebSide = (unsigned char *)WebSide;     // pointer to HTML-code
      }

      if (HTTPBytesToSend > MAX_TCP_TX_DATA_SIZE)     // transmit a segment of MAX_SIZE
      {
        if (!(HTTPStatus & HTTP_SEND_PAGE))           // 1st time, include HTTP-header
        {
          memcpy(TCP_TX_BUF, GetResponse, sizeof(GetResponse) - 1);
          memcpy(TCP_TX_BUF + sizeof(GetResponse) - 1, PWebSide, MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
          PWebSide += MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
        }
        else
        {
          memcpy(TCP_TX_BUF, PWebSide, MAX_TCP_TX_DATA_SIZE);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE;
          PWebSide += MAX_TCP_TX_DATA_SIZE;
        }

        TCPTxDataCount = MAX_TCP_TX_DATA_SIZE;   // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // xfer buffer
      }
      else if (HTTPBytesToSend)                  // transmit leftover bytes
      {
        memcpy(TCP_TX_BUF, PWebSide, HTTPBytesToSend);
        TCPTxDataCount = HTTPBytesToSend;        // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // send last segment
        TCPClose();                              // and close connection
        HTTPBytesToSend = 0;                     // all data sent
      }
      HTTPStatus |= HTTP_SEND_PAGE;              // ok, 1st loop executed
    }
  }
  else
    HTTPStatus &= ~HTTP_SEND_PAGE;               // reset help-flag if not connected
}

// samples and returns the AD-converter value of channel xx

unsigned int GetAD7Val(void)
{
// function replaced to handle LPC17xx A/D converter.
  unsigned int val;

  LPC_ADC->CR = 0x01203104;         // Setup A/D: 12-bit AIN2 @ 
  do {
    val = LPC_ADC->GDR;             // Read A/D Data Register
  } while ((val & 0x80000000) == 0);// Wait for end of A/D Conversion
  LPC_ADC->CR &= ~0x01000001;       // Stop A/D Conversion
  //val = (val >> 4) & 0x0FFF;      // Extract AIN2 Value
  val = (val >> 6) & 0x03FF;        // Extract only 10-bit AIN2 Value
  return(val / 10);                 // result of A/D process 
}

// samples and returns AD-converter value of channel xx

unsigned int GetTempVal(void)
{
// function replaced to handle LPC17xx A/D converter.
  unsigned int val;

  LPC_ADC->CR = 0x01203104;      // Setup A/D: 12-bit AIN2 @ 
  do {
    val = LPC_ADC->GDR;             // Read A/D Data Register
  } while ((val & 0x80000000) == 0);  // Wait for end of A/D Conversion
  LPC_ADC->CR &= ~0x01000002;       // Stop A/D Conversion
  val = (val >> 4) & 0x0FFF;        // Extract AIN2 Value
//  val = (val >> 6) & 0x03FF;          // Extract AIN2 Value
  return(val / 10);                   // result of A/D process 
}

// searches the TX-buffer for special strings and replaces them
// with dynamic values (AD-converter results)

void InsertDynamicValues(void)
{
  unsigned char *Key;
           char NewKey[5];
  unsigned int i;
  
  if (TCPTxDataCount < 4) return;                     // there can't be any special string
  
  Key = TCP_TX_BUF;

  for (i = 0; i < (TCPTxDataCount - 3); i++)
  {
    if (*Key == 'A')
     if (*(Key + 1) == 'D')
       if (*(Key + 3) == '%')
         switch (*(Key + 2))
         {
           case '7' :                                 // "AD7%"?
           {
             sprintf(NewKey, "%3u", GetAD7Val());     // insert AD converter value
             memcpy(Key, NewKey, 3);                  // channel 7 (P6.7)
             break;
           }
           case 'A' :                                 // "ADA%"?
           {
             sprintf(NewKey, "%3u", GetTempVal());    // insert AD converter value
             memcpy(Key, NewKey, 3);                  // channel 10 (temp.-diode)
             break;
           }
         }
    Key++;
  }
}

