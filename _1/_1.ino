/*
www.openjumper.com
日期:2013.5.18
IDE 版本:1.0.1
功能：采集环境中的声音，光线，温度数据，并在一定时间内刷新数据，串口显示。
*/
#define R 3
#define G 5
#define B 6
int temPin = A4; //设置模拟口4 为LM35 的信号输入端口
int soundPin = A2;
int lightPin = A1;
int a[10];
float temperature = 0; //设置temperature 为浮点变量
long val=0; //设置val 为长整数变量
int soundvalue,lightvalue;
float readtemp() //读取温度
{
val=analogRead(temPin);//温度传感器LM35 接到模拟PIN4 上；val 变量为从LM35 信号
//口读取到的数值
temperature = (val*0.0048828125*100); //把读取到的val 转换为温度数值,系数一：
//0.00488125=5/1024,0~5V 对应模拟口读数1~1024,系数二：100=1000/10,1000 是毫伏与伏的
//转换；10 是每10 毫伏对应一度温升。
return temperature; //返回温度值
}
int readsound() //读取最大噪音值
{
int sound_max=0;
for(int i=0;i<10;i++)
{
a[i] = analogRead(soundPin);
sound_max=max(sound_max,a[i]);
}
return sound_max;
}
int readlight() //采样10 次光线值，返回其平均值
{
int light_sum,light_average;
int light_max = 0;
int light_min=1024;
for(int i=0;i<10;i++)
{
a[i] = analogRead(lightPin);
light_sum=light_sum + a[i];
light_max=max(light_max,a[i]);
light_min=min(light_min,a[i]);
}
light_average = (light_max + light_min)/2;
return light_average;
}
void setup()
{
Serial.begin(9600);//设置波特率
Serial.println("Smart Environment monitor begin....");
pinMode(R,OUTPUT);
pinMode(G,OUTPUT);
pinMode(B,OUTPUT); //将连接LED 灯的数字IO 口置为输出状态
}
void loop()
{
float tempvalue = 0;
tempvalue = readtemp();
soundvalue = readsound();
lightvalue = analogRead(lightPin);
float flag = 35-tempvalue;
if(flag<5)
{
analogWrite(R,flag/5*255);
analogWrite(G,255);
analogWrite(B,255);
}
else if(flag<10)
{
analogWrite(R,255);
analogWrite(G,flag/5*255);
analogWrite(B,255);
}
else if(flag<15)
{
analogWrite(R,255);
analogWrite(G,255);
analogWrite(B,flag/5*255);
}
Serial.print("now the temperature is ") ;
Serial.println(tempvalue);
Serial.print("the soundvalue is ") ;
Serial.println(soundvalue);
Serial.print("the lightvalue is ") ;
Serial.println(lightvalue);
Serial.println(); //串口分别显示读取的温度、声音、光敏值
delay(1000); //1 秒刷新一次
}
