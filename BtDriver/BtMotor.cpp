/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "BtDriver/BtMotor.hpp"

BtMotor::BtMotor(const byte *motors, const byte motorCount, const byte *logicForward, const byte *logicBackward, 
		const byte *logicLeft, const byte *logicRight, const byte *logicStop) : motors(motors), motorCount(motorCount), logicForward(logicForward),
			logicBackward(logicBackward), logicLeft(logicLeft), logicRight(logicRight), logicStop(logicStop), locked(false)
{ 
	for (int i = 0; i < motorCount; i++)
	{
		pinMode(motors[i], OUTPUT); 
	}
	
	pinMode(motors[motorCount], OUTPUT);
	pinMode(motors[motorCount + 1], OUTPUT);
}

void BtMotor::forward()
{
	steer(logicForward);
}

void BtMotor::backward()
{
	steer(logicBackward);
}

void BtMotor::left()
{
	steer(logicLeft);
}

void BtMotor::right()
{
	steer(logicRight);
}

void BtMotor::stop()
{
	steer(logicStop);
}

void BtMotor::lock()
{
	locked = true;
}

void BtMotor::release()
{
	locked = false;
}

boolean BtMotor::isLocked()
{
	return locked;
}

void BtMotor::steer(const byte *dir)
{
	if (locked)
	{
		return;
	}

	for (int i = 0; i < motorCount; i++)
	{
		digitalWrite(motors[i], dir[i]);
	}
	
	analogWrite(motors[motorCount], dir[motorCount]);
	analogWrite(motors[motorCount + 1], dir[motorCount + 1]);
}