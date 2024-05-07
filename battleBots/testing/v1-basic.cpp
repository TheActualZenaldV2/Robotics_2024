
//basic movement forward/backwards based off ESPNOW Data - might switch to use websockets later. 
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define LEFT_MOTOR_PWM D5
#define LEFT_MOTOR_DIR D4
#define RIGHT_MOTOR_PWM D6
#define RIGHT_MOTOR_DIR D3

// Structure to receive data
typedef struct struct_message {
  char a[32];
} struct_message;

// Callback when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  Serial.print("Data received: ");
  struct_message *msg = (struct_message *)data;
  Serial.println(msg->a);

  // Parse received data and control motors accordingly
  int speed = atoi(msg->a); // Assuming received data is a number representing motor speed
  if (speed >= 0) {
    // Move forward
    digitalWrite(LEFT_MOTOR_DIR, HIGH);
    digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  } else {
    // Move backward
    digitalWrite(LEFT_MOTOR_DIR, LOW);
    digitalWrite(RIGHT_MOTOR_DIR, LOW);
    speed = abs(speed); // Make speed positive
  }

  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6); // Assuming you have broadcastAddress defined
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register callback for receiving data
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Since motor control is handled in the callback function,
  // we don't need to do anything here
}
