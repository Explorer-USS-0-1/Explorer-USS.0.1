#include <esp_now.h>
#include <WiFi.h>

#define MOTOR1_SPEED 27
#define MOTOR2_SPEED 26
#define MOTOR3_SPEED 14
#define MOTOR4_SPEED 25

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  String dir;
  int speed;
  
} struct_message;

struct_message myData;

int motor1pin1 = 21;
int motor1pin2 = 33;
int motor2pin1 = 16;
int motor2pin2 = 17;
int motor3pin1 = 18;
int motor3pin2 = 19;
int motor4pin1 = 22;
int motor4pin2 = 23;

String command;
int speed_control;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Dir: ");
  Serial.println(myData.dir);
  
  // reset commands
  command = "off";
  speed_control=0;

  command = myData.dir;
  speed_control = myData.speed;

  if (command == "left") {
    Serial.println("COMMAND LEFT");
    leftstop();
    rightstop();
    leftforward();
  }

  if (command == "right") {
    Serial.println("COMMAND RIGHT");
    leftstop();
    rightstop();
    rightforward();
  }

  if (command == "forward") {
    Serial.println("COMMAND UP");
    leftstop();
    rightstop();
    rightforward();
    leftforward();
  }

  if (command == "backward") {
    Serial.println("COMMAND DOWN");
    leftstop();
    rightstop();
    rightbackwards();
    leftbackwards();
  }

  if(command == "off"){
    leftstop();
    rightstop();
  }

  if(speed_control == 1){
      analogWrite(MOTOR1_SPEED,255);
      analogWrite(MOTOR2_SPEED,255);
      analogWrite(MOTOR3_SPEED,255);
      analogWrite(MOTOR4_SPEED,255);
  }
  else{
      analogWrite(MOTOR1_SPEED,200);
      analogWrite(MOTOR2_SPEED,200);
      analogWrite(MOTOR3_SPEED,200);
      analogWrite(MOTOR4_SPEED,200);
  }
}

void rightforward(){
  digitalWrite(motor1pin1,HIGH);
  digitalWrite(motor1pin2,LOW);

  digitalWrite(motor2pin1,HIGH);
  digitalWrite(motor2pin2,LOW);
}

void leftforward(){
  digitalWrite(motor3pin1,LOW);
  digitalWrite(motor3pin2,HIGH);
  
  digitalWrite(motor4pin1,LOW);
  digitalWrite(motor4pin2,HIGH);
}

void leftbackwards(){
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,HIGH);

  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,HIGH);
}

void rightbackwards(){
  digitalWrite(motor3pin1,HIGH);
  digitalWrite(motor3pin2,LOW);

  digitalWrite(motor4pin1,HIGH);
  digitalWrite(motor4pin2,LOW);
}

void rightstop(){
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,LOW);

  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,LOW);
}

void leftstop(){
  digitalWrite(motor3pin1,LOW);
  digitalWrite(motor3pin2,LOW);

  digitalWrite(motor4pin1,LOW);
  digitalWrite(motor4pin2,LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor3pin1, OUTPUT);
  pinMode(motor3pin2, OUTPUT);
  pinMode(motor4pin1, OUTPUT);
  pinMode(motor4pin2, OUTPUT);

  pinMode(MOTOR1_SPEED, OUTPUT);
  pinMode(MOTOR2_SPEED, OUTPUT);
  pinMode(MOTOR3_SPEED, OUTPUT);
  pinMode(MOTOR4_SPEED, OUTPUT);

  Serial.begin(115200);

  analogWrite(MOTOR1_SPEED,200);
  analogWrite(MOTOR2_SPEED,200);
  analogWrite(MOTOR3_SPEED,200);
  analogWrite(MOTOR4_SPEED,200);

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

