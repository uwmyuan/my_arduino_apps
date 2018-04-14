/*
 Standalone Sketch to use with a Arduino UNO and a
 Sharp Optical Dust Sensor GP2Y1010AU0F
*/
#include "U8glib.h"
U8GLIB_MINI12864 u8g(13, 11, 8, 10, 9); //SPI Com: SCK = 13, MOSI = 11, CS = 8, A0 = 10 ,RST=9;

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity[100];
  
void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
   if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
float testDensity()
{
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin); // read the dust value
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  return calcVoltage;
}
void loop(){
  int i=0;
  for(i=0;i<100;i++)
{
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity[i] = 0.17 * testDensity() - 0.1;
  delay(1000);
}  

  
  //Serial.print("Raw Signal Value (0-1023): ");
  //Serial.print(voMeasured);
  
  //Serial.print(" - Voltage: ");
  //Serial.print(calcVoltage);
  
  //Serial.print(" - Dust Density: ");
  //Serial.println(dustDensity); // unit: mg/m3
  float s=0;
  for(i=0;i<100;i++)
  {
    s+=dustDensity[i];
  }
  s=s/100*1000;
  u8g.firstPage();  
  do {
     // graphic commands to redraw the complete screen should be placed here  
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 15);
    u8g.print(s);   
    u8g.setPrintPos(50, 15);
    u8g.print("ug/m^3");  
  } while( u8g.nextPage() );
  delay(1000);
}
