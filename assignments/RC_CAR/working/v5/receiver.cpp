

/* v4.0
  ESP-NOW Code by Rui Santos at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
//
//
  RC-Car Code
  Receiver/Driver 
  Use Big Motor Driver & Servo motors
  XIAO_ESP32_S3

*/

#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

int x;
int y;

Servo myServo_D2;

// Structure example to receive
// Must match the sender structure
typedef struct struct_message {
  char a[32]; // Assuming you want to store a string of up to 32 characters in a
  char b[32]; // Adding another array to store another string of up to 32 characters
} struct_message;
// Create a struct_message called myData
struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // Copy the received data into the struct_message variable
  memcpy(&myData, incomingData, sizeof(myData));

  // Print the received values
  Serial.print("X: ");
  Serial.println(myData.a);
  Serial.print("Y: ");
  Serial.println(myData.b);
  Serial.print("-----------------------------------------------------------------------------------\n");
x = atoi(myData.a);
y = atoi(myData.b);


if(y < 0){
  //car is going backwards. find backwards direction.
  digitalWrite(D5, 0);    // set other direction
  digitalWrite(D3, 1);    // set one direction
  analogWrite(D4, abs(y));   // set speed to positive value
} else if (y > 0){
  //car is going forwards.
  digitalWrite(D5, 1);    // set other direction
  digitalWrite(D3, 0);    // set one direction
  analogWrite(D4, y);   // set speed
} else if (y == 0) {
  //car is not moving.
  digitalWrite(D5, 0);    
  digitalWrite(D3, 0);    //GLIDING 
  analogWrite(D4, y);   // set speed

}

  myServo_D2.write(x);


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

   pinMode(D3, OUTPUT);   // digital 0 to 1
    pinMode(D4, OUTPUT);   // PWM 0 to 255
    pinMode(D5, OUTPUT);   // digital 0 to 1

      myServo_D2.attach(D6);   // D2 should do PWM on Portenta

}
 
void loop() {

}
