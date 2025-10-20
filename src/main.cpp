#include <Arduino.h>

#define RightLine A2
#define LeftLine A4

#define start A0

const int LeftPWM = 10;
const int LeftDir = 12;
const int RightPWM = 11;
const int RightDir = 13;

const int s1 = 0;
const int s2 = 1;
const int s3 = 2;
const int s4 = 4;
#define s5 A5

float pos;
float Kp1 = 50;
int baseSpeed = 200;

float Kp2 = 40;
float Ki = 0.3;
float Kd = 20;

float prevError = 0;
float integral = 0;

// const int dip1 = 5;
const int dip2 = 6;
const int dip3 = 7;

void Forward(float speed)
{
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed - (35 / 100.0 * speed));
}

void Backward(int speed)
{
  digitalWrite(LeftDir, HIGH);
  digitalWrite(RightDir, HIGH);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed - (35 / 100.0 * speed));
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
  digitalWrite(LeftDir, HIGH);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed / 4);
  analogWrite(LeftPWM, speed);
}

void turnLeft(float speedDR, float speedSTG)
{
  digitalWrite(RightDir, LOW);
  digitalWrite(LeftDir, HIGH);
  analogWrite(RightPWM, speedDR);
  analogWrite(LeftPWM, speedSTG);
}

void turnRight(float speedDR, float speedSTG)
{
  digitalWrite(RightDir, HIGH);
  digitalWrite(LeftDir, LOW);
  analogWrite(RightPWM, speedDR);
  analogWrite(LeftPWM, speedSTG);
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
    turnLeft(150, 150);
    delay(100);
  }
  else if (analogRead(LeftLine) < 100)
  {
    Backward(255);
    delay(350);
    turnRight(150, 150);
    delay(100);
  }
  else if (analogRead(LeftLine) < 100 && analogRead(RightLine) < 100)
  {
    Backward(255);
    delay(550);
    turnLeft(150, 150);
    delay(200);
  }
}

void attack()
{
  float error = pos - 3.0;
  float correction = Kp1 * error;
  int leftSpeed = constrain(baseSpeed - correction, 0, 255);
  int rightSpeed = constrain(baseSpeed + correction, 0, 255);

  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(LeftPWM, leftSpeed);
  analogWrite(RightPWM, rightSpeed);
}

void atrack2 (){
    float error = pos - 3.0;
  integral += error;
  integral = constrain(integral, -50, 50);
  float derivative = error - prevError;

  float correction = Kp2 * error + Ki * integral + Kd * derivative;

  int leftSpeed = constrain(baseSpeed - correction, 0, 255);
  int rightSpeed = constrain(baseSpeed + correction, 0, 255);

  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(LeftPWM, leftSpeed);
  analogWrite(RightPWM, rightSpeed);

  prevError = error;
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
  pinMode(dip2, INPUT);
  pinMode(dip3, INPUT);
  Serial.begin(9600);
}

void offensive()
{
  ringLim();
  enemyPos();
  if (pos == 0)
    Forward(baseSpeed);
  else
  {

    if (digitalRead(s1) == HIGH && digitalRead(s2) == LOW && digitalRead(s3) == LOW && digitalRead(s4) == LOW && digitalRead(s5) == LOW)
    {
      while (digitalRead(s3) == LOW)
        turnLeft(255, 255);
    }
    else if (digitalRead(s1) == LOW && digitalRead(s2) == LOW && digitalRead(s3) == LOW && digitalRead(s4) == LOW && digitalRead(s5) == HIGH)

      while (digitalRead(s3) == LOW)
        turnRight(255, 255);
    else
      attack();
  }
}

void offensive2(){
  ringLim();
  enemyPos();

  if (pos == 0)
  {
    turnLeft(150, 150);
  }
  else
  {
    if (digitalRead(s1) == HIGH && digitalRead(s2) == LOW && digitalRead(s3) == LOW && digitalRead(s4) == LOW && digitalRead(s5) == LOW)
    {
      while (digitalRead(s3) == LOW)
        turnLeft(255, 255);
    }
    else if (digitalRead(s1) == LOW && digitalRead(s2) == LOW && digitalRead(s3) == LOW && digitalRead(s4) == LOW && digitalRead(s5) == HIGH)
      while (digitalRead(s3) == LOW)
        turnRight(255, 255);
    else
      attack();
  }
}

void loop()
{
  while (digitalRead(start) == HIGH)
  {
    if (digitalRead(dip2) == HIGH)
      offensive();
    if (digitalRead(dip3)==HIGH)
      offensive2();
  }
  stop();
}