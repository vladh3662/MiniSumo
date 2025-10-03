#include <Arduino.h>

#define RightLine A2
#define LeftLine  A4
#define start     A0

const int LeftPWM  = 10;
const int LeftDir  = 12;
const int RightPWM = 11;
const int RightDir = 13;

float pos;         // poziția adversarului
float Kp = 50;     // câștig proporțional
int baseSpeed = 200;

void Forward(int speed)
{
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed);
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

// calculează poziția adversarului ca medie ponderată
void enemyPos() {
  int s1 = digitalRead(0);
  int s2 = digitalRead(1);
  int s3 = digitalRead(2);
  int s4 = digitalRead(4);
  int s5 = digitalRead(A5);

  int sumSensors = s1+s2+s3+s4+s5;
  if (sumSensors > 0) {
    pos = (s1*1 + s2*2 + s3*3 + s4*4 + s5*5) / (float)sumSensors;
  } else {
    pos = 0;  // nu se vede adversarul
  }
}

// protecția ringului
void ringLim(){
   if (analogRead(RightLine) < 100)
    {
      Backward(150);
      delay(350);
      Left(150);
      delay(500);
    }
    else if (analogRead(LeftLine) < 100)
    {
      Backward(150);
      delay(350);
      Right(150);
      delay(500);
    }
    else if (analogRead(LeftLine) < 100 && analogRead(RightLine) < 100)
    {
      Backward(150);
      delay(550);
      Left(255);
      delay(500);
    }
}

void setup()
{
  pinMode(LeftDir, OUTPUT);
  pinMode(LeftPWM, OUTPUT);
  pinMode(RightDir, OUTPUT);
  pinMode(RightPWM, OUTPUT);
  pinMode(start, INPUT);

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(A5, INPUT);
}

void loop()
{
  if (digitalRead(start) == HIGH)
  {
    ringLim();       // verificare margine
    enemyPos();      // calculează poziția adversarului

    if (pos == 0) {
      // nu vede adversarul → căutare
      Left(baseSpeed);
    } else {
      // control P: corectează viteza roților
      float error = pos - 3.0;  // centru = 3
      float correction = Kp * error;

      int leftSpeed  = constrain(baseSpeed - correction, 0, 255);
      int rightSpeed = constrain(baseSpeed + correction, 0, 255);

      digitalWrite(LeftDir, LOW);
      digitalWrite(RightDir, LOW);
      analogWrite(LeftPWM, leftSpeed);
      analogWrite(RightPWM, rightSpeed);
    }
  }
  else
  {
    stop();
  }
}
