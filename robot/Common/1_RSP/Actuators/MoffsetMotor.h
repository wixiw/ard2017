/*
 * MoffsetMotor.h
 *
 *  Created on: 24 mai 2017
 *      Author: lambert.w
 */

#ifndef ROBOT_COMMON_1_RSP_ACTUATORS_MOFFSETMOTOR_H_
#define ROBOT_COMMON_1_RSP_ACTUATORS_MOFFSETMOTOR_H_

#include <stdint.h>

namespace ard {

class MoffsetMotor {
public:
	MoffsetMotor(uint8_t pin);

	//Turn at full speed when on is true
	void turn(bool on);

protected:
	uint8_t pin;
};

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ACTUATORS_MOFFSETMOTOR_H_ */
