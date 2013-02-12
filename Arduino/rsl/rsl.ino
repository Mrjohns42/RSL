#include <SoftwareSerial.h>
SoftwareSerial swSerial(10,11);

//== MPU ==//
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define OUTPUT_READABLE_YAWPITCHROLL

MPU6050 mpu;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

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
{
   swSerial.begin(9600);
   delay(500);
   swSerial.println("\r\n\r\n====ROCKEM SOCKEM LIVE!====\r\n");
   
   //Setup MPU
   Wire.begin();
   delay(500);
   swSerial.println("Testing MPU6050:");
   mpu.initialize();
   bool mpuconnect = mpu.testConnection();
   swSerial.println(mpu.testConnection() ? F("  MPU6050 connection successful") : F("  MPU6050 connection failed"));
   devStatus = mpu.dmpInitialize();
   if(!devStatus)
   {    swSerial.println("  Enabling DMP...");
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        swSerial.println("    Enabling interrupt detection (Arduino external interrupt 0)...");
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        swSerial.println("    DMP ready! Waiting for first interrupt...");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        swSerial.print("  DMP Initialization failed (code ");
        swSerial.print(devStatus);
        swSerial.println(")");
    }

   //Setup Servos
   Dynamixel.begin(1000000,2);
   delay(500);  
   swSerial.println("Testing AX-12 Servos:");
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
     delay(1500);     
   }
   swSerial.print("  Summary: ");
   swSerial.print(errorstatus);swSerial.println(" SERVO ERRORS\r\n");
   
   
    
  
  
}

void loop(){}
