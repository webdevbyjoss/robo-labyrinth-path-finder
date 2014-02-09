/*
 * wheelbase.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: joseph
 */
#include "wheelbase.h"

MOTOR MOTORR1 = { 22, 23, 9 };
MOTOR MOTORR2 = { 24, 25, 10 };
MOTOR MOTORL1 = { 26, 27, 11 };
MOTOR MOTORL2 = { 28, 29, 12 };
RoboCraft robotL(&MOTORL1, &MOTORL2);
RoboCraft robotR(&MOTORR1, &MOTORR2);

void MotorStop() {
	// stop wheels
	robotL.motor_forward(0, 0);
	robotR.motor_forward(0, 0);
}

void MotorUpdate(JOY_DIRECTION joyDir) {
	MotorMove(robotL, joyDir.left);
	MotorMove(robotR, joyDir.right);
}

void MotorMove(RoboCraft motor, int direction) {

	if (direction < 0) {
		motor.motor_back(0, -1 * direction);
	} else if (direction > 0) {
		motor.motor_forward(0, direction);
	} else {
		motor.motor_forward(0, 0);
	}

}
