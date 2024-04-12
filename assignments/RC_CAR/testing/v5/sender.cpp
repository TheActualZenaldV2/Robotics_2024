//EMERGENCY STOP EXPERIMENTAL WORK 


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
const int joystickPinX = A0; // Pin connected to the joystick
const int joystickPinY = A1; // Pin connected to the joystick

int joystickValueX; // Variable to store the joystick input value
int joystickValueY; // Variable to store the joystick input valu
int parsedX; 
int parsedY;
int speed = 3;
bool emergencyStop = false;
//const int buttonPin = 2; // Change this to the pin you connect the button to

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x34, 0x85, 0x18, 0xAD, 0x01, 0xC8};


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
 void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 //Serial.print("\r\nLast Packet Send Status:\t");
 // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
   }




   
int mapJoystickToMotor(int joystickValueY) {
    // Define the minimum, middle and maximum values for the joystick
    int minJoystick = 0;
    int midJoystick = 1900;
    int maxJoystick = 4095;

    // Define the minimum, middle and maximum values for the motor
    int minMotor = -255;
    int midMotor = 0;
    int maxMotor = 255;

    // Map the joystick value to the motor value
    if (joystickValueY <= midJoystick) {
        return map(joystickValueY, minJoystick, midJoystick, minMotor, midMotor);
    } else {
        return map(joystickValueY, midJoystick, maxJoystick, midMotor, maxMotor);
    }
}

//
//
//
int mapJoystickToServo(int joystickValueX) {
    // Define the minimum, middle and maximum values for the joystick
    int minJoystick = 0;
    int midJoystick = 1955;
    int maxJoystick = 4095;

    // Define the minimum, middle and maximum values for the servo
    int minServo = 0;
    int midServo = 90;
    int maxServo = 180;

    // Map the joystick value to the servo value
    if (joystickValueX <= midJoystick) {
        return map(joystickValueX, minJoystick, midJoystick, minServo, midServo);
    } else {
        return map(joystickValueX, midJoystick, maxJoystick, midServo, maxServo);
    }
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
  //
  //
  //
  //
  //
  //USE BUTTON PRESS SWITCH, INIT PINS HERE 
    //pinMode(buttonPin, INPUT_PULLUP); // Configure the button pin as an input with pull-up resistor

}
 
void loop() {
  if(!emergencyStop){
// Read the analog input from the joystick
  joystickValueX = analogRead(joystickPinX);
    joystickValueY = analogRead(joystickPinY);


    parsedY = mapJoystickToMotor(joystickValueY);
    parsedX = mapJoystickToServo(joystickValueX);

  // Print the joystick value to the serial monitor
  Serial.print("Joystick Value X: ");
  Serial.println(parsedX);
  // Print the joystick value to the serial monitor
  Serial.print("Joystick Value Y: ");
  Serial.println(parsedY/speed);


sprintf(myData.a, "%d", parsedX); // Convert integer x to string and store in myData.a
sprintf(myData.b, "%d", parsedY/speed); // Convert integer y to string and store in myData.b



  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    //sucess
   // Serial.println("Packet Sent Sucessfully");
    //Serial.print("-----------------------------------------------------------------------------------\n");
  }
  else {
    //error 
   // Serial.println("Error sending the data");
    //Serial.print("-----------------------------------------------------------------------------------\n");

  }
  delay(100);
  } else {
    parsedY = 0; // Later set up a special number, recognize it in receiver and apply brakes if detected. 
    parsedX = 90;

    Serial.print("----------------------------------------------------------------------------------------------------------\n");
    Serial.print("----------------------------------------------------------------------------------------------------------\n");
    Serial.print("EMERGENCY STOP DETECTED\n");
    Serial.print("----------------------------------------------------------------------------------------------------------\n");
    Serial.print("----------------------------------------------------------------------------------------------------------\n");

    sprintf(myData.a, "%d", parsedX); // Convert integer x to string and store in myData.a
    sprintf(myData.b, "%d", parsedY); // Convert integer y to string and store in myData.b

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    if (result == ESP_OK) {
        Serial.print("EMERGENCY STOP SENT\n");
        Serial.print("----------------------------------------------------------------------------------------------------------\n");
        Serial.print("----------------------------------------------------------------------------------------------------------\n");
    } else {
        Serial.println("EMERGENCY STOP ERROR");
        Serial.print("----------------------------------------------------------------------------------------------------------\n");
        Serial.print("----------------------------------------------------------------------------------------------------------\n");
        
        // Retry sending the message
        result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        if (result == ESP_OK) {
            Serial.print("EMERGENCY STOP RESENT\n");
            Serial.print("----------------------------------------------------------------------------------------------------------\n");
            Serial.print("----------------------------------------------------------------------------------------------------------\n");
        } else {
            Serial.print("EMERGENCY STOP RESEND ERROR\n");
            Serial.print("----------------------------------------------------------------------------------------------------------\n");
            Serial.print("----------------------------------------------------------------------------------------------------------\n");
        }
    }
}





 // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // Button is pressed, set emergencyStop to true
    emergencyStop = true;
    Serial.println("Emergency stop activated!");
  }
  
}
