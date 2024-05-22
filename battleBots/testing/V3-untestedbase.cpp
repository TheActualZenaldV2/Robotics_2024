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

int x;
int y;

// MAC address of the sender
uint8_t broadcastAddress[] = {0x34, 0x85, 0x18, 0xAD, 0x86, 0x70};

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
    // car is going backwards
    digitalWrite(D4, LOW);    // set direction forward for Motor 1
    digitalWrite(D2, LOW);    // set direction forward for Motor 2
    analogWrite(D4, abs(y)); // set speed for motor 1
    analogWrite(D5, abs(y)); // set speed for motor 2
  } else if (y > 0){
    // car is going forwards
    digitalWrite(D4, HIGH);    // set direction forward for Motor 1
    digitalWrite(D2, HIGH);    // set direction forward for Motor 2
    analogWrite(D4, y);     // set speed for motor 1
    analogWrite(D5, y);     // set speed for motor 2
  } else {
    // car is not moving
    digitalWrite(D2, LOW);    
    digitalWrite(D3, LOW);    // GLIDING
    analogWrite(D4, 0);       // set speed for motor 1
    analogWrite(D5, 0);       // set speed for motor 2
  }
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
  
  // Register for receive callback to get received packet info
  esp_now_register_recv_cb(OnDataRecv);
    pinMode(D5, OUTPUT);   // PWM for Motor 1
    pinMode(D4, OUTPUT);   // Direction for Motor 1
    pinMode(D3, OUTPUT);   // PWM for Motor 2
    pinMode(D2, OUTPUT);   // Direction for Motor 2

}

void loop() {
  // No need to add code here for now
}
