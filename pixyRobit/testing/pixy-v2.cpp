#include <Arduino.h>
#include <Pixy2.h>

Pixy2 pixy;

const int leftDirectionPin = D3;
const int leftSpeedPin = D4;
const int rightDirectionPin = D0;
const int rightSpeedPin = D1;
const int enablePin = D2;

void setup() {
    pinMode(leftDirectionPin, OUTPUT);
    pinMode(leftSpeedPin, OUTPUT);
    pinMode(rightDirectionPin, OUTPUT);
    pinMode(rightSpeedPin, OUTPUT);
    pinMode(enablePin, OUTPUT);

    digitalWrite(enablePin, LOW);

    Serial.begin(115200);
    pixy.init();
}

void loop() {
    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks) {
        int centerX = pixy.ccc.blocks[0].m_x; // x location of the main object
        int objectWidth = pixy.ccc.blocks[0].m_width;

        // Calculate how far the object is from the center of the camera's field of view
        int distanceFromCenter = centerX - (pixy.frameWidth / 2);

        // Map the distance to a range of -100 to 100 (left to right)
        int mappedValue = map(distanceFromCenter, -pixy.frameWidth / 2, pixy.frameWidth / 2, -100, 100);

        // Adjust motor speeds based on the mapped value
        int leftSpeed = 150 - mappedValue;
        int rightSpeed = 150 + mappedValue;

        // Ensure motor speeds are within valid range (0-255)
        leftSpeed = constrain(leftSpeed, 0, 255);
        rightSpeed = constrain(rightSpeed, 0, 255);

        // Move the robot
        moveMotors(leftSpeed, rightSpeed);

        Serial.print("Center X: ");
        Serial.print(centerX);
        Serial.print(", Object Width: ");
        Serial.print(objectWidth);
        Serial.print(", Distance from Center: ");
        Serial.println(distanceFromCenter);
    } else {
        // No blocks seen by camera, stop the robot
        moveMotors(0, 0);
    }

    delay(100); // Adjust as needed
}

void moveMotors(int leftSpeed, int rightSpeed) {
    // Determine direction based on speed sign
    int leftDirection = (leftSpeed >= 0) ? 1 : 0; // 1 for forward, 0 for backward
    int rightDirection = (rightSpeed >= 0) ? 1 : 0;

    // Set motor directions
    digitalWrite(leftDirectionPin, leftDirection);
    digitalWrite(rightDirectionPin, rightDirection);

    // Set motor speeds
    analogWrite(leftSpeedPin, abs(leftSpeed));
    analogWrite(rightSpeedPin, abs(rightSpeed));
}
