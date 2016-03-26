#include <Servo.h>
#include <Math.h>

#define SERVO_BASE_PIN 3
#define SERVO_ARM_PIN 5
#define SERVO_ELBOW_PIN 6
#define SERVO_WRIST_PIN 9
#define SERVO_HAND_PIN 10
#define SERVO_GRASPER_PIN 11

#define JOYSTICK_B_X_PIN A1
#define JOYSTICK_B_Y_PIN A0
#define JOYSTICK_A_X_PIN A5
#define JOYSTICK_A_Y_PIN A4

#define JOYSTICK_B_BUTTON_PIN 2
#define JOYSTICK_A_BUTTON_PIN 4

#define JOYSTICK_THRESHOLD 100

#define MAX_SPEED 5

#define LENGTH_ARM 14.5
#define LENGTH_ELBOW 9

#define RAD_TO_DEG 57.297

Servo servoBase;
Servo servoArm;
Servo servoElbow;
Servo servoWrist;
Servo servoHand;
Servo servoGrasper;

int button_A = 0;
int button_B = 0;

double r;
double z;
double b;

double r_c;
double z_c;
double b_c;

int mode = 0;
int grasp_toggle = 0;

void setup() {
    Serial.begin(9600);
    
    servoBase.attach(SERVO_BASE_PIN);
    servoArm.attach(SERVO_ARM_PIN);
    servoElbow.attach(SERVO_ELBOW_PIN);
    servoWrist.attach(SERVO_WRIST_PIN);
    servoHand.attach(SERVO_HAND_PIN);
    servoGrasper.attach(SERVO_GRASPER_PIN);
    
    pinMode(JOYSTICK_A_X_PIN, INPUT);
    pinMode(JOYSTICK_A_Y_PIN, INPUT);
    pinMode(JOYSTICK_B_X_PIN, INPUT);
    pinMode(JOYSTICK_B_Y_PIN, INPUT);
    pinMode(JOYSTICK_A_BUTTON_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_B_BUTTON_PIN, INPUT_PULLUP);
    reset();
    //Write initial positions
}

void loop() {
    
    if (digitalRead(JOYSTICK_A_BUTTON_PIN) != button_A){
        /* button A trigger */
        button_A = !button_A;   
        if (button_A == 1){
          grasp();          
        }

    }
    if (digitalRead(JOYSTICK_B_BUTTON_PIN) != button_B){
        /* button B trigger */
        
        button_B = !button_B;   
    }
    b=b_c;
    r=r_c;
    z=z_c;
    
    
    int joy_a_x = analogRead(JOYSTICK_A_X_PIN)-512;
    b -= (abs(joy_a_x) > JOYSTICK_THRESHOLD) ? (joy_a_x/512.0)*MAX_SPEED : 0;
    
    int joy_a_y = analogRead(JOYSTICK_A_Y_PIN)-512;
    r -= (abs(joy_a_y) > JOYSTICK_THRESHOLD) ? (joy_a_y/512.0) : 0;
    
    int joy_b_y = analogRead(JOYSTICK_B_Y_PIN)-512; 
    z -= (abs(joy_b_y) > JOYSTICK_THRESHOLD) ? (joy_b_y/512.0) : 0;
    
    int joy_b_x = analogRead(JOYSTICK_B_X_PIN);

    
    if (r != r_c || z != z_c || b != b_c) {
        if (b < 180 && b > 0 && r > 0 && r < 23 && z > -5 && z < 23){
          if (sqrt(pow(r,2)+pow(z,2)) < LENGTH_ARM+LENGTH_ELBOW &&  sqrt(pow(r,2)+pow(z,2)) > LENGTH_ARM-LENGTH_ELBOW) {
              moveToPosition(r,z,b);   
          }
        }
    }
    
    
}
void grasp(){
    grasp_toggle = !grasp_toggle;
    int pos = grasp_toggle ? 1000 : 1500;
    servoGrasper.writeMicroseconds(pos);
}
void moveToPosition(double r, double z, double b) {
    
    double l = sqrt(pow(r,2) + pow(z,2));
    double alpha = atan(z/r);
    double beta = acos((pow(LENGTH_ELBOW,2)-pow(LENGTH_ARM,2)-pow(l,2))/(-2*LENGTH_ARM*l));
    double theta2 = alpha + beta;
    double phi = asin((l/LENGTH_ELBOW)*sin(beta));
    phi = (l>sqrt(pow(LENGTH_ARM,2) + pow(LENGTH_ELBOW,2))) ? PI-phi : phi; 
    double theta3 = -(PI-phi);
    
    moveToAngles(b,(int)RAD_TO_DEG*theta2,(int)RAD_TO_DEG*theta3);
    
    r_c = r;
    z_c = z;
    b_c = b;
}

//put in angles in the code convention, it will calculate actual angles
void moveToAngles(int a1, int a2, int a3){

    servoBase.write(180-a1); 
    servoArm.write(30+a2);
    servoElbow.write(-a3);

    //increasing microseconds moves it clockwise when looking into horn
}

void reset() {
    moveToAngles(0,90,-90);
    r=9;
    z=14.5;
    b=0;
    r_c=9;
    z_c=14.5;
    b_c=0;
}
