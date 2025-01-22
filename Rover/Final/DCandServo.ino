#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define MOTOR1_SPEED 27
#define MOTOR2_SPEED 26
#define MOTOR3_SPEED 14
#define MOTOR4_SPEED 25

Servo servoX;
Servo servoY;

// Structure example to receive data
// Must match the sender structure
typedef struct myData {
  String mensage;
} myData;
myData RecievedData;

int motor1pin1 = 21;
int motor1pin2 = 33;
int motor2pin1 = 16;
int motor2pin2 = 17;
int motor3pin1 = 18;
int motor3pin2 = 19;
int motor4pin1 = 22;
int motor4pin2 = 23;

String DATA;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&RecievedData, incomingData, sizeof(RecievedData));
  //Serial.print("Mensage: ");
  //Serial.println(myData.mensage);

  DATA = RecievedData.mensage;

    if (DATA == "left..") {
    //Serial.println("COMMAND LEFT");
      leftstop();
      rightstop();
      rightforward();
      leftbackwards();
    }
    else if (DATA == "right.") {
         // Serial.println("COMMAND RIGHT");
      leftstop();
      rightstop();
      leftforward();
      rightbackwards();
    } 
    else if (DATA == "forwar") {
          //Serial.println("COMMAND UP");
      leftstop();
      rightstop();
      rightforward();
      leftforward();
    }
    else if (DATA == "backwa") {
          //Serial.println("COMMAND DOWN");
      leftstop();
      rightstop();
      rightbackwards();
      leftbackwards();
    }
    else if(DATA == "defaul"){
      leftstop();
      rightstop();
    }
    else if (DATA == "cright") {
        servoX.write(42);
    }
    else if (DATA == "cleft.") {
      servoX.write(142);
    }
    else if (DATA == "down..") {
      servoY.write (30);
    }
    else if (DATA == "up....") {
      servoY.write(115);
    }
    else if (DATA == "middle") {
      servoX.write(87);
      servoY.write(63);
    }
}  
/* SPEED CONTROLS; SAVE IF NEEDED LATER
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
  }*/


void leftforward(){
  digitalWrite(motor1pin1,HIGH);
  digitalWrite(motor1pin2,LOW);

  digitalWrite(motor2pin1,HIGH);
  digitalWrite(motor2pin2,LOW);
}

void rightforward(){
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

  servoX.attach(13);
  servoY.attach(32);
  servoX.write(87);
  servoY.write(63);

  WiFi.mode(WIFI_STA);

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
