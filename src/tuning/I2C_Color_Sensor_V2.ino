#include <PRIZM.h>
#include "Adafruit_TCS34725.h"

PRIZM prizm;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() 
{
  prizm.PrizmBegin();
  Serial.begin(9600);
}

void loop() 
{
  int art_color=0;
    uint16_t clear, red, green, blue;
    tcs.getRawData(&red, &green, &blue, &clear);
    int i =0;
    for(i=0;i<3;i++)
    {
    if(red > green & red < blue ) // 빨강색 = 1
    {art_color =1;}
    else if(blue > red & blue < green ) // 초록색 = 2
    {art_color =2;}
    else if(green > red & green < blue) // 퍼렁색 =3
    {art_color =3;}
    else
    {art_color = 1;}
    }
  Serial.print("R: "); Serial.print(red, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(green, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(blue, DEC); Serial.print(" ");
  Serial.print("art_color: "); Serial.print(art_color, DEC);
  Serial.println(" ");
}
