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

    // Adjust steering based on the object's position
    if (objectX < 100) {
      // Object is on the left side, steer left
      x = 60; // Adjust servo position for left turn
    } else if (objectX > 220) {
      // Object is on the right side, steer right
      x = 120; // Adjust servo position for right turn
    } else {
      // Object is in the center, go straight
      x = 90; // Set servo position for straight
    }

    // Adjust speed based on object width (distance)
    if (objectWidth > 100) {
      // Object is far, move forward
      y = 90; // Set maximum speed
    } else {
      // Object is close, slow down or stop
      y = 40; // Set reduced speed
    }
  } else {
    // No object detected, stop the car
    y = 0; // Set speed to st1  op
  }

  // Control the car's movement based on Pixy's output
  if (y < 0) {
    // Car is going backwards
    digitalWrite(D5, 1); // Set one direction
    digitalWrite(D3, 0); // Set other direction
    analogWrite(D4, abs(y)); // Set speed to positive value
  } else if (y > 0) {
    // Car is going forwards
    digitalWrite(D5, 0); // Set one direction
    digitalWrite(D3, 1); // Set other direction
    analogWrite(D4, y); // Set speed
  } else {
    // Car is not moving
    digitalWrite(D5, 0);
    digitalWrite(D3, 0); // Gliding
    analogWrite(D4, y); // Set speed
  }

  // Control the car's steering based on Pixy's output
  myServo_D2.write(x);

  delay(100); // Adjust delay as needed for your application
}
