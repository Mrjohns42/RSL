//matt is lame 


#include <DynamixelSerial1.h>
#include <SoftwareSerial.h>

#define l_rotator 0
#define l_shoulder 1
#define l_elbow 2
#define l_wrist 3
#define r_rotator 4
#define r_shoulder 5
#define r_elbow 6
#define r_wrist 7
#define all 254

SoftwareSerial swSerial(10,11);

void setup(){
  Dynamixel.begin(1000000,2);  // Inicialize the servo at 1Mbps and Pin Control 2
  delay(1000);
  swSerial.begin(9600);
  delay(1000);
  
  Dynamixel.torqueStatus(all,1);
  
  for(int i=4; i < 8; i++)
  {
    int error = Dynamixel.move(i,512);
    swSerial.print("SERVO #");swSerial.println(i);
    swSerial.print("  Position: ");swSerial.println(512);
    if (error > 0)
    {
       swSerial.print("  Error: ");swSerial.println(error);
    }
    delay(1500);      
  }
  
}

void loop(){
   
  for(int i=0;i<128;i++)
  {
    int error = Dynamixel.move(all, 448 + i);
    swSerial.print("SERVO #");swSerial.println(254);
    swSerial.print("  Position: ");swSerial.println(448+i);
    if (error > 0)
    {
       swSerial.print("  Error: ");swSerial.println(error);
    }
    delay(20);    
  }  
  for(int i=128;i>0;i--)
  {
    int error = Dynamixel.move(all, 448 + i);
    swSerial.print("SERVO #");swSerial.println(254);
    swSerial.print("  Position: ");swSerial.println(448+i);
    if (error > 0)
    {
       swSerial.print("  Error: ");swSerial.println(error);
    }
    delay(20); 
    
  }
  
  
}
