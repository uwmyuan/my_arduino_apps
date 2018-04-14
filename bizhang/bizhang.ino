int SNUM[3] ;//定义3个传感器，从SNUM[0]-SNUM[2]为从左到右3个红外避障传感器
int INA = 4; //电机A正反转控制端
int PWMA = 5; //电机A调速端
int INB = 7; //电机B正反转控制端
int PWMB = 6; //电机B调速端
#include <Servo.h> //包含舵机库
Servo myservo; //创建一个舵机控制类
// 设定SR04 连接的Arduino 引脚
const int TrigPin = 2;
const int EchoPin = 3;
float distance[3];
void motospd(int sp1,int sp2) 
//电机速度控制函数。括号内分别为左右电机速度值，
//范围-255～+255，正值为正转，负值为反转。
{                        
  if(sp1>0)
    digitalWrite(INA, HIGH);    
  else
     digitalWrite(INA, LOW); 
  if(sp2>0)
     digitalWrite(INB, HIGH); 
  else
     digitalWrite(INB, LOW); 
  analogWrite(PWMA,abs (sp1)); 
  analogWrite(PWMB,abs (sp2));
}
void setup(){
pinMode(11, INPUT); //配置左传感器IO口为输入
pinMode(12, INPUT); //配置中传感器IO口为输入
pinMode(13, INPUT); //配置右传感器IO口为输入
Serial.begin(9600); //打开串口，初始化
pinMode(INA,OUTPUT);
pinMode(INB,OUTPUT);   //配置电机驱动IO口为输出
myservo.attach(9); //连接舵机到9号数字口
pinMode(TrigPin, OUTPUT);// 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
pinMode(EchoPin, INPUT);
}
void loop(){
 SNUM[0] = digitalRead(11); //左传感器赋值
 SNUM[1] = digitalRead(12); //中传感器赋值
 SNUM[2] = digitalRead(13); //右传感器赋值
 myservo.write(110); //舵机转动到相应角度
 delay(500);
 // 产生一个10us 的高脉冲去触发TrigPin
 digitalWrite(TrigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(TrigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(TrigPin, LOW);
 // 检测脉冲宽度，并计算出距离
 distance[0] = pulseIn(EchoPin, HIGH) / 58.00;
 Serial.print("distance forward ");
 Serial.print(distance[0]);
 Serial.println();
 if(distance[0]>20)
 {
   motospd(150,150);//直行
   delay(1000);
 }
 else
 {
  motospd(0,0);//stop
  myservo.write(160); //舵机转动到相应角度
  delay(500);
  // 产生一个10us 的高脉冲去触发TrigPin
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // 检测脉冲宽度，并计算出距离
  distance[1] = pulseIn(EchoPin, HIGH) / 58.00;
  Serial.print("distance right ");
  Serial.print(distance[1]);
  Serial.println();
  myservo.write(60); //舵机转动到相应角度
  delay(500);
  // 产生一个10us 的高脉冲去触发TrigPin
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // 检测脉冲宽度，并计算出距离
  distance[2] = pulseIn(EchoPin, HIGH) / 58.00;
  Serial.print("distance left ");
  Serial.print(distance[2]);
  Serial.println();
  if(distance[1]<20&&distance[2]<20)
  {
    motospd(-100,-100);//后退
    delay(1000);
  }
  if(distance[2]>=distance[1])
  {
    motospd(-100,100);//左转
    delay(1000);
  }
  else
  {
      motospd(100,-100);//右转
      delay(1000);
   }
 }
}

