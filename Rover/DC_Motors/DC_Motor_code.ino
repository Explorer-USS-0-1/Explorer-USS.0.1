#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define MOTOR1_SPEED 3
#define MOTOR2_SPEED 5
#define MOTOR3_SPEED 6
#define MOTOR4_SPEED 9

#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

int motor1pin1 = 2;
int motor1pin2 = 4;
int motor2pin1 = 12;
int motor2pin2 = 13;
int motor3pin1 = 7;
int motor3pin2 = 8;
int motor4pin1 = 10;
int motor4pin2 = 11;

int sw=A2;

int xValue = 0 ; // To store value of the X axis
int yValue = 0 ; // To store value of the Y axis
int swValue = 0;

int swState = 0;

int command = COMMAND_NO;

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

void rightbackwards(){
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,HIGH);

  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,HIGH);
}

void leftbackwards(){
  digitalWrite(motor3pin1,HIGH);
  digitalWrite(motor3pin2,LOW);

  digitalWrite(motor4pin1,HIGH);
  digitalWrite(motor4pin2,LOW);
}

void leftstop(){
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,LOW);

  digitalWrite(motor2pin1,LOW);
  digitalWrite(motor2pin2,LOW);
}

void rightstop(){
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

  pinMode(sw, INPUT);

  Serial.begin(9600);

  digitalWrite(sw,HIGH);

  analogWrite(MOTOR1_SPEED,80);
  analogWrite(MOTOR2_SPEED,80);
  analogWrite(MOTOR3_SPEED,80);
  analogWrite(MOTOR4_SPEED,80);
}

void loop() {

  
  // put your main code here, to run repeatedly:
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  swValue = digitalRead(sw);

  // converts the analog value to commands
  // reset commands
  command = COMMAND_NO;

  // check left/right commands
  if (xValue < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  // check up/down commands
  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS
  Serial.print("\"(");
  Serial.print(xValue);
  Serial.print(", ");
  Serial.print(yValue);
  Serial.print(", ");
  Serial.print(swValue);
  Serial.print("\")");
  // print command to serial and process command
  if (command == COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
    leftstop();
    rightstop();
    leftforward();
  }

  if (command == COMMAND_RIGHT) {
    Serial.println("COMMAND RIGHT");
    leftstop();
    rightstop();
    rightforward();
  }

  if (command == COMMAND_UP) {
    Serial.println("COMMAND UP");
    leftstop();
    rightstop();
    rightforward();
    leftforward();
  }

  if (command == COMMAND_DOWN) {
    Serial.println("COMMAND DOWN");
    leftstop();
    rightstop();
    rightbackwards();
    leftbackwards();
  }

  if(command == COMMAND_NO){
    leftstop();
    rightstop();
  }

  if(swValue == 0){
    Serial.println("SWITCHING BUTTON");
    swState=(swState+1)%2;
    Serial.println("--");
    Serial.println(swState);
    if(swState == 1){
      analogWrite(MOTOR1_SPEED,255);
      analogWrite(MOTOR2_SPEED,255);
      analogWrite(MOTOR3_SPEED,255);
      analogWrite(MOTOR4_SPEED,255);
    }
    else{
      analogWrite(MOTOR1_SPEED,100);
      analogWrite(MOTOR2_SPEED,100);
      analogWrite(MOTOR3_SPEED,60);
      analogWrite(MOTOR4_SPEED,60);
    }
  }
  
}
