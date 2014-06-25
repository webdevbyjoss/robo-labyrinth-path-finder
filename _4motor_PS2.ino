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
int turnTimeForPiAngle = 3000; // time to turn into Pi angle in miliseconds
int reactionDistance = 40; // distance in centimeters to stop before wall

void turn(float angle, bool turnRight) {
	if (turnRight) {
		MotorUpdate(turnRightDir);
	} else {
		MotorUpdate(turnLeftDir);
	}
    int delayInterval = (int) turnTimeForPiAngle / M_PI * angle;
    delay(delayInterval);
    MotorUpdate(turnStopDir);
}

// servo controls
int di = 10;
int m = 40;

void setup()
{
  // Enable debug console
  Serial.begin(9600);

  MotorStop();

  // set servo position to middle
  myservo.attach(EYE);
  eyePos = BASE;
  myservo.write(eyePos);

  initController();
}

// general counter
int i = 0;

// TODO: investigate the problem with joystick
// for some reason after we move servo, the joystick reports active state
// and autopilot mode becomes disabled
// we should skip this initial state and give control to joystick
// only if it is reported as active for second time
int skipJoyFix = 0;

void loop()
{
	delay(50);

	// read the movement state from Joystick as highest priority
	jState = updateController();
	Serial.print("Joy position: ");
	Serial.print(jState.left);
	Serial.print(":");
	Serial.print(jState.right);

	// do not do anything if user takes control with joystick
	if (jState.left || jState.right) {
		if (skipJoyFix == 1) {
			skipJoyFix = 0;
			return;
		}
		MotorUpdate(jState);
		Serial.println(" - Joystick active");
		// set last time when joystick was active
		startTime = millis();
		return;
	}

	skipJoyFix = 0;

	// STOP after joystick becomes inactive
	MotorUpdate(jState);

	// add 2 seconds delay before turning on auto-pilot
	endTime = millis();
	if (endTime - startTime < AUTO_PILOT_TIMEOUT) {
		Serial.print(" - pause...");
		Serial.println(endTime - startTime);
		return;
	}

	// auto-pilot mode
	m = eyeGetDistance();
	Serial.print(" - Auto-pilot. Ping: ");
	Serial.print(m);
	Serial.print("cm\t Angle: ");
	Serial.println(i);

	// move to the neares wall and stop before it
	if (m == 0 || m >= reactionDistance) {
		jState.left = 255;
		jState.right = 255;
		MotorUpdate(jState);

	// look around and turn into direction where there is more space available
	} else {
		// stop the machine
		jState.left = 0;
		jState.right = 0;
		MotorUpdate(jState);

		// detect the direction by looking with uv-sensor to the right and to the left
		// robot will move into the direction where we have more free space
		myservo.write(BASE + 70);
		delay(500);
		int mleft = eyeGetDistance();
		// TODO: investigate situation when uv-sensor returns zero for some reason
		if (mleft == 0) {
			mleft = eyeGetDistance();
		}
		if (mleft == 0) {
			mleft = eyeGetDistance();
		}
		delay(50);

		myservo.write(BASE - 70);
		delay(500);
		int mright = eyeGetDistance();
		if (mright == 0) {
			mright = eyeGetDistance();
		}
		if (mright == 0) {
			mright = eyeGetDistance();
		}
		delay(50);

		// return eye to initial position
		myservo.write(BASE);

		Serial.print("Left: ");
		Serial.print(mleft);
		Serial.print("cm, Right: ");
		Serial.print(mright);
		Serial.println("cm");

		// turn side to find new way
		if (mleft < mright) {
			Serial.println("Turn right");
			turn(M_PI / 5, 1);
		} else {
			Serial.println("Turn left");
			turn(M_PI / 5, 0);
		}
		m = 40;

		// eliminate the autopilot timeout, so that after robot turns it can start
		// moving forward in autopilot mode immediately
		startTime = millis() - AUTO_PILOT_TIMEOUT;
		skipJoyFix = 1;
	}
}
