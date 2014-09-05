/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "BtDriver/BtSeeker.hpp"

#include <Debug/Debug.hpp>

BtSeeker::BtSeeker(BtMotor *motor, Servo *servo, Ultrasonic *sonic) : motor(motor), servo(servo), sonic(sonic), status(Status_Checking), 
	timer(0), minDistance(22), rangeIntervalMillis(50), ultraWaitMillis(670), currUltraPos(0), moveWaitMillis(2500)
{ 
	distances[0] = 0;
	distances[1] = 0;
	distances[2] = 0;
	servoVals[0] = 30;
	servoVals[1] = 90;
	servoVals[2] = 150;
}

void BtSeeker::Update()
{
	switch (status)
	{
		case Status_Forward:
		// Moving forward, check range in intervals.
		// TODO: 	Also move servo when checking.
		// 			Check if range changes (when in "working" range)
		if (timer == 0)
		{
			timer = millis();
		}
		else if (millis() - timer > rangeIntervalMillis)
		{
			//writeon(" * ");
			if (sonic->Ranging(CM) < minDistance)
			{
				motor->stop();
				status = Status_Checking;
				timer = 0;
				writeln("\nStopping while moving, obstacle. -> status_Checking");
			}
			else
			{
				timer = millis();
			}
		}
		break;

		case Status_Checking:
		if (timer != 0)
		{
			if (millis() - timer > ultraWaitMillis)
			{
				writeln("Reading distance.");
				distances[currUltraPos] = sonic->Ranging(CM);

				if (currUltraPos == 2) // Last pos
				{
					status = Status_Moving;

					if (distances[1] > minDistance)
					{
						writeln("(First check) Forward is okay. -> status_Forward");
						status = Status_Forward;
					}
					else if ((distances[0] + distances[1] + distances[2]) / 3 < minDistance)
					{
						motor->backward();
						writeln("Moving backwards, all distances too close");
					}
					else
					{
						byte lowDistIndex = 0;

						if (distances[1] > distances[lowDistIndex])
						{
							lowDistIndex = 1;
						}

						if (distances[2] > distances[lowDistIndex])
						{
							lowDistIndex = 2;
						}

						switch (lowDistIndex)
						{
							case 0:
							motor->left();
							writeln("Moving left");
							break;

							case 1:
							status = Status_Forward;
							writeln("(Var check) Moving forward. ->status_Forward");
							break;
		
							case 2:
							motor->right();
							writeln("Moving right");
							break;
						};
					}

					currUltraPos = 0;
					servo->write(servoVals[1]);
					timer = 0;

					if (status == Status_Forward)
					{
						// Workaround to let servo move before going forward
						servo->write(servoVals[1]);
						writeln("Delay to let servo move before moving forward.");
						delay(ultraWaitMillis);
						writeln("Delay finished, moving forward.");
						motor->forward();
					}
				}
				else
				{
					currUltraPos++;
					servo->write(servoVals[currUltraPos]);
					writeln("Moving servo to next position.");
					timer = millis();
				}
			}
		}
		else
		{
			writeln("Checking started");
			servo->write(servoVals[0]);
			timer = millis();
			currUltraPos = 0;
		}
		
		break;

		case Status_Moving:
		if (timer == 0)
		{
			timer = millis();
		}
		else if (millis() - timer > moveWaitMillis)
		{
			writeln("Finished moving. -> status_Checking");
			motor->stop();
			timer = 0;
			status = Status_Checking;
		}
		break;
	};
}
