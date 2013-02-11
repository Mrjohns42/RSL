/*
  ax12.cpp - arbotiX Library for AX-12 Servos
  Copyright (c) 2008,2009 Michael E. Ferguson.  All right reserved.
  Modificada el 15/11/09 por Pablo Gindel.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

//#include "wiring.h" // we need this for the serial port defines
#include "ax12.h"
#include <avr/interrupt.h>
#include <arduino.h>

/******************************************************************************
 * Hardware Serial Level, this uses the same stuff as Serial, therefore 
 *  you should not use the Arduino Serial library.
 ******************************************************************************/

byte ax_rx_buffer[AX12_BUFFER_SIZE];    //buffer de recepción
int status_id;
int status_error;
int status_data;

// making these volatile keeps the compiler from optimizing loops of available()
volatile byte ax_rx_Pointer;                       

/** helper functions to emulate half-duplex */
void setTX(){
    bitClear(UCSR0B, RXCIE0);    // deshabilita la interrupción de recepción 
    bitClear(UCSR0B, RXEN0);     // deshabilila la recepción
    bitSet(UCSR0B, TXEN0);       // habilita la trasmisión
}
void setRX(){
    bitClear(UCSR0B, TXEN0);
    bitSet(UCSR0B, RXEN0);
    bitSet(UCSR0B, RXCIE0); 
    ax_rx_Pointer = 0;         // resetea el puntero del buffer
}

/** Sends a character out the serial port */
byte ax12writeB(byte data){
    while (bit_is_clear(UCSR0A, UDRE0));    // espera que el micro esté pronto para trasmitir
    UDR0 = data;                            // escribe el byte a trasmitir
    return data; 
}

/** We have a one-way recieve buffer, which is reset after each packet is receieved.
    A wrap-around buffer does not appear to be fast enough to catch all bytes at 1Mbps. */
ISR(USART_RX_vect){  
    ax_rx_buffer[(ax_rx_Pointer++)] = UDR0;    // esta es la rutina de interrupción de recepción
}                                              // lo que hace es meter el byte recibido en el buffer

/** initializes serial0 transmit at baud, 8-N-1 */
void ax12Init(long baud){
    UBRR0H = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8;
    UBRR0L = ((F_CPU / 16 + baud / 2) / baud - 1);        // setea la velocidad del USART
    ax_rx_Pointer = 0;                                    
    // enable rx
    setRX();    
}

/******************************************************************************
 * Packet Level
 ******************************************************************************/

/** send instruction packet */
void ax12SendPacket (byte id, byte datalength, byte instruction, byte *data){
    byte checksum = 0;
    setTX();    
    ax12writeB(0xFF);
    ax12writeB(0xFF);
    checksum += ax12writeB(id);
    checksum += ax12writeB(datalength + 2);
    checksum += ax12writeB(instruction);
    for (byte f=0; f<datalength; f++) {     // data = parámetros
      checksum += ax12writeB(data[f]);
    }
    // checksum = 
    ax12writeB(~checksum);
    setRX();
}

/** read status packet 
/** retorna el código interno de error; 0 = OK */
byte ax12ReadPacket(){
    unsigned long ulCounter;
    byte timeout, error, status_length, checksum, offset;
    byte volatile bcount; 
    // primero espera que llegue toda la data
    offset = 0; timeout = 0; bcount = 0;
    while(bcount < 11){        // 8 es el largo máximo que puede tener un packet
        ulCounter = 0;
        while((bcount + offset) == ax_rx_Pointer){
            if(ulCounter++ > 1000L){  // was 3000
                timeout = 1;
                break;
            }
        }
        if (timeout) break;
        if ((bcount == 0) && (ax_rx_buffer[offset] != 0xff)) offset++;
        else bcount++;
    }
    // ahora decodifica el packet
    // corrección de cabecera
    error = 0;                                             // código interno de error
    do {
        error++;
        offset++;
        bcount--;
    } while (ax_rx_buffer[offset] == 255);          
    if (error > 1) error =0;                               // prueba de cabecera
    // offset = primer byte del mensaje (sin cabecera)
    // bcount = largo del mensaje leido (sin cabecera)
    status_length = 2 + ax_rx_buffer[offset+1];            // largo del mensaje decodificado 
    if (bcount != status_length) error+=2;                 // prueba de coherencia de data
    checksum = 0;                                          // cálculo de checksum
    for (byte f=0; f<status_length; f++) 
        checksum += ax_rx_buffer[offset+f];
    if (checksum != 255) error+=4;                          // prueba de checksum
    if (error == 0) {
        status_id = ax_rx_buffer[offset];
        status_error = ax_rx_buffer[offset+2];
        switch (status_length) {
            case 5: status_data = ax_rx_buffer[offset+3]; break;   
            case 6: status_data = ax_rx_buffer[offset+3] + (ax_rx_buffer[offset+4]<<8); break;
            default: status_data = 0;
        }
    } else {
        status_id = -1;
        status_error = -1;
        status_data = -1; 
    }
    return error;
}

/******************************************************************************
 * Instruction Level
 ******************************************************************************/

/** ping */
byte ping (byte id) {
     byte *data;
     ax12SendPacket (id, 0, AX_PING, data); 
     return ax12ReadPacket(); 
}

/** reset */
byte reset (byte id) {
     byte *data;
     ax12SendPacket (id, 0, AX_RESET, data); 
     return ax12ReadPacket(); 
}

/** action */
byte action (byte id) {
     byte *data;
     ax12SendPacket (id, 0, AX_ACTION, data); 
     return ax12ReadPacket(); 
}

/** read data */
byte readData (byte id, byte regstart, byte reglength) {
    byte data [2];
    data [0] = regstart; data [1] = reglength;
    ax12SendPacket (id, 2, AX_READ_DATA, data);
    return ax12ReadPacket();
}

/** write data */
byte writeData (byte id, byte regstart, byte reglength, int value) {
    byte data [reglength+1];
    data [0] = regstart; data [1] = value&0xFF;
    if (reglength > 1) {data[2] = (value&0xFF00)>>8;}
    ax12SendPacket (id, reglength+1, AX_WRITE_DATA, data);
	return ax12ReadPacket();
}

/** reg write */
byte regWrite (byte id, byte regstart, byte reglength, int value) {
    byte data [reglength+1];
    data [0] = regstart; data [1] = value&0xFF;
    if (reglength > 1) {data[2] = (value&0xFF00)>>8;}
    ax12SendPacket (id, reglength+1, AX_REG_WRITE, data);
    return ax12ReadPacket();
}

/* falta implementar:
                     SYNC WRITE
                     decodificador de error
                     alto nivel
*/
