#include <robocraft.h>
#include <NewPing.h>
#include <Servo.h>
#include <PS2X_lib.h>  //for v1.6

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

void loop()
{
	JOY_DIRECTION dir = updateController();
	MotorUpdate(dir);
	delay(50);
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
