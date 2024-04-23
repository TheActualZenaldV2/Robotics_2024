#include <Arduino.h>
#include <Pixy2.h>
#include <ESP32Servo.h>

Pixy2 pixy; // Create our pixy object

int x = 90; // Default servo position
int y = 0;  // Default speed (stop)
Servo myServo_D2;

void setup() {
  Serial.begin(115200);
  pixy.init(); // Initialize the pixy object
  pixy.changeProg("line");

  pinMode(D3, OUTPUT); // digital 0 to 1
  pinMode(D4, OUTPUT); // PWM 0 to 255
  pinMode(D5, OUTPUT); // digital 0 to 1
  
  myServo_D2.attach(D6); // Attach servo to pin D6 (or adjust to your setup)
}

void loop() {
  pixy.line.getMainFeatures(); // Get main line features from Pixy

  if (pixy.line.numVectors > 0) {
    // Calculate servo angle based on the nearest vector's position
    int lineX = (pixy.line.vectors[0].m_x0 + pixy.line.vectors[0].m_x1) / 2;
    x = map(lineX, 0, 319, 20, 160);

    // Adjust speed based on distance to the nearest vector
    y = map(abs(lineX - 160), 0, 160, 40, 30); // Map distance to a speed range

    // Constrain y value to ensure it stays within the desired range
    y = constrain(y, 30, 40);
  } else {
    // No vectors detected, stop the car
    y = 0; // Set speed to stop
  }

  // Control the car's movement based on Pixy's line detection
  if (y < 0) {
    // Car is going backwards
    digitalWrite(D5, 0); // Set one direction
    digitalWrite(D3, 1); // Set other direction
    analogWrite(D4, abs(y)); // Set speed to positive value
  } else if (y > 0) {
    // Car is going forwards
    digitalWrite(D5, 1); // Set one direction
    digitalWrite(D3, 0); // Set other direction
    analogWrite(D4, y); // Set speed
  } else {
    // Car is not moving
    digitalWrite(D5, 1);
    digitalWrite(D3, 1); // brake
    analogWrite(D4, y); // Set speed
  }

  // Control the car's steering based on Pixy's line detection
  myServo_D2.write(x);

  delay(16); // Adjust according to your needs
}
