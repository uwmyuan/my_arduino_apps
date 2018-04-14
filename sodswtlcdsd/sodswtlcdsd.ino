//lcd
#include "U8glib.h"
//TH
#include <SHT1x.h>
#define dataPin  7
#define clockPin 6
//sd
#include <SPI.h>
#include <SD.h>
const int sdSelect = 4;
//TH
SHT1x sht1x(dataPin, clockPin);
//lcd
const int lcdSelect = 8;
U8GLIB_MINI12864 u8g(13, 11, lcdSelect, 10, 9); //SPI Com: SCK = 13, MOSI = 11, CS = 8, A0 = 10 ,RST=9;
//dust
const int measurePin = 0; //Connect dust sensor to Arduino A0 pin
const int ledPower = 5;   //Connect 3 led driver pins of dust sensor to Arduino D2
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity[10];

void setup(){
  Serial.begin(115200);
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
  digitalWrite(lcdSelect,LOW);
  pinMode(sdSelect,OUTPUT);
  SD.begin(sdSelect);
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
  for(i=0;i<10;i++)
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
  for(i=0;i<10;i++)
  {
    s+=dustDensity[i];
  }
  s=s/10*1000;
  //------sods---end
  //humidity temperature-----start
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  //temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
  //humidity temperature-----end
  //print serial
  Serial.print("PM2.5:");
  Serial.print(s);
  Serial.print("ug/m^3,");
  Serial.print("Temperature:");
  Serial.print(temp_c);
  Serial.print("C,");
  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.print("%");
  Serial.println();
  //sd
  digitalWrite(lcdSelect,HIGH);
  digitalWrite(sdSelect,LOW);
  File file=SD.open("dust.txt", FILE_WRITE);
  file.println(s);
  file.close();
  digitalWrite(sdSelect,HIGH);
  digitalWrite(lcdSelect,LOW);
  //print lcd
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
  } while( u8g.nextPage() );
  delay(1000);
}
