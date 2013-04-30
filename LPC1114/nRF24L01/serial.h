#ifndef SERIAL_H
#define SERIAL_H

#define _100KHZ 0x00
#define _1MHZ 0x01

void SER_init (void);
int sendchar (int c);
int getkey (void);
void configureGPIO(uint8_t baud);

#endif
