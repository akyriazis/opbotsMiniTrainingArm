#include <Servo.h>
/* LEFT JOYSTICK */
Servo bRotation;
Servo bCouple;
Servo aHeight;

int bRotationPin = A5;
int bCouplePin = A4;
int buttonPinLeft = 4;

int bRotationPosition = 0;
int bCouplePosition = 0;
int buttonStateLeft = 0;
int aHeightPosition = 0;

boolean currStateLeft = 1;
boolean buttonPressedLeft = false;
boolean buttonReleasedLeft = false;
int ModeLeft = 1;
int previousLeft = 1;

/* RIGHT JOYSTICK */
Servo x;
Servo y;
Servo wrist;

int xPin = A1; 
int yPin = A0;
int buttonPinRight = 2;

int xPosition = 0;
int yPosition = 0;
int buttonStateRight = 0;
int wristPosition = 0;

boolean currStateRight = 1;
boolean buttonPressedRight = false;
boolean buttonReleasedRight = false;
int ModeRight = 1;
int previousRight = 1;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
  /*RIGHT*/
  y.attach(9);
  x.attach(10);
  wrist.attach(11);
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPinRight, INPUT_PULLUP); 
  
  /*LEFT*/
  bRotation.attach(3);
  bCouple.attach(5);
  aHeight.attach(6);
  
  pinMode(bRotationPin, INPUT);
  pinMode(bCouplePin, INPUT);
  
  pinMode(buttonPinLeft, INPUT_PULLUP);
  
  // For versions prior to Arduino 1.0.1
  // pinMode(buttonPin, INPUT);
  // digitalWrite(buttonPin, HIGH);
 

}

void loop() {
  
  /*RIGHT*/
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  wristPosition = yPosition;
  previousRight = 1;
  buttonStateRight = digitalRead(buttonPinRight);
  
  Serial.print(" | Button: ");
  Serial.println(buttonStateRight);
  Serial.print(" | Mode: ");
  Serial.print(ModeRight);
  
  while (buttonStateRight != currStateRight) {
    
    buttonPressedRight = true;
    if (buttonPressedRight == true){
      Serial.print(buttonStateRight);
      Serial.print(currStateRight);
      if (buttonStateRight != currStateRight){
        buttonReleasedRight = true;
        buttonPressedRight = false;
      }
    }
    if (buttonReleasedRight == true){
      buttonReleasedRight = false;
      buttonPressedRight = false;
      if(buttonStateRight == currStateRight){
        Serial.print("RELEASED");
      }
    }
    int stateRight = digitalRead(buttonPinRight);
    if(stateRight != buttonStateRight) {
      ModeRight = !ModeRight;
      break;
    }
  }

  if (ModeRight == 1) {
    int servoX = map(xPosition, 0, 1024, 180, 0);
    int servoWrist = map(wristPosition, 0, 1024, 180, 0);
    wrist.write(servoWrist);  
    x.write(servoX);
    
    delay(100); // add some delay between reads
  }
  
  else if (ModeRight == 0) {
    int servoY = map(yPosition, 0, 1024, 0, 180);  // Height of upper arm
    y.write(servoY);
    
    delay(100);
  }
  
  /*LEFT*/
  bRotationPosition = analogRead(bRotationPin);
  bCouplePosition = analogRead(bCouplePin);
  aHeightPosition = bCouplePosition;
  previousLeft = 1;
  buttonStateLeft = digitalRead(buttonPinLeft);
  
  while (buttonStateLeft != currStateLeft) {
    
    buttonPressedLeft = true;
      if (buttonStateLeft != currStateLeft){
        buttonReleasedLeft = true;
        buttonPressedLeft = false;
      }
    if (buttonReleasedLeft == true){
      buttonReleasedLeft = false;
      buttonPressedLeft = false;
    }
    int stateLeft = digitalRead(buttonPinLeft);
    if(stateLeft != buttonStateLeft) {
      ModeLeft = !ModeLeft;
      break;
    }
  }

  if (ModeLeft == 1) {
    int servoRotation = map(bRotationPosition, 0, 1024, 0, 180);
    int servoCouple = map(bCouplePosition, 0, 1024, 0, 180);
    bRotation.write(servoRotation);  
    bCouple.write(servoCouple);
    
    delay(100); // add some delay between reads
  }
  
  else if (ModeLeft == 0) {
    int servoaHeight = map(aHeightPosition, 0, 1024, 0, 95);  // Gripper close/open
    aHeight.write(servoaHeight);
    
    delay(10);
  }
}


  
