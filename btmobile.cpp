/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "BtDriver/BtDriver.hpp"

#include <Ultrasonic/Ultrasonic.h>
#include <Servo.h> 
#include <stdlib.h>
#include <WProgram.h>
#include <Debug/Debug.hpp>

#ifdef MEETANDROID
#include "MeetAndroid.h"
MeetAndroid meetAndroid;
#endif

/* Serial
*/

char serialBuffer[16];
char serialType;
char serialStatus;
int serialReceived;

/*
PWM10 och (?) PWM11 funkar inte?
*/

/*

Size info

Batteries

10x6x3,5 cm


*/

#define MAX_MOTOR_SPEED 255

/* L298N

INA - 13
INB - 12
INC - 4
IND - 2

PWMA - 11
PWMB - 6

OUT1 - Röd, vit
OUT2 - Orange, Grön

*/

// Last two pwm
const byte motors[] = { 
  13, 12, 4, 2, 6, 11 };
const byte motorCount = 4;

// Non-const debug
const byte dirBack[] = { 
  HIGH, LOW, LOW, HIGH, MAX_MOTOR_SPEED, MAX_MOTOR_SPEED };
const byte dirForw[] = {
  LOW, HIGH, HIGH , LOW, MAX_MOTOR_SPEED, MAX_MOTOR_SPEED };
const byte dirRight[] = { 
  HIGH, LOW, HIGH, LOW, MAX_MOTOR_SPEED, MAX_MOTOR_SPEED };
const byte dirLeft[] = { 
  LOW, HIGH, LOW, HIGH, MAX_MOTOR_SPEED, MAX_MOTOR_SPEED };
const byte dirStop[] = { 
  LOW, LOW, LOW, LOW, MAX_MOTOR_SPEED, MAX_MOTOR_SPEED };
  
BtMotor btMotor(motors, motorCount, dirForw, dirBack, dirLeft, dirRight, dirStop);

/* Ultrasonic
 Red - VCC
 Yellow - Trig
 White - Echo
 Black - GND
 */

Ultrasonic sonic(10, 9); // (TP, EP)

/* Servo
Signal - 5
 */

Servo servo;

BtSteer btSteer(&btMotor, &servo, 25.0f);
BtSeeker btSeeker(&btMotor, &servo, &sonic);

const byte steerCount = 2;
BtCom *steerers[steerCount] = {
  &btSteer, &btSeeker };
  
byte currSteerer = 1;

void readSerial();

void setup()
{
#ifdef MEETANDROID
	Serial.begin(115200);
    meetAndroid.registerFunction(phoneOrientation, 'A');
    meetAndroid.registerFunction(phoneCompass, 'B');
    
    meetAndroid.registerFunction(lockMotor, 'X');
    meetAndroid.registerFunction(setSteer, 'Z');
    meetAndroid.registerFunction(motorSpeed, 'S');
    
    // BtSCompass
    meetAndroid.registerFunction(BtSCompass_Limit, 'a');
    meetAndroid.registerFunction(BtSCompass_Direction, 'b');
#else
	Serial.begin(9600);
#endif
    servo.attach(5);
    serialBuffer[0] = '\0';
    serialType = '\0';
    serialStatus= 0;
    serialReceived = 0;
    
    writeln("Startup");
}

void loop() 
{
#ifdef MEETANDROID
    meetAndroid.receive();
#else
    readSerial();
#endif
    steerers[currSteerer]->Update();
}


// Serial
#ifndef MEETANDROID
void readSerial()
{    
    while (Serial.available() > 0)
    {
        char val = Serial.read();
        
        if (val == '\'')
        {
           // Start/End
           if (serialStatus == 0)
           {
               serialStatus = 1;
           }
           else
           {
              // End message 
              // Received something
               int value = atoi(serialBuffer);
               Serial.println(">>> Info");
               Serial.println("* Bytes");
               Serial.println(serialReceived, DEC);
               Serial.println("* Buffer");
               Serial.println(serialBuffer);
               Serial.println("* Type");
               Serial.println(serialType);
               Serial.println("* Value");
               Serial.println(value, DEC);
               Serial.println("<<< Info");
               
               steerers[currSteerer]->Serial(serialType, value);
               
               serialBuffer[0] = '\0';
               serialType = '\0';
               serialStatus = 0;
               serialReceived = 0;
           }
           continue;
        }
        
        if (serialType == '\0')
        {
           serialType = val;
           continue; 
        }
        
        serialBuffer[serialReceived] = val;
        serialReceived++;
        serialBuffer[serialReceived] = '\0';
    }
}
#endif

// Overall data
#ifdef MEETANDROID
void lockMotor(byte flag, byte numOfValues)
{
    if (btMotor.isLocked())
    {
        btMotor.release();
    }
    else
    {
        btMotor.lock();
    }
}

void setSteer(byte flag, byte numOfValues)
{
    const int steer = meetAndroid.getInt();
    
    if (steer < steerCount)
    {
       currSteerer = steer; 
    }
    
    // If same as current, restart it
    steerers[currSteerer]->startup();
}

// Phone information stuff

void phoneCompass(byte flag, byte numOfValues)
{
    steerers[currSteerer]->phoneCompass(&meetAndroid);
}

void phoneOrientation(byte flag, byte numOfValues)
{
    steerers[currSteerer]->phoneOrient(&meetAndroid);
}

#endif

