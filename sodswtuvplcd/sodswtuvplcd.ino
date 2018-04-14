#include "U8glib.h"
#include <SHT1x.h>
#include <Wire.h>
#include <BMP085.h>
#define dataPin  7
#define clockPin 6
SHT1x sht1x(dataPin, clockPin);
U8GLIB_MINI12864 u8g(13, 11, 8, 10, 9); //SPI Com: SCK = 13, MOSI = 11, CS = 8, A0 = 10 ,RST=9;

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity[100];

BMP085 bmp;
void setup(){
  bmp.begin();
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
  //------sods---end
  //humidity temperature-----start
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
  //humidity temperature-----end
  //uv------start
  int sensorUV;
  int UV;
  sensorUV = analogRead(3);//connect UV sensors to Analog 0
  if(sensorUV<46)UV=0;
  else if(sensorUV>=46&&sensorUV<65)UV=1;
  else if(sensorUV>=65&&sensorUV<83)UV=2;
  else if(sensorUV>=83&&sensorUV<103)UV=3;
  else if(sensorUV>=103&&sensorUV<124)UV=4;
  else if(sensorUV>=124&&sensorUV<142)UV=5;
  else if(sensorUV>=124&&sensorUV<142)UV=5;
  else if(sensorUV>=142&&sensorUV<162)UV=6;
  else if(sensorUV>=162&&sensorUV<180)UV=7;
  else if(sensorUV>=180&&sensorUV<199)UV=8;
  else if(sensorUV>=199&&sensorUV<220)UV=9;
  else if(sensorUV>=220&&sensorUV<239)UV=10;
  else if(sensorUV>=239)UV=11;  
  //uv------end
  //bmp085----start
  float temp_p;
  float temp_a;
  temp_p=bmp.readPressure();
  temp_a=bmp.readAltitude();
  //bmp085----end
  u8g.firstPage();  
  do {
     // graphic commands to redraw the complete screen should be placed here  
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 15);
    u8g.print("PM2.5");
    u8g.setPrintPos(50, 15);
    u8g.print(s);
    u8g.setPrintPos(90, 15);
    u8g.print(" ug");
    u8g.setPrintPos(0, 30);
    u8g.print("T: ");
    u8g.setPrintPos(50, 30);
    u8g.print(temp_c);
    u8g.setPrintPos(90, 30);
    u8g.print(" C");
    u8g.setPrintPos(0, 45);
    u8g.print("H: ");
    u8g.setPrintPos(50, 45);
    u8g.print(humidity);
    u8g.setPrintPos(90, 45);
    u8g.print(" %");
    u8g.setPrintPos(0, 60);
    u8g.print("UV:");
    u8g.setPrintPos(25, 60);
    u8g.print(sensorUV);
    u8g.setPrintPos(50, 60);
    u8g.print("Pres");
    u8g.setPrintPos(90, 60);
    u8g.print(temp_p);
  } while( u8g.nextPage() );
  delay(1000);
}
