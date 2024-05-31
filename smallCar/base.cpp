#include <Arduino.h>

// Define pin assignments
#define MOTOR1_PWM_PIN D1
#define MOTOR1_DIR_PIN D0
#define MOTOR2_PWM_PIN D2
#define MOTOR2_DIR_PIN D3

void setup() {
    // Initialize motor control pins as outputs
    pinMode(MOTOR1_PWM_PIN, OUTPUT); // PWM for Motor 1
    pinMode(MOTOR1_DIR_PIN, OUTPUT); // Direction for Motor 1
    pinMode(MOTOR2_PWM_PIN, OUTPUT); // PWM for Motor 2
    pinMode(MOTOR2_DIR_PIN, OUTPUT); // Direction for Motor 2
}

void loop() {
    // Motor 1 forward
    digitalWrite(MOTOR1_DIR_PIN, HIGH);  // Set direction forward for Motor 1
    analogWrite(MOTOR1_PWM_PIN, 255);    // Motor 1 maximum speed

    // Motor 2 forward
    digitalWrite(MOTOR2_DIR_PIN, HIGH);  // Set direction forward for Motor 2
    analogWrite(MOTOR2_PWM_PIN, 255);    // Motor 2 maximum speed

    delay(10000);  // Move forward for 10 seconds

    // Stop motors
    analogWrite(MOTOR2_PWM_PIN, 0);  // Stop Motor 2
    analogWrite(MOTOR1_PWM_PIN, 0);  // Stop Motor 1

    delay(1000);  // Wait for 1 second

    // Motor 1 backward
    digitalWrite(MOTOR1_DIR_PIN, LOW);  // Set direction backward for Motor 1
    analogWrite(MOTOR1_PWM_PIN, 255);   // Motor 1 maximum speed

    // Motor 2 backward
    digitalWrite(MOTOR2_DIR_PIN, LOW);  // Set direction backward for Motor 2
    analogWrite(MOTOR2_PWM_PIN, 255);   // Motor 2 maximum speed

    delay(10000);  // Move backward for 10 seconds

    // Stop motors
    analogWrite(MOTOR2_PWM_PIN, 0);  // Stop Motor 2
    analogWrite(MOTOR1_PWM_PIN, 0);  // Stop Motor 1

    delay(1000);  // Wait for 1 second
}
