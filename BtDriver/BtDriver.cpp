/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "BtDriver.h"

BtSCompass::BtSCompass(int direction, BtMotor *motor, int limit) : direction(direction), motor(motor), limit(limit)
{ }

void BtSCompass::setDireciton(int dir)
{
	direction = dir;
}

void BtSCompass::setLimit(int limit)
{
	this->limit = limit;
}

#ifdef MEETANDROID
void BtSCompass::PhoneCompass(MeetAndroid *android)
{
	const int comp = android->getInt();
	int curr = comp - direction;
	curr = abs(curr);

	if (curr > limit)
	{
		// Must move
		if (359 - curr < curr)
		{
			motor->right();
		}
		else
		{
			motor->left();
		}
	}
	else
	{
		motor->forward();
	}
}
#endif
void BtSCompass::Serial(char type, int value)
{
	direction = value;
}