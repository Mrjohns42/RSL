#ifndef SERIAL_H
#define SERIAL_H

void SER_init (void);
int sendchar (int c);
int getkey (void);
void configureGPIO(void);

#endif
