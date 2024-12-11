#include <esp_now.h>
#include <WiFi.h>
#include <ezButton.h>

#define VRX_PIN  34 // Arduino pin connected to VRX pin
#define VRY_PIN  35 // Arduino pin connected to VRY pin
#define SW_PIN 33 //Arduino pin connected so SW

ezButton button(SW_PIN);

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = { 0xcc , 0xdb , 0xa7 , 0x34 , 0x29 , 0x14};

// Variable to store if sending data was successful
String success;
String dir;
int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int speed=0;
esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  button.setDebounceTime(50);
  dir="off";
  speed=0;
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
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
    return;
  }
}
typedef struct myData {
  String dir;
  int speed;
} myData;

// Create a struct_message called myData
int buton;
myData mydata;
int i = 0;

void loop() {

  button.loop();
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  buton = digitalRead(SW_PIN);

  // Set values to send
  if (xValue<=1500){
    dir="left";
  }
  else if (xValue>=2100){
    dir="right";
  }
  else if (yValue<=1800){
    dir="forward";
  }
  else if(yValue>=2500){
    dir="backward";
  }
  else {dir="off";}
  if (buton==0){
    dir="off";
    i=i+1;
    if (i%2) {
      speed=0;
    }
      else {speed=1;
    }
  }

  mydata.dir = dir;
  mydata.speed= speed;


  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &mydata, sizeof(mydata));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10);
}