//testing linerar steering @60fps w pixy cam

#include <Arduino.h>
#include <Pixy2.h>
#include <WiFi.h>
#include <ESP32Servo.h>

Pixy2 pixy; // Create our pixy object

int x = 90; // Default servo position
int y = 0;  // Default speed (stop)

Servo myServo_D2;

void setup() {
  Serial.begin(115200);
  pixy.init(); // Initialize the pixy object
  
  WiFi.mode(WIFI_STA); // Set device as a Wi-Fi Station

  pinMode(D3, OUTPUT); // digital 0 to 1
  pinMode(D4, OUTPUT); // PWM 0 to 255
  pinMode(D5, OUTPUT); // digital 0 to 1

  myServo_D2.attach(D6); // Attach servo to pin D6 (or adjust to your setup)
}

void loop() {
  pixy.ccc.getBlocks(); // Get block data from Pixy

  if (pixy.ccc.numBlocks) {
    // Object detected, let's extract its location and width
    int objectX = pixy.ccc.blocks[0].m_x;
    int objectY = pixy.ccc.blocks[0].m_y;
    int objectWidth = pixy.ccc.blocks[0].m_width;

    // Map object's position to servo angle range (0 to 180 degrees)
    x = map(objectX, 0, 319, 0, 180);

    // Map object's width (distance) to speed range (30 to 100)
    y = map(objectWidth, 20, 320, 30, 100);

    // Constrain y value to ensure it stays within the desired range
    y = constrain(y, 30, 100);
  } else {
    // No object detected, stop the car
    y = 0; // Set speed to stop
  }

  // Control the car's movement based on Pixy's output
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

  // Control the car's steering based on Pixy's output
  myServo_D2.write(x);

  delay(16); // Adjust delay as needed for your application
}
