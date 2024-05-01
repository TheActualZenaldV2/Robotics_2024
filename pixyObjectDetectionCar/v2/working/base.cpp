//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
#include <ESP32Servo.h>

#include <Pixy2.h>
#include <PIDLoop.h>
Servo myServo_D2;

Pixy2 pixy;
PIDLoop panLoop(700, 0, 100, true);
PIDLoop tiltLoop(800, 0, 100, true);
int servoOutput;
int x = 90; // Default servo position
int y = 0;  // Default speed (stop)


void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
 
  // We need to initialize the pixy object 
  pixy.init();
  // Use color connected components program for the pan tilt to track 
  pixy.changeProg("color_connected_components");

   int8_t result = pixy.setLamp(1, 1); // Turns on both upper white LEDs and RGB LED
  
  if (result == PIXY_RESULT_OK) {
    Serial.println("Lamp turned on successfully");
  } else {
    Serial.println("Failed to turn on lamp");
  }
  myServo_D2.attach(D6); // Attach servo to pin D6 (or adjust to your setup)

  pinMode(D3, OUTPUT); // digital 0 to 1
  pinMode(D4, OUTPUT); // PWM 0 to 255
  pinMode(D5, OUTPUT); // digital 0 to 1
}

void loop()
{  
  static int i = 0;
  int j;
  char buf[64]; 
  int32_t panOffset, tiltOffset;
  
  // get active blocks from Pixy
  pixy.ccc.getBlocks();
  
  if (pixy.ccc.numBlocks)
  {        
    i++;
    
    if (i%60==0)
      Serial.println(i);   
    
    // calculate pan and tilt "errors" with respect to first object (blocks[0]), 
    // which is the biggest object (they are sorted by size).  
    panOffset = (int32_t)pixy.frameWidth/2 - (int32_t)pixy.ccc.blocks[0].m_x;
    tiltOffset = (int32_t)pixy.ccc.blocks[0].m_y - (int32_t)pixy.frameHeight/2;  
  
    // update loops
    panLoop.update(panOffset);
    tiltLoop.update(tiltOffset);
  
    // set pan and tilt servos  
    pixy.setServos(panLoop.m_command, tiltLoop.m_command);
     // Print pan servo value to serial monitor
// Print pan servo value to serial monitor
// Print pan servo value to serial monitor
int servoOutput = panLoop.m_command; 

// Map the Pixy servo output to the steering range
int mainServoValue = map(servoOutput, 1000, 0, 0, 180);

// Ensure the main servo value stays within the valid range of 0-180
mainServoValue = constrain(mainServoValue, 0, 180);
 // Control the car's steering based on Pixy's output
    myServo_D2.write(mainServoValue);


Serial.print("X: "); // Print the label
Serial.println(mainServoValue); // Print the main servo value
      digitalWrite(D5, 1); // Set one direction
      digitalWrite(D3, 0); // Set other direction
      analogWrite(D4, 40); // Set speed

   
#if 0 // for debugging
    sprintf(buf, "%ld %ld %ld %ld", rotateLoop.m_command, translateLoop.m_command, left, right);
    Serial.println(buf);   
#endif

  }  
  else // no object detected, go into reset state
  {
    panLoop.reset();
    tiltLoop.reset();
    pixy.setServos(panLoop.m_command, tiltLoop.m_command);
    digitalWrite(D5, 1); // Set one direction
      digitalWrite(D3, 1); // Set other direction
      analogWrite(D4, 0); // Set speed
  }
}


