/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef BTSTEER_HPP
#define BTSTEER_HPP

#include "BtDriver/BtCom.hpp"
#include "BtDriver/BtMotor.hpp"

#include <Servo.h>

class BtSteer : public BtCom
{
public:
	BtSteer(BtMotor *motor, Servo *servo, float limit);
	
#ifdef MEETANDROID
	void PhoneOrient(MeetAndroid *android);
	void PhoneCompass(MeetAndroid *android);
#endif

	void Serial(char type, int value);
	
private:
	Servo *servo;
	BtMotor *motor;
	float limits[3];
};

#endif // BTSTEER_HPP