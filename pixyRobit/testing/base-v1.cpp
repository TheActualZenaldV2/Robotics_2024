#include <Arduino.h>

// Define motor control pins
const int leftDirectionPin = D3;
const int leftSpeedPin = D4;
const int rightDirectionPin = D0;
const int rightSpeedPin = D1;
const int enablePin = D2;

// Define motor directions
const int forward = 1;
const int backward = 0;

void setup() {
    pinMode(leftDirectionPin, OUTPUT);
    pinMode(leftSpeedPin, OUTPUT);
    pinMode(rightDirectionPin, OUTPUT);
    pinMode(rightSpeedPin, OUTPUT);
    pinMode(enablePin, OUTPUT);

    // Initially, set motors to stop
    digitalWrite(enablePin, LOW);
}

void loop() {
    // Move forward
    moveMotors(forward, forward, 150);
    delay(3000);

    // Move backward
    moveMotors(backward, backward, 150);
    delay(3000);

    // Turn left
    moveMotors(backward, forward, 150);
    delay(3000);

    // Turn right
    moveMotors(forward, backward, 150);
    delay(3000);
}

// Function to control both motors
void moveMotors(int leftDirection, int rightDirection, int speed) {
    digitalWrite(leftDirectionPin, leftDirection);
    digitalWrite(rightDirectionPin, rightDirection);
    analogWrite(leftSpeedPin, speed);
    analogWrite(rightSpeedPin, speed);
}
