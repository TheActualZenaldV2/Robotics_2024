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

// Define pin assignments
#define MOTOR1_PWM_PIN D1
#define MOTOR1_DIR_PIN D0
#define MOTOR2_PWM_PIN D2
#define MOTOR2_DIR_PIN D3

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

  x = atoi(myData.a);
  y = atoi(myData.b);

  // Add padding for the center values
  if (y > -10 && y < 10) {
    y = 0;
  }    analogWrite(MOTOR2_PWM_PIN, 0);    // Motor 2 speed

  if (x > 80 && x < 100) {
    x = 90;
        analogWrite(MOTOR1_PWM_PIN, 0);  // Stop Motor 1

  }

  // Print the received values
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("-----------------------------------------------------------------------------------\n");

  // Control Motor 2 (for direction)
  if (y < 0) {
    // Motor 2 backward
    digitalWrite(MOTOR2_DIR_PIN, HIGH);  // Set direction backward for Motor 2
    analogWrite(MOTOR2_PWM_PIN, abs(y));    // Motor 2 speed
  } else if (y > 0) {
    // Motor 2 forward
    digitalWrite(MOTOR2_DIR_PIN, LOW);  // Set direction forward for Motor 2
    analogWrite(MOTOR2_PWM_PIN, y);    // Motor 2 speed
  } else {
    // car is not moving
    analogWrite(MOTOR2_PWM_PIN, 0);  // Stop Motor 2
  }

  // Control Motor 1 (for steering)
  if (x < 90) {
    // Motor 1 forward
    digitalWrite(MOTOR1_DIR_PIN, HIGH);  // Set direction forward for Motor 1
    analogWrite(MOTOR1_PWM_PIN, 255);    // Motor 1 maximum speed
  } else if (x > 90) {
    // Motor 1 backward
    digitalWrite(MOTOR1_DIR_PIN, LOW);  // Set direction backward for Motor 1
    analogWrite(MOTOR1_PWM_PIN, 255);   // Motor 1 maximum speed
  } else {
    // Motor 1 stop
    analogWrite(MOTOR1_PWM_PIN, 0);  // Stop Motor 1
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

  // Initialize motor control pins as outputs
  pinMode(MOTOR1_PWM_PIN, OUTPUT); // PWM for Motor 1
  pinMode(MOTOR1_DIR_PIN, OUTPUT); // Direction for Motor 1
  pinMode(MOTOR2_PWM_PIN, OUTPUT); // PWM for Motor 2
  pinMode(MOTOR2_DIR_PIN, OUTPUT); // Direction for Motor 2
}

void loop() {
  // No need to add code here for now
}
