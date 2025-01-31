#include <esp_now.h>
#include <WiFi.h>

//uint8_t broadcastAddress[] = { 0xcc , 0xdb , 0xa7 , 0x3f , 0x6a , 0xc4}; Ip address of the reciever esp, in case we want to change the esp's position
uint8_t broadcastAddress[] = { 0xcc , 0xdb , 0xa7 , 0x34 , 0x29 , 0x14};
esp_now_peer_info_t peerInfo; //Saves the 


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");                                         //these line are used if you want to debug 
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");  //and check if the packets are sent correctly
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // opens a serial connection (BAUD RATE, RX, TX)
                                          //SERIAL_8N1 means that the serial communication will have 8 data bits, no parity, 1 stop bit
                                          //This config should match the MSP one

  //Initialize ESP_now protocol
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent); 

  memcpy(peerInfo.peer_addr, broadcastAddress, 6); //Saves and register the destination IP
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

//Add the peer info
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  
}

typedef struct myData {
  String mensage;
} myData;
myData mydata;

#define BUFFER_SIZE 7  // Tamanho máximo da string recebida +1

char recievedString[BUFFER_SIZE];
int bufferIndex = 0;   
char recievedChar;

void loop() {
  //Loop to read 6 chars and tranform them into a string
  while (Serial1.available()) { //If there is something to read in Serial1
    recievedChar = Serial1.read();  // read a char from Serial1
    recievedString[bufferIndex] = recievedChar; //add to the String array
    bufferIndex++;
    // Verify if we are at the maximum length
    if (bufferIndex >= BUFFER_SIZE - 1) {
      recievedString[bufferIndex] = '\0';  // Adds the null char to finish the String
      bufferIndex = 0;  
    }
  }

  
  mydata.mensage = recievedString; //Saves the recieved string into the sending structure

  //Send the data and saves the output in "result"
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &mydata, sizeof(mydata));

  //While testing we use this structure to avaluate if the data is being correctly sent
  //if (result == ESP_OK) {
  //  Serial.println("Sent with success");
  //}
  //else {
  //  Serial.println("Error sending the data");
  //}
}
 
}
