#include <Arduino.h>

#define RightLine A2
#define LeftLine A4

#define start A0

const int LeftPWM = 10;
const int LeftDir = 12;
const int RightPWM = 11;
const int RightDir = 13;

float pos;
float Kp = 50;
int baseSpeed = 200;

//const int dip1 = 5;
const int dip2 = 6;
const int dip3 = 7;

void Forward(float speed)
{
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed - (35/100.0*speed));
}

void Backward(int speed)
{
  digitalWrite(LeftDir, HIGH);
  digitalWrite(RightDir, HIGH);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed);
}

void Left(int speed)
{
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed / 4);
}

void Right(int speed)
{
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed / 4);
  analogWrite(LeftPWM, speed);
}

void stop()
{
  analogWrite(RightPWM, 0);
  analogWrite(LeftPWM, 0);
}

void enemyPos()
{
  int s1 = digitalRead(0);
  int s2 = digitalRead(1);
  int s3 = digitalRead(2);
  int s4 = digitalRead(4);
  int s5 = digitalRead(A5);

  pos = (s1 * 1 + s2 * 2 + s3 * 3 + s4 * 4 + s5 * 5) / 5.0;
}

void ringLim()
{
  if (analogRead(RightLine) < 100)
  {
    Backward(255);
    delay(350);
    Left(150);
    delay(500);
  }
  else if (analogRead(LeftLine) < 100)
  {
    Backward(255);
    delay(350);
    Right(150);
    delay(500);
  }
  else if (analogRead(LeftLine) < 100 && analogRead(RightLine) < 100)
  {
    Backward(255);
    delay(550);
    Left(255);
    delay(500);
  }
}

void attack()
{
  float error = pos - 3.0;
  float correction = Kp * error;
  int leftSpeed = constrain(baseSpeed - correction, 0, 255);
  int rightSpeed = constrain(baseSpeed + correction, 0, 255);

  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(LeftPWM, leftSpeed);
  analogWrite(RightPWM, rightSpeed);
}

void setup()
{
  pinMode(LeftDir, OUTPUT);
  pinMode(LeftPWM, OUTPUT);
  pinMode(RightDir, OUTPUT);
  pinMode(RightPWM, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(A5, INPUT);
  //pinMode(dip1, INPUT);
  pinMode(dip2, INPUT);
  pinMode(dip3, INPUT);
  Serial.begin(9600);
}

void offensive(){
  while(digitalRead(start) == HIGH)
  {
    ringLim();
    enemyPos();
    if (pos == 0)
      Forward(baseSpeed);
    else
      attack();
  }
    stop();
}

void loop()
{
 while(digitalRead(dip2)==HIGH){
   offensive();
 }
 Forward(255);
 ringLim();
}
