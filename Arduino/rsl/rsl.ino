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
   swSerial.begin(115200);
   delay(500);
   swSerial.println("\r\n\r\n====ROCKEM SOCKEM LIVE!====\r\n");
   
   //Setup Servos
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

   //ready
   /*
   swSerial.println("READY: Send any character to begin demo.");
   while (swSerial.available() && swSerial.read()); // empty buffer
   while (!swSerial.available());                 // wait for data
   while (swSerial.available() && swSerial.read()); // empty buffer again
  */
}


//== LOOP ==//
void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady)
    {
      swSerial.println("DMP not ready.");
      return;  
    }
    

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        swSerial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            swSerial.print("ypr\t");
            swSerial.print(ypr[0] * 180/M_PI);
            swSerial.print("\t");
            swSerial.print(ypr[1] * 180/M_PI);
            swSerial.print("\t");
            swSerial.println(ypr[2] * 180/M_PI);
        #endif
        
        int pitch = (int) ypr[1];
        if(pitch > 90) pitch = 90;
        if(pitch < 0) pitch = 0;
        swSerial.println(512-pitch);
        int error = Dynamixel.move(5, 512 - pitch);
        if (error > 0)
        {
          swSerial.print("  Error: ");swSerial.println(error);
        }
        delay(20);

       


        
    }
}
