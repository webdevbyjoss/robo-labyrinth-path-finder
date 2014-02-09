/*
 * controler.h
 *
 *  Created on: Feb 9, 2014
 *      Author: joseph
 */
#include <PS2X_lib.h>  //for v1.6

#ifndef CONTROLER_H_
#define CONTROLER_H_

// speed for left and right wheels
// -127...-1 - back, 0 - stop, 1..127 - forward
struct JOY_DIRECTION
{
  int left;
  int right;
};

void initController();
JOY_DIRECTION updateController();

#endif /* CONTROLER_H_ */
