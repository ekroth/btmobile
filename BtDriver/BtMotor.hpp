/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef BTMOTOR_HPP
#define BTMOTOR_HPP

#include <WProgram.h>

class BtMotor
{
public:
	BtMotor(const byte *motors, const byte motorCount, const byte *logicForward, const byte *logicBackward, 
		const byte *logicLeft, const byte *logicRight, const byte *logicStop);

	void forward();
	void backward();
	void left();
	void right();
	void stop();

	void lock();
	void release();
	boolean isLocked();
private:
	void steer(const byte *dir);

	const byte *motors;
	const byte motorCount;
	
	const byte *logicForward;
	const byte *logicBackward;
	const byte *logicLeft;
	const byte *logicRight;
	const byte *logicStop;

	boolean locked;
};

#endif // BTMOTOR_HPP