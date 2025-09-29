#include <Arduino.h>

#define RightLine A2
#define LeftLine A4

const int LeftPWM = 10;
const int LeftDir = 12;
const int RightPWM = 11;
const int RightDir = 13;

void Forward(int speed){
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed);
}

void Backward(int speed){
  digitalWrite(LeftDir, HIGH);
  digitalWrite(RightDir, HIGH);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed);
}

void Left(int speed){
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed);
  analogWrite(LeftPWM, speed/4);
}

void Right(int speed){
  digitalWrite(LeftDir, LOW);
  digitalWrite(RightDir, LOW);
  analogWrite(RightPWM, speed/4);
  analogWrite(LeftPWM, speed);
}

void setup() {
  pinMode(LeftDir, OUTPUT);
  pinMode(LeftPWM, OUTPUT);
  pinMode(RightDir, OUTPUT);
  pinMode(RightPWM, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  Forward(255);
  if(analogRead(RightLine)<100){
    Backward(150);
    delay(350);
    Left(150);
    delay(500);
  }
  if(analogRead(LeftLine)<100){
    Backward(150);
    delay(350);
    Right(150);
    delay(500);
  }
}