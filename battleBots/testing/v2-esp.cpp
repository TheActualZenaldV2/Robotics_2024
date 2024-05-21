#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Structure to receive data
typedef struct struct_message {
    char a[32]; // Assuming you want to store a string of up to 32 characters in a
    char b[32]; // Adding another array to store another string of up to 32 characters
} struct_message;

// Create a struct_message called receivedData
struct_message receivedData;

// Pin definitions
const int motor1PWM = D5;
const int motor1Dir = D4;
const int motor2PWM = D3;
const int motor2Dir = D2;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.println("Received data:");
    Serial.print("Parsed X: ");
    Serial.println(receivedData.a);
    Serial.print("Parsed Y: ");
    Serial.println(receivedData.b);

    int parsedX = atoi(receivedData.a); // Convert string to integer for parsedX
    int parsedY = atoi(receivedData.b); // Convert string to integer for parsedY

    // Adjust motor control based on received data
    if (parsedY >= 0) {
        digitalWrite(motor1Dir, HIGH); // Set direction forward for Motor 1
        digitalWrite(motor2Dir, HIGH); // Set direction forward for Motor 2
    } else {
        digitalWrite(motor1Dir, LOW); // Set direction backward for Motor 1
        digitalWrite(motor2Dir, LOW); // Set direction backward for Motor 2
    }

    analogWrite(motor1PWM, abs(parsedY)); // Set speed for Motor 1
    analogWrite(motor2PWM, abs(parsedY)); // Set speed for Motor 2
}

void setup() {
    Serial.begin(115200);

    // Set up motor control pins
    pinMode(motor1PWM, OUTPUT);
    pinMode(motor1Dir, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
    pinMode(motor2Dir, OUTPUT);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, register for recv CB to get recv packet info
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // Your main code logic here
}
