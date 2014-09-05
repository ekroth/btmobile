/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef BTCOM_HPP
#define BTCOM_HPP

#include <WProgram.h>

class BtCom
{
public:
	virtual void Startup() { }
#ifdef MEETANDROID
	virtual void PhoneOrient(MeetAndroid *android) { }
	virtual void PhoneCompass(MeetAndroid *android) { }
#endif
	virtual void Update() { }
	virtual void Serial(char type, int value) { }
};

#endif // BTCOM_HPP