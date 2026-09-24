#include <Servo.h>

#include <elapsedMillis.h>

// Unity Data Driven Actuators
// A simple Arduino sketch to control actuators based on data from Unity
// Created by Byron Lahey on 9/23/2026
// Requirements: Arduino microcontroller, 1 Servo, 1 LED
// Recieves data from Unity via the serial port
// Parses the data
// Controls the actuators according to specified logic and mapping of data 

const int servoPin = 9;
const int ledPin = 13;
int servoValue = 0;
int ledValue = 0;

Servo myServo;

elapsedMillis readSerialDataTimer;
long readSerialDataInterval = 40;

elapsedMillis updateServoTimer;
long updateServoInterval = 40;

elapsedMillis updateLedTimer;
long updateLedInterval = 40;


void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for the serial port to be ready
  pinMode(servoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);

}

void loop() {
  parseData();
  positionServo();
  updateLED();

}

void parseData() {
  if (readSerialDataTimer > readSerialDataInterval) {
    readSerialDataTimer = 0;
    if (Serial.available() > 0) {
      String data = Serial.readString();
      int commaIndex = data.indexOf(',');
      if (commaIndex != -1) {
        servoValue = data.substring(0, commaIndex).toInt();
        ledValue = data.substring(commaIndex + 1).toInt();
      }
    }
  }
}

void positionServo() {
  if (updateServoTimer > updateServoInterval) {
    updateServoTimer = 0;
    int servoPosition = map(servoValue, 0, 1023, 0, 180);
    servoPosition = constrain(servoPosition, 0, 180);
    myServo.write(servoPosition);
  }
}

void updateLED() {
  if (updateLedTimer > updateLedInterval) {
    updateLedTimer = 0;
    digitalWrite(ledPin, ledValue);
  }
}
