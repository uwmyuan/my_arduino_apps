/*
www.openjumper.com
日期:2013.5.18
IDE 版本:1.0.1
功能：旋转电位器则舵机转动，当电位器转动一圈是，舵机则相应的转动最大的180 度，
实现实时控制舵机的转动。
*/
#include <Servo.h> //包含舵机库
Servo myservo; //创建一个舵机控制类
int pos = 0; //定义一个变量存储舵机转动角度
// 设定SR04 连接的Arduino 引脚
const int TrigPin = 2;
const int EchoPin = 3;
float distance;
void setup()
{
myservo.attach(9); //连接舵机到9 号数字口
// 初始化串口通信及连接SR04 的引脚
Serial.begin(9600);
pinMode(TrigPin, OUTPUT);
// 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
pinMode(EchoPin, INPUT);
Serial.println("Ultrasonic sensor:");
}
void loop()
{
int value=analogRead(A0); //读取A0 口输入的模拟值
pos=map(value,0,1023,0,180); //将读到的模拟值0~1023 映射为0°~180°
myservo.write(pos); //舵机转动到相应角度
delay(100); //延时一段时间让舵机转动到对应位置
// 产生一个10us 的高脉冲去触发TrigPin
digitalWrite(TrigPin, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin, LOW);
// 检测脉冲宽度，并计算出距离
distance = pulseIn(EchoPin, HIGH) / 58.00;
Serial.print(distance);
Serial.print("cm");
Serial.println();
delay(1000);
}
