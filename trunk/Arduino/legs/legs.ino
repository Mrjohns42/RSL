#include <SoftwareSerial.h>
SoftwareSerial rightleg(10,11); //software serial port for right leg
int rightDir = 12; //pin for serial comm direction
byte command[10];

void write_data(byte id, byte reg, byte * vals, byte n = 1)
{
  digitalWrite(rightDir, HIGH);
  unsigned int checksum = 0;
  rightleg.write(0xFF); //2 start bytes
  rightleg.write(0xFF);
  rightleg.write(id);  //servo id
  checksum += id;
  rightleg.write(n + 3); //length
  checksum += (n+3);
  rightleg.write(reg);  //starting register
  checksum += reg;
  int i;
  for(i=0; i < n; i++) //params
  {
     rightleg.write(vals[i]);
     checksum += vals[i];
  }
  byte chksm = (byte)((~checksum) % 256);
  rightleg.write(chksm);  
  digitalWrite(rightDir,LOW);
}  


void setup()
{ 
  Serial.begin(9600); //for serial debugging monitor
  delay(1000);  
  rightleg.begin(115200);
  pinMode(rightDir, OUTPUT); //1 for TX, 0 for RX  
  Serial.println("Begin Broadcast");
  
  byte baud[] = {0xCF};
  write_data(0xFE,0x04,baud,1);
  byte params[] = {0x01,0x01};
  write_data(0xFE,0x18,params,2);
  byte vals[] = {0x00,0x02};
  write_data(0xFE,0x1E,vals,2);
  
}

void loop()
{  

  
}



  
