#include <PRIZM.h>

#define line analogRead(A1)
#define line_left analogRead(A2)
#define line_right analogRead(A3)
PRIZM prizm;

void setup() 
{          
  prizm.PrizmBegin();
  Serial.begin(9600);
}

void loop() 
{
  int i;
  for(i=0;i<5;i++)
  {
  Serial.print("line=");
  Serial.println(line);
  Serial.print("line_left="); 
  Serial.println(line_left);
  Serial.print("line_right="); 
  Serial.println(line_right);
  }
  prizm.PrizmEnd();
}
