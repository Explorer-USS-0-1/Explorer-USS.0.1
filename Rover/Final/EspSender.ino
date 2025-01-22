#include <esp_now.h>
#include <WiFi.h>

//uint8_t broadcastAddress[] = { 0xcc , 0xdb , 0xa7 , 0x3f , 0x6a , 0xc4};
uint8_t broadcastAddress[] = { 0xcc , 0xdb , 0xa7 , 0x34 , 0x29 , 0x14};
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // Configura Serial1

  //Init ESP_now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

//Add the peer 
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
  while (Serial1.available()) {
    recievedChar = Serial1.read();  // Lê o próximo caractere
    recievedString[bufferIndex] = recievedChar;
    bufferIndex++;
    // Verifica se o buffer atingiu o tamanho máximo ou se o caractere recebido é o delimitador de fim de string (opcional)
    if (bufferIndex >= BUFFER_SIZE - 1) {
      recievedString[bufferIndex] = '\0';  // Adiciona o caractere nulo para terminar a string
      bufferIndex = 0;  // Reseta o índice do buffer para a próxima leitura
    }
  }
  //if recieved
  //Serial.println(recievedString);
  mydata.mensage = recievedString;
  //Serial.println(mydata.mensage);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &mydata, sizeof(mydata));
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
 
}
