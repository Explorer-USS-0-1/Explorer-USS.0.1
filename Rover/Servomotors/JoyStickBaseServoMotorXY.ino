#include <Servo.h>
#include <ezButton.h>  //you have to download it 


#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN 2

Servo myservo;

ezButton button(SW_PIN);


int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int pos = 0;

void setup() {
  Serial.begin(9600) ;
  myservo.attach(9);
  myservo.write(90);
  button.setDebounceTime(50);
}

void loop() {

  button.loop();

//  while (button.isPressed()){
//    seril.print('pressed');
//    if (button.isReleased()){
//      serial.print('released');
//    }
//  }
  int buton=digitalRead(SW_PIN);
  if (buton==0) {
    myservo.write(87);
  }
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  if (xValue<=300) {
    myservo.write(142);
  }
  else if (xValue>=800) {
    myservo.write(42);
  }

}
