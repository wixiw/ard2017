/*
 * common.h
 *
 *  Created on: 28 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_UT_ACTUATORS_COMMON_H_
#define ROBOT_UT_ACTUATORS_COMMON_H_

#include "BSP.hpp"

using namespace ard;

ArdUART serial0(UART, ID_UART, SERIAL_BUF_SIZE /*RX bvuf size*/, SERIAL_BUF_SIZE /*TX bvuf size*/);


#endif /* ROBOT_UT_ACTUATORS_COMMON_H_ */
