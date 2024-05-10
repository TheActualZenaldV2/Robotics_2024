#include <Arduino.h>

void setup() {
    pinMode(D5, OUTPUT);   // PWM for Motor 1
    pinMode(D4, OUTPUT);   // Direction for Motor 1
    pinMode(D3, OUTPUT);   // PWM for Motor 2
    pinMode(D2, OUTPUT);   // Direction for Motor 2
}
void loop() {
    // Motor 1 forward
    digitalWrite(D4, HIGH);    // set direction forward for Motor 1
    analogWrite(D5, 55);       // Motor 1 maximum speed

    // Motor 2 forward
    digitalWrite(D2, HIGH);    // set direction forward for Motor 2
    analogWrite(D3, 55);       // Motor 2 maximum speed

    delay(10000);
    //stop motors
        analogWrite(D3, 0);       // Motor 2 maximum speed
    analogWrite(D5, 0);       // Motor 1 maximum speed
delay(1000);

    // Motor 1 backward
    digitalWrite(D4, LOW);     // set direction backward for Motor 1
    analogWrite(D5, 55);       // Motor 1 maximum speed

    // Motor 2 backward
    digitalWrite(D2, LOW);     // set direction backward for Motor 2
    analogWrite(D3, 55);       // Motor 2 maximum speed

    delay(10000);

   
}
