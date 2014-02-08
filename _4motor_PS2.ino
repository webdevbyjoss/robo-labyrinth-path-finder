#include <robocraft.h>
#include <NewPing.h>
#include "Servo.h"
#include <PS2X_lib.h>  //for v1.6


MOTOR MOTORR1 = { 22, 23, 9 };
MOTOR MOTORR2 = { 24, 25, 10 };
MOTOR MOTORL1 = { 26, 27, 11 };
MOTOR MOTORL2 = { 28, 29, 12 };
RoboCraft robotL(&MOTORL1, &MOTORL2);
RoboCraft robotR(&MOTORR1, &MOTORR2);

Servo myservo;
#define EYE 8
#define BASE 82
boolean eyeMove;
int eyePos;
int eyeDir = 1;

#define TRIGGER_PIN  30  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     31  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned int uS;

#define STOP 0
#define RUN 1
int state = 0;
boolean obstacle = false;

// PS2
PS2X ps2x; // create PS2 Controller Class
//CONTROLLER VOLTAGE MUST BE SET TO 3.3V, NOT 5V
//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int PS2error = 0; 
byte PS2type = 0;
int PS2temp = 0;
byte PS2vibrate = 0;

unsigned int LYstate = 127;
unsigned int RYstate = 127;
unsigned int speed;

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  robotL.motor_forward(0,0);
  robotR.motor_forward(0,0);
  state = STOP;
  myservo.attach(EYE);
  eyePos = BASE;
  myservo.write(eyePos);
  delay(1000);
  myservo.detach();
  clearEyeTimer();
  
  // PS2
  Serial.begin(57600);
  PS2error = ps2x.config_gamepad(41,39,38,40, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if(PS2error == 0){
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if(PS2error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if(PS2error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(PS2error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  PS2type = ps2x.readType(); 
  switch(PS2type) {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    break;
  }
}

void loop()
{
  if(PS2error == 1) //skip loop if no controller found
    return; 

  if(PS2type == 2){ //Guitar Hero Controller    
  }

  else { //DualShock Controller

    ps2x.read_gamepad(false, PS2vibrate);          //read controller and set large motor to spin at 'vibrate' speed
 
    /* Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC); 
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC); 
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC); 
   */

//speed = (unsigned char)ps2x.Analog(PSS_LY);
//Serial.print("Got ");
//Serial.print(speed);
//Serial.println("");

    if (LYstate != (unsigned char)ps2x.Analog(PSS_LY)) {
      //Serial.println("Change L speed");
      LYstate = (unsigned char)ps2x.Analog(PSS_LY);
      if ((LYstate == 127) || (LYstate == 128)) {
        //Serial.println("L stop");
        robotL.motor_forward(0,0);
      } else if (LYstate < 127) {
        speed = 255 - LYstate;
        robotL.motor_forward(0,speed);
      } else {
        speed = LYstate;
        robotL.motor_back(0,speed);
      }
    }
    if (RYstate != (unsigned char)ps2x.Analog(PSS_RY)) {
      //Serial.println("Change R speed");
      //Serial.print("Got ");
      RYstate = (unsigned char)ps2x.Analog(PSS_RY);
      //Serial.println(RYstate);
      if ((RYstate == 127) || (RYstate == 128)) {
        //Serial.println("R stop");
        robotR.motor_forward(0,0);
      } else if (RYstate < 127) {
        speed = 255 - RYstate;
        robotR.motor_forward(0,speed);
      } else {
        speed = RYstate;
        robotR.motor_back(0,speed);
      }
    }
  }
  delay(50);
}

void clearEyeTimer() {
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  eyeMove = false;
  myservo.detach();
}

/*
ISR(TIMER3_COMPA_vect) {
  eyePos = eyePos + 10 * eyeDir;
  if  ((eyePos < 40) || (eyePos > 140)) {
    eyeDir = eyeDir * -1;
    eyePos = eyePos + 10 * eyeDir;
  }
  myservo.write(eyePos);
}
*/

void setEyeTimer() {
  clearEyeTimer();  
  OCR3A = 1562;            // compare match register 16MHz/256/nHz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS10) | (1 << CS12) ;    // 1024 prescaler 
  TIMSK3 |= (1 << OCIE1A);  // enable timer compare inte
  myservo.attach(EYE);
}

