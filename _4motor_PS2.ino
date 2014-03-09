#include <robocraft.h>
#include <NewPing.h>
#include <Servo.h>
#include <PS2X_lib.h>  //for v1.6

#include "wheelbase.h"
#include "controler.h"
#include "eyes.h"

#define M_PI 3.1415926535897932384

unsigned long startTime;
unsigned long endTime;
JOY_DIRECTION jState;

#define AUTO_PILOT_TIMEOUT 3000 // in miliseconds before auto-pilot starts

JOY_DIRECTION turnLeftDir = {-255, 255};
JOY_DIRECTION turnRightDir = {255, -255};
JOY_DIRECTION turnStopDir = {0, 0};
int turnTimeForPiAngle = 1000; // time to turn into Pi angle in miliseconds

void turn(float angle, bool turnRight) {
	if (turnRight) {
		MotorUpdate(turnLeftDir);
	} else {
		MotorUpdate(turnRightDir);
	}
    int delayInterval = (int) turnTimeForPiAngle / M_PI * angle;
    delay(delayInterval);
    MotorUpdate(turnStopDir);
}

// servo controls
int i = 1;
int di = 10;
int m = 40;

void setup()
{
  // Enable debug console
  Serial.begin(9600);

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

void loop()
{
	delay(50);

	// read the movement state from Joystick as highest priority
	jState = updateController();
	Serial.print("Joy position: ");
	Serial.print(jState.left);
	Serial.print(":");
	Serial.print(jState.right);

	if (jState.left || jState.right) {
		MotorUpdate(jState);


		startTime = millis();

		// reset eyes and sensor
		myservo.write(eyePos);
		i = eyePos;
		m = 40;

		Serial.println(" - Joystick active");
		return; // do not do anything if user takes control with joystick
	}

	// STOP after joystick becomes inactive
	jState.left = 0;
	jState.right = 0;
	MotorUpdate(jState);

	// add 2 seconds delay before turning on auto-pilot
	endTime = millis();
	if (endTime - startTime < AUTO_PILOT_TIMEOUT) {
		Serial.print(" - pause...");
		Serial.println(endTime - startTime);
		return;
	}

	// auto-pilot mode

	// turn servo
	/*
	if (i > 180 || i < 0) {
		di = -1 * di;
	}
	i = i + di;
	myservo.write(i);
	*/


	m = eyeGetDistance();
	Serial.print(" - Auto-pilot. Ping: ");
	Serial.print(m);
	Serial.print("cm\t Angle: ");
	Serial.println(i);

	// move to the nearest wall and stop 20cm before it
	if (m == 0 || m >= 30) {
		jState.left = 255;
		jState.right = 255;
		MotorUpdate(jState);
	} else {
		// turn side to find new way
		jState.left = 0;
		jState.right = 0;
		MotorUpdate(jState);

		turn(M_PI / 4, true);
		delay(200);
		m = 40;
	}
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
