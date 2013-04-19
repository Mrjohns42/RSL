
#ifndef _Kalman_h
#define _Kalman_h

#include "kalman.h"
void kalman_init(void);
float kalman_update(float newAngle, float newRate, float dt);

#endif
