#include <SoftwareSerial.h>
#include <DynamixelSoftSerial.h>


void setup()
{
  Serial.begin(9600);
  Dynamixel.begin(115200,0,1);  //baud, Rx, Tx, D_pin
  delay(1000);
  for(int i=0; i < 4; i++)
  {
    int error = Dynamixel.torqueStatus(i,ON);
   // Serial.print("Servo ");Serial.print(i);Serial.print(": ERROR ");Serial.println(error); 
  }
}



void loop()
{
  


}


