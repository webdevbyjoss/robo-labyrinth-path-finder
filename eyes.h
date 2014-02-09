/*
 * eyes.h
 *
 *  Created on: Feb 9, 2014
 *      Author: joseph
 */

#ifndef EYES_H_
#define EYES_H_

/* Init servos */
Servo myservo;
#define EYE 8
#define BASE 82
boolean eyeMove;
int eyePos;
int eyeDir = 1;


/* Init sonar */
#define TRIGGER_PIN  30  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     31  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned int uS;


#endif /* EYES_H_ */
