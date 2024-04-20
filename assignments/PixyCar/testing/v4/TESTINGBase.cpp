//hopefully fixing multiple objects: should navigate towards the widest (closest???) one
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
    
int myTriggerPin = D6;  // Trigger on RangeFinder
int myEchoPin = D7;     // Echo on Rangefinder  
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
  Serial.println("Duration us: "+ String(myDuration));
  if (myDuration > 1000){       // raw data from 200 to 16000                                          
    //digitalWrite(LEDB, LOW);    // LEDB Blue LED on if far 
    emergencyStop = false;
  } else { 
    //digitalWrite(LEDB, HIGH);   // LEDB Blue LED off if close or nothing
    emergencyStop = true;
  }  

  if(!emergencyStop){
    pixy.ccc.getBlocks(); // Get block data from Pixy
    
    int widestWidth = 0; // Initialize the widest width variable
    int widestIndex = -1; // Initialize the index of the widest object

    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      int objectWidth = pixy.ccc.blocks[i].m_width;
      if (objectWidth > widestWidth) {
        widestWidth = objectWidth;
        widestIndex = i;
      }
    }

    if (widestIndex != -1) { // If a widest object is found
      // Extract the location of the widest object
      int objectX = pixy.ccc.blocks[widestIndex].m_x;
      int objectY = pixy.ccc.blocks[widestIndex].m_y;

      // Map object's position to servo angle range (0 to 180 degrees)
      x = map(objectX, 0, 319, 0, 180);

      // Map object's width (distance) to speed range (30 to 100)
      y = map(widestWidth, 20, 320, 30, 100);

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

    delay(16); // 60 fps also pixy refresh rate 
  }
}
