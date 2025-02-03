#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  String dirX;
  String dirY;
  
} struct_message;

// Create a struct_message called myData
struct_message myData;

Servo servoX;
Servo servoY;

int posX=87;
int posY=63;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("DirX: ");
  Serial.println(myData.dirX);
  Serial.print("DirY: ");
  Serial.println(myData.dirY);

  if (myData.dirX=="right") {
    servoX.write(42);
  }
  if (myData.dirX=="left") {
    servoX.write(142);
  }
  if (myData.dirY=="down") {
    servoY.write (30);
  }
  if (myData.dirY=="up") {
    servoY.write(115);
  }
  if ((myData.dirX=="center") && (myData.dirY=="center")) {
    servoX.write(87);
    servoY.write(63);
  }  
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  servoX.attach(13);
  servoY.attach(32);
  servoX.write(posX);
  servoY.write(posY);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
void loop() {

}
