#include <SoftwareSerial.h>
SoftwareSerial swSerial(10,11);

// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-10-07 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Kalman.h>

Kalman kalmanX;
Kalman kalmanY;

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t tempRaw;

double accXangle; // Angle calculate using the accelerometer
double accYangle;
double temp;
double gyroXangle = 180; // Angle calculate using the gyro
double gyroYangle = 180;
double compAngleX = 180; // Calculate the angle using a Kalman filter
double compAngleY = 180;
double kalAngleX; // Calculate the angle using a Kalman filter
double kalAngleY;

uint32_t timer;

#define LED_PIN 13
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    swSerial.begin(115200);

    // initialize device
    swSerial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    swSerial.println("Testing device connections...");
    swSerial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    kalmanX.setAngle(180); // Set starting angle
    kalmanY.setAngle(180);
    timer = micros();
    
    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
    
    // display tab-separated accel/gyro x/y/z values
    /*
    swSerial.print("a/g:\t");
    swSerial.print(ax); swSerial.print("\t");
    swSerial.print(ay); swSerial.print("\t");
    swSerial.print(az); swSerial.print("\t");
    swSerial.print(gx); swSerial.print("\t");
    swSerial.print(gy); swSerial.print("\t");
    swSerial.println(gz);
    */
    
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    accYangle = (atan2(ax,az)+PI)*RAD_TO_DEG;
    accXangle = (atan2(ay,az)+PI)*RAD_TO_DEG;   
    
    double gyroXrate = (double)gx/131.0;
    double gyroYrate = -((double)gy/131.0);
    gyroXangle += gyroXrate*((double)(micros()-timer)/1000000); // Calculate gyro angle without any filter  
    gyroYangle += gyroYrate*((double)(micros()-timer)/1000000);
    //gyroXangle += kalmanX.getRate()*((double)(micros()-timer)/1000000); // Calculate gyro angle using the unbiased rate
    //gyroYangle += kalmanY.getRate()*((double)(micros()-timer)/1000000);
    
    compAngleX = (0.93*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.07*accXangle); // Calculate the angle using a Complimentary filter
    compAngleY = (0.93*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.07*accYangle);  
    
    kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000); // Calculate the angle using a Kalman filter
    kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000);
    timer = micros();
    
    temp = ((double)tempRaw + 12412.0) / 340.0;
    
    /* Print Data */   
    /*
    swSerial.print(accX);swSerial.print("\t");
    swSerial.print(accY);swSerial.print("\t");  
    swSerial.print(accZ);swSerial.print("\t");    
    
    swSerial.print(gyroX);swSerial.print("\t");  
    swSerial.print(gyroY); swSerial.print("\t");   
    swSerial.print(gyroZ);swSerial.print("\t");  
    
    swSerial.print(accXangle);swSerial.print("\t");
    swSerial.print(accYangle);swSerial.print("\t"); 
      
    swSerial.print(gyroXangle);swSerial.print("\t");
    swSerial.print(gyroYangle);swSerial.print("\t");
    
    swSerial.print(compAngleX);swSerial.print("\t");
    swSerial.print(compAngleY); swSerial.print("\t");
    */
    
    swSerial.print(kalAngleX);swSerial.print("\t");
    swSerial.print(kalAngleY);swSerial.print("\t");
    
    //swSerial.print(temp);swSerial.print("\t");
     
    swSerial.print("\r\n");
    
    delay(1); // The accelerometer's maximum samples rate is 1kHz
}

