#include <Arduino.h>
#include <Pixy2.h>
#include <WiFi.h>
#include <ESP32Servo.h>

Pixy2 pixy; // Create our pixy object

int x = 90; // Default servo position
int y = 0;  // Default speed (stop)
int emergencyStop = false;
Servo myServo_D2;

// Note: VCC on RangeFinder must go to +5V on Portenta
//         GND on rangefinder goes to GND on Portenta 
int myTriggerPin = A0;  // Trigger on RangeFinder
int myEchoPin = A1;     // Echo on Rangefinder  
unsigned long myDuration;

void setup() {
  Serial.begin(115200);
  pixy.init(); // Initialize the pixy object
  
  WiFi.mode(WIFI_STA); // Set device as a Wi-Fi Station

  pinMode(D3, OUTPUT); // digital 0 to 1
  pinMode(D4, OUTPUT); // PWM 0 to 255
  pinMode(D5, OUTPUT); // digital 0 to 1
  pinMode(myTriggerPin, OUTPUT);
  pinMode(myEchoPin, INPUT_PULLDOWN);
  
  myServo_D2.attach(D6); // Attach servo to pin D6 (or adjust to your setup)
}

void loop() {
  digitalWrite(myTriggerPin, LOW);
  delayMicroseconds(10); 
  digitalWrite(myTriggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(myTriggerPin, LOW);
  myDuration = pulseIn(myEchoPin, HIGH, 40000UL);
  
  if (myDuration > 1000){ // raw data from 200 to 16000                                          
    //digitalWrite(LEDB, LOW);    // LEDB Blue LED on if far 
    emergencyStop = false;
  } else { 
    //digitalWrite(LEDB, HIGH);   // LEDB Blue LED off if close or nothing
    emergencyStop = true;
  }  

  if (!emergencyStop) {
    pixy.line.getAllFeatures(); // Get line features from Pixy
    
    if (pixy.line.numFeatures) { // If lines are detected
      int lineX = pixy.line.vectors[0].m_x1 + (pixy.line.vectors[0].m_x2 - pixy.line.vectors[0].m_x1) / 2; // Calculate the X position of the line
      
      // Map line's position to servo angle range (0 to 180 degrees)
      x = map(lineX, 0, 319, 20, 160);
  
      // Adjust speed based on distance to the line (you might need to tweak these values)
      int distanceToLine = abs(lineX - 160); // Distance from the line to the center of the frame
      y = map(distanceToLine, 0, 160, 40, 30); // Map distance to a speed range
  
      // Constrain y value to ensure it stays within the desired range
      y = constrain(y, 30, 40);
    } else {
      // No line detected, stop the car
      y = 0; // Set speed to stop
    }
  } else {
    // Emergency stop triggered, stop the car
    x = 90; // Set servo to center position
    y = 0;  // Set speed to stop
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

  // Control the car's steering based on Pixy's line detection or emergency stop
  myServo_D2.write(x);

  delay(16); // Adjust according to your needs
}
