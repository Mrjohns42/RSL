#include <ax12.h>
#include <SoftwareSerial.h>


SoftwareSerial swSerial (2, 3);

void setup () {
  ax12Init (1000000);
  swSerial.begin (9600);
  
  writeData (254, AX_TORQUE_ENABLE, 1, 1); // "14" is the engine ID

}

void loop () {
  
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

} 
