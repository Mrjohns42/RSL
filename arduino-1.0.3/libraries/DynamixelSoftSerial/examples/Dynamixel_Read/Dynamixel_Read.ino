#include <DynamixelSoftSerial.h>
#include <SoftwareSerial.h>
int Temperature,Voltage,Position; 

void setup(){
Serial.begin(9600);              // Begin Serial Comunication
Dynamixel.begin(9600,2,3,4);  // Inicialize the servo at 1Mbps and Pin Control 2
delay(1000);
}

void loop(){
  Temperature = Dynamixel.readTemperature(1); // Request and Print the Temperature
  Voltage = Dynamixel.readVoltage(1);         // Request and Print the Voltage
  Position = Dynamixel.readPosition(1);       // Request and Print the Position 
 
  Dynamixel.move(1,random(200,800));  // Move the Servo radomly from 200 to 800

  Serial.print(" *** Temperature: ");   // Print the variables in the Serial Monitor
  Serial.print(Temperature);
  Serial.print(" Celcius  Voltage: ");
  Serial.print(Voltage);
  Serial.print("  Volts   Position: ");
  Serial.print(Position);
  Serial.println(" of 1023 resolution");
  
delay(1000);

}
