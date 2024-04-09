/*
  Rui Santos original code 
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.


*/

#include <esp_now.h>
#include <WiFi.h>
#include <Servo.h>
Servo myServo_D2;
//CONNECT TO D2

// Structure example to receive
// Must match the sender structure
typedef struct struct_message {
  char a[32]; // Assuming you want to store a string of up to 32 characters in a
  char b[32]; // Adding another array to store another string of up to 32 characters
} struct_message;
// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
Serial.print("X: ");
Serial.println(myData.a);
Serial.print("Y: ");
Serial.println(myData.b);
Serial.print("-----------------------------------------------------------------------------------\n");
//
//
//MOVE DATA INTO VARIABLES:
memcpy(&myData, incomingData, sizeof(myData)); // Copy received data to myData struct
  
// Extract value of "X and Y" from the received data
int X = atoi(myData.a); // Convert string to integer
int Y = atoi(myData.b); // Convert string to integer
//
//
//
//MOVE SERVO:
myServo_D2.write(X); 


}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    Serial.print("-----------------------------------------------------------------------------------\n");
    return;
  } else {
        Serial.println("ESP-NOW Init Success");
        Serial.print("-----------------------------------------------------------------------------------\n");
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  //
  //
  //
  //
  //SERVO CODE:
myServo_D2.attach(D2);   // D2 should do PWM on Portenta

}
 
void loop() {

}
