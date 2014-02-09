/*
 * wheelbase.c
 *
 *  Created on: Feb 9, 2014
 *      Author: joseph
 */
#ifndef WHEELS_H_
#define WHEELS_H_

#include <robocraft.h>
#include "controler.h"

#define STOP 0
#define RUN 1

void MotorStop();
void MotorUpdate(JOY_DIRECTION joyDir);
void MotorMove(RoboCraft motor, int direction);

#endif /* WHEELS_H_ */
