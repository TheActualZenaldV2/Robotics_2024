/*
//board e8
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>
int joyX =0;
int joyY =0;
int x = 0;
int y= 0;


// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {};


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32]; // Assuming you want to store a string of up to 32 characters in a
  char b[32]; // Adding another array to store another string of up to 32 characters
} struct_message;
// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// // callback when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }
 //
 //
//
 //
 
int mapJoystickToMotor(int joyY) {
    // Define the minimum and maximum values for the joystick and motor
    int minJoystick = -455;
    int maxJoystick = 455;
    int minMotor = 0;
    int maxMotor = 255;

    // Map the joystick value to the motor value
    return map(joyY, minJoystick, maxJoystick, minMotor, maxMotor);
}
//
//
//
int mapJoystickToServo(int joyX) {
    // Define the minimum and maximum values for the joystick and servo
    int minJoystick = -455;
    int maxJoystick = 455;
    int centerJoystick = 0;
    
    int minServo = 0;
    int maxServo = 180;
    int centerServo = 90;
    
    // Map the joystick value to the servo value
    int servoValue = map(joyX, minJoystick, maxJoystick, minServo, maxServo);
    
    // Adjust for the center offset
    servoValue += centerServo - map(centerJoystick, minJoystick, maxJoystick, minServo, maxServo);
    
    // Constrain the servo value within its range
    return constrain(servoValue, minServo, maxServo);
}


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    Serial.print("-----------------------------------------------------------------------------------\n");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    Serial.print("-----------------------------------------------------------------------------------\n");
    return;
  }
}
 
void loop() {
//get X AND Y FROM JOYSTICK
//maybe will work? ::::
//joyX = analogRead(A0); // Replace A0 with actual pin for X-axis 
//joyY = analogRead(A1); // Replace A1 with actual pin for Y-axis
//::::
//
//
//
//
Y = mapJoystickToMotor(joyY);
X = mapJoystickToServo(joyX);

// Set values to send
sprintf(myData.a, "%d", x); // Convert integer x to string and store in myData.a
sprintf(myData.b, "%d", y); // Convert integer y to string and store in myData.b



  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Packet Sent Sucessfully");
    Serial.print("-----------------------------------------------------------------------------------\n");
  }
  else {
    Serial.println("Error sending the data");
    Serial.print("-----------------------------------------------------------------------------------\n");

  }
  delay(10);
}
