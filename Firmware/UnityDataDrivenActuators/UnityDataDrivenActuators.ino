#include <Servo.h>

#include <elapsedMillis.h>

// Unity Data Driven Actuators
// A simple Arduino sketch to control actuators based on data from Unity
// Created by Byron Lahey on 9/23/2026
// Requirements: Arduino microcontroller, 1 Servo, 1 LED
// Recieves data from Unity via the serial port
// Parses the data
// Controls the actuators according to specified logic and mapping of data 
// Data will be sent from Unity in the format of three comma-separated positions (x, y, z) and a binary value (0 or 1) for the light. 
// The servo will be controlled by the x position data.
// The LED will be controlled by the binary value.

const int servoPin = 9;
const int ledPin = 8;
int positionValue[3];
int lightValue = 0;
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
      // Read the full data message from Unity
      // Save the postion and Light data to a string
      // Use the x position to set the servo value
      // Use the light status to set the LED value

      String data = Serial.readString();
      positionValue[0] = data.substring(0, data.indexOf(',')).toInt();
      positionValue[1] = data.substring(data.indexOf(',') + 1, data.lastIndexOf(',')).toInt();
      positionValue[2] = data.substring(data.lastIndexOf(',') + 1).toInt();
      lightValue = data.substring(data.lastIndexOf(',') + 1).toInt();
      servoValue = positionValue[0];
      ledValue = lightValue;
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
