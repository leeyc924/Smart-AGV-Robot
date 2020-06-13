#include <PRIZM.h>
PRIZM prizm;
void setup() 
{
  prizm.PrizmBegin();
  Serial.begin(9600);
}

void loop() 
{
  int x;
  int i;
  x =prizm.readBatteryVoltage();
  for(i=0;i<4;i++)
  {Serial.print("x=");Serial.println(x);} 
  prizm.PrizmEnd();
}
