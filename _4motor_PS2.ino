#include <robocraft.h>
#include <NewPing.h>
#include <Servo.h>
#include <PS2X_lib.h>  //for v1.6

#define M_PI 3.1415926535897932384

#include "wheelbase.h"
#include "controler.h"
#include "eyes.h"

void setup()
{
  // Enable debug console
  Serial.begin(57600);

  // pinMode(13, OUTPUT);
  // digitalWrite(13, HIGH);

  MotorStop();

  // set servo position to middle
  myservo.attach(EYE);
  eyePos = BASE;
  myservo.write(eyePos);
  // delay(1000);
  // myservo.detach();

  initController();
}

bool turned = false;

void loop()
{
	MotorUpdate(updateController());
	delay(50);

	// turn left
	if (!turned) {
		delay(2000);
		turnRight(M_PI / 2);
		delay(2000);
		turnLeft(M_PI / 2);
		delay(2000);
		turnRight(M_PI);
		turned = true;
	}
}


JOY_DIRECTION turnLeftDir = {-255, 255};
JOY_DIRECTION turnRightDir = {255, -255};
JOY_DIRECTION turnStopDir = {0, 0};
int turnTimeForPiAngle = 1000; // time to turn into Pi angle in miliseconds


void turnLeft(float angle) {
	MotorUpdate(turnLeftDir);
	int delayInterval = (int) turnTimeForPiAngle / M_PI * angle;
	delay(delayInterval);
	MotorUpdate(turnStopDir);
}

void turnRight(float angle) {
	MotorUpdate(turnRightDir);
	int delayInterval = (int) turnTimeForPiAngle / M_PI * angle;
	delay(delayInterval);
	MotorUpdate(turnStopDir);
}


/*
void clearEyeTimer() {
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  eyeMove = false;
  myservo.detach();
}*/

/*
void setEyeTimer() {
  clearEyeTimer();  
  OCR3A = 1562;            // compare match register 16MHz/256/nHz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS10) | (1 << CS12) ;    // 1024 prescaler 
  TIMSK3 |= (1 << OCIE1A);  // enable timer compare inte
  myservo.attach(EYE);
}
*/
