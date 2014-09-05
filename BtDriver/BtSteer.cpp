/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "BtDriver/BtSteer.hpp"

BtSteer::BtSteer(BtMotor *motor, Servo *servo, float limit) : motor(motor), servo(servo)
{ 
	for (int i = 0; i < 3; i++)
	{
		limits[i] = limit;
	}
}

#ifdef MEETANDROID
void BtSteer::PhoneOrient(MeetAndroid *android)
{
	float orient[3];
	android->getFloatValues(orient);

	if (orient[1] > limits[1])
	{
		motor->left(); 
	}
	else if (orient[1] < -limits[1])
	{
		motor->right();
	}
	else if (orient[2] < -limits[2])
	{
		motor->forward();
	}
	else if (orient[2] > limits[2])
	{
		motor->backward();
	}
	else
	{
		motor->stop();
	}
}

void BtSteer::PhoneCompass(MeetAndroid *android)
{
	int val = constrain(android->getInt(), 0, 179);

	servo->write(val);
}
#endif

void BtSteer::Serial(char type, int value)
{
	switch (type)
  {
  case 'l':
    motor->left();
    break;

  case 'r':
    motor->right();
    break; 

  case 'f':
    motor->forward();
    break;

  case 'b':
    motor->backward();
    break;

  case 's':
    motor->stop();
    break;

	case 'h':
	// Sensor
	servo->write(value);
	break;
  }
}