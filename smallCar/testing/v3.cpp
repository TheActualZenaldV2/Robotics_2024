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




  // Print the received values
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("-----------------------------------------------------------------------------------\n");

  // Control Motor 2 (for direction)
  if (y < 15) {
      digitalWrite(0, HIGH);    // set direction forward for Motor 2
    analogWrite(D1, 55);       // Motor 2 
  } else if (y > 15) {
        digitalWrite(0, LOW);    // set direction forward for Motor 2
    analogWrite(D1, 55);       // Motor 2 
  } else {
    // car is not moving
     analogWrite(D1, 0);       // Motor 2 

  }

  // Control Motor 1 (for steering)
  if (x < 75) {
    // Motor 1 forward
       // Motor 2 forward
    digitalWrite(D3, LOW);    // set direction forward for Motor 2
    analogWrite(D2, 150);       // Motor 2 
   
  } else if (x > 105) {
    // Motor 1 backward
     digitalWrite(D3, HIGH);    // set direction forward for Motor 2
    analogWrite(D2, 150);       // Motor 2 
   
  } else {
    // Motor 1 stop
    analogWrite(D2, 0);       

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

pinMode(D1, OUTPUT);   // PWM for Motor 1
    pinMode(D0, OUTPUT);   // Direction for Motor 1
    pinMode(D2, OUTPUT);   // PWM for Motor 2
    pinMode(D3, OUTPUT);   // Direction for Motor 2
//2= w/b
}

void loop() {
  // No need to add code here for now
}
