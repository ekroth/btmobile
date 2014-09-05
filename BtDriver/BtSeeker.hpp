/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef BTSEEKER_HPP
#define BTSEEKER_HPP

#include "BtDriver/BtCom.hpp"
#include "BtDriver/BtMotor.hpp"

#include <Ultrasonic/Ultrasonic.h>
#include <Servo.h>

class BtSeeker : public BtCom
{
public:
	BtSeeker(BtMotor *motor, Servo *servo, Ultrasonic *sonic);
	void Update();
private:
	enum Status
	{
		Status_TurningLeft,
		Status_TurningRight,
		Status_Backward,
		Status_Forward
	};
	
	enum Reason
	{
		Reason_None,
		Reason_Obstacle
	};

	BtMotor *motor;
	Servo *servo;
	Ultrasonic *sonic;
	Status lastStatus;
	Status status;
	unsigned int distances[3];
	byte servoVals[3];
	unsigned long timer;

	const unsigned int minDistance;
	const unsigned int rangeIntervalMillis;

	const unsigned int ultraWaitMillis;
	byte currUltraPos;

	const unsigned int moveWaitMillis;
};

#endif // BTSEEKER_HPP
