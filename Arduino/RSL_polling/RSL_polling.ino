#include <SoftwareSerial.h>
SoftwareSerial swSerial(10,11);

//== MPU ==//
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Kalman.h>

MPU6050 accelgyro;
Kalman kalmanX;
Kalman kalmanY;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t tempRaw;
uint32_t timer;
double accXangle;
double accYangle;
double temp;
double gyroXangle = 180; 
double gyroYangle = 180;
double compAngleX = 180;
double compAngleY = 180;
double kalAngleX;
double kalAngleY;

//== AX12 ==//
#include <DynamixelSerial1.h>

#define l_rotator 0
#define l_shoulder 1
#define l_elbow 2
#define l_wrist 3
#define r_rotator 4
#define r_shoulder 5
#define r_elbow 6
#define r_wrist 7
#define all 254



//== SETUP ==//
void setup()
{   //Initialize swSerial output for debug
    swSerial.begin(115200);
    delay(500);
    swSerial.println("\r\n\r\n====ROCKEM SOCKEM LIVE!====\r\n");
    
    //Setup AX-12a Servos
    Dynamixel.begin(1000000,2);
    delay(500);  
    swSerial.println("Testing AX-12 Servos:");
    Dynamixel.torqueStatus(all, 1);
    int errorstatus = 0;
    for(int i=4; i < 8; i++)
    {
      int error = Dynamixel.move(i,512);
      swSerial.print("  SERVO ");swSerial.print(i);
      swSerial.print(" | Position: ");swSerial.print(512);
      if (error > 0)
      {
         swSerial.print(" |  Error: ");swSerial.print(error);
         errorstatus++;
      }
      swSerial.println("");
      delay(20);     
    }
    swSerial.print("  Summary: ");
    swSerial.print(errorstatus);swSerial.println(" SERVO ERRORS\r\n");
   
    //Setup MPU
    Wire.begin();
    swSerial.println("Testing MPU-6050:");
    accelgyro.initialize();
    swSerial.println(accelgyro.testConnection() ? "  MPU6050 connection successful" : "  MPU6050 connection failed");
    kalmanX.setAngle(0); // Set starting angle
    kalmanY.setAngle(0);
    
    //Begin after user input
    swSerial.println("\r\nREADY: Send any character to begin demo.");
    while (swSerial.available() && swSerial.read()); // empty buffer
    while (!swSerial.available());                 // wait for data
    while (swSerial.available() && swSerial.read()); // empty buffer again
    
    timer = micros();
}


//== LOOP ==//
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
    

    
    accYangle = (atan2(ax,az)+PI)*RAD_TO_DEG;
    accXangle = (atan2(ay,az)+PI)*RAD_TO_DEG;   
    
    double gyroXrate = (double)gx/131.0;
    double gyroYrate = -((double)gy/131.0);
    
    /*No Filter*/
    //gyroXangle += gyroXrate*((double)(micros()-timer)/1000000);
    //gyroYangle += gyroYrate*((double)(micros()-timer)/1000000);
    
    /*Unbiased Filter*/
    //gyroXangle += kalmanX.getRate()*((double)(micros()-timer)/1000000);
    //gyroYangle += kalmanY.getRate()*((double)(micros()-timer)/1000000);    
    
    /*Complimentary Filter*/
    //compAngleX = (0.93*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.07*accXangle); 
    //compAngleY = (0.93*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.07*accYangle);  
    
    /*Kalman Filter (offset by 180)*/
    kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000)-180; 
    kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000)-180;
    timer = micros();
    
    //temp = ((double)tempRaw + 12412.0) / 340.0;
    
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
    
    swSerial.print(kalAngleX);swSerial.print("\t\t");
    swSerial.print(kalAngleY);swSerial.print("\t\t");
    
    //swSerial.print(temp);swSerial.print("\t");
     
    swSerial.print("\r\n");
    
    delay(1); // The accelerometer's maximum samples rate is 1kHz
    
    //ADC
    float pitch = kalAngleX + 150;
    if (pitch > 300) pitch = 300;
    if (pitch < 0) pitch = 0;
    
    pitch = pitch/300 * 1023;
    
    
    int error = Dynamixel.move(6, (int)pitch);
    if (error > 0)
    {
      swSerial.print("  Error: ");swSerial.println(error);
    }
    delay(20);

}

