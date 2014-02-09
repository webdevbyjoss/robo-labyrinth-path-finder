/*
 * controler.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: joseph
 */
#include "controler.h"

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
JOY_DIRECTION joyDir = {0, 0};

unsigned int speed = 0;
unsigned int LYstate = 127;
unsigned int RYstate = 127;

void initController() {

	//GamePad(clock, command, attention, data, Pressures?, Rumble?)
	PS2error = ps2x.config_gamepad(41, 39, 38, 40, true, true);

	// check for error
	if (PS2error == 0) {
		Serial.println("Found Controller, configured successful");
		Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
		Serial.println("holding L1 or R1 will print out the analog stick values.");
		Serial.println("Go to www.billporter.info for updates and to report bugs.");
	} else if (PS2error == 1) {
		Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
	} else if (PS2error == 2) {
		Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
	} else if(PS2error == 3) {
		Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
	}

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

JOY_DIRECTION updateController() {

	if (PS2error == 1) { //skip loop if no controller found
		return joyDir;
	}

	if (PS2type == 2) { //Guitar Hero Controller
		return joyDir;
	}

	//DualShock Controller
	ps2x.read_gamepad(false, PS2vibrate); //read controller and set large motor to spin at 'vibrate' speed

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
		// robotL.motor_forward(0,0);
		joyDir.left = 0;
	  } else if (LYstate < 127) {
		speed = 255 - LYstate;
		// robotL.motor_forward(0,speed);
		joyDir.left = speed;
	  } else {
		speed = LYstate;
		// robotL.motor_back(0,speed);
		joyDir.left = -1 * speed;
	  }
	}

	if (RYstate != (unsigned char)ps2x.Analog(PSS_RY)) {
	  //Serial.println("Change R speed");
	  //Serial.print("Got ");
	  RYstate = (unsigned char)ps2x.Analog(PSS_RY);
	  //Serial.println(RYstate);
	  if ((RYstate == 127) || (RYstate == 128)) {
		//Serial.println("R stop");
		// robotR.motor_forward(0,0);
		joyDir.right = 0;
	  } else if (RYstate < 127) {
		speed = 255 - RYstate;
		// robotR.motor_forward(0,speed);
		joyDir.right = speed;
	  } else {
		speed = RYstate;
		// robotR.motor_back(0,speed);
		joyDir.right = -1 * speed;
	  }
	}

	/*
	Serial.print(joyDir.left); //Left stick, Y axis. Other options: LX, RY, RX
	Serial.print(",");
	Serial.println(joyDir.right);
	*/

	return joyDir;
}
