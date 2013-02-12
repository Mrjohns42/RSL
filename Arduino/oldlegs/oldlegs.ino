#include <ax12.h>
#include <SoftwareSerial.h>


SoftwareSerial swSerial (2, 3); //Rx,Tx

void setup () {
  ax12Init(1000000);
  swSerial.begin (9600);
  
  writeData (254, AX_TORQUE_ENABLE, 1, 1); 
  for(int i=0; i < 4; i++)
  {
    int error = writeData(i, AX_GOAL_POSITION_L, 2, 512);
    swSerial.print ("goal position ID"); swSerial.println (status_id, DEC);
    swSerial.print ("int.error:"); swSerial.println (error, DEC);
    swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
    swSerial.println ("");
    delay(1500);      
  }
}

void loop () {
 
  for(int i=0;i<128;i++)
  {
    int error = writeData(254, AX_GOAL_POSITION_L, 2, 448 + i);
    swSerial.print ("goal position ID"); swSerial.println (status_id, DEC);
    swSerial.print ("int.error:"); swSerial.println (error, DEC);
    swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
    swSerial.println ("");
    delay(2000);    
  }  
  for(int i=128;i>0;i--)
  {
    int error = writeData(254, AX_GOAL_POSITION_L, 2, 448 + i);
    swSerial.print ("goal position ID"); swSerial.println (status_id, DEC);
    swSerial.print ("int.error:"); swSerial.println (error, DEC);
    swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
    swSerial.println ("");
    delay(2000); 
    
  }
  /*
  error = writeData(254, AX_GOAL_POSITION_L, 2, int (random (128)+448));
  delay(500);
  */
  
  
  /*
  for(int i=0; i < 4; i++)
  {
    error = writeData(i, AX_GOAL_POSITION_L, 2, 512);
    delay(1500);      
  }
   for(int i=3; i >=0; i--)
  {
    error = writeData(i, AX_GOAL_POSITION_L, 2, 512+128);
    delay(1500);      
  }
  */
  
  
  /*
  int error = ping (0);
  swSerial.print ("ping ID:"); swSerial.print (status_id, DEC);
  swSerial.print ("int.error:"); swSerial.print (error, DEC);
  swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
  swSerial.println ("");
  
  error = readData(254, AX_PRESENT_TEMPERATURE, 1);
  swSerial.print ("Temperature"); swSerial.print (status_data, DEC);
  swSerial.print ("int.error:"); swSerial.print (error, DEC);
  swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
  swSerial.println ("");
  
  error = writeData(254, AX_GOAL_POSITION_L, 2, int (random (256)+128));
  swSerial.print ("goal position ID"); swSerial.print (status_id, DEC);
  swSerial.print ("int.error:"); swSerial.print (error, DEC);
  swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
  swSerial.println ("");
  
  delay (200);
  
  error = readData(254, AX_PRESENT_POSITION_L, 2);
  swSerial.print ("position"); swSerial.print (status_data, DEC);
  swSerial.print ("int.error:"); swSerial.print (error, DEC);
  swSerial.print ("status.error:"); swSerial.println (status_error, DEC);
  swSerial.println ("");
  */

} 
