/*
 * Conveyance.cpp
 *
 * Created: 9/18/2013 2:29:00 PM
 *  Author: Nev
 */ 
#include "Arduino.h"
#include <String.h>
void setup();
void loop();

#define MOTORENABLE 3
#define MOTOR1 4
#define MOTOR2 5
#define MOTORPWM 9
#define BREAKBEAM 6
#define FWDBUTTON 7
#define REVBUTTON 8
#define LED 13

int actionResponse(string )

void setup()
{
	pinMode(MOTORENABLE, OUTPUT);
	pinMode(MOTOR1, OUTPUT);
	pinMode(MOTOR2, OUTPUT);
	pinMode(MOTORPWM, OUTPUT);
	pinMode(LED, OUTPUT);
	
	pinMode(BREAKBEAM, INPUT);
	pinMode(FWDBUTTON, INPUT);
	pinMode(REVBUTTON, INPUT);
	
	analogWrite(MOTORPWM, 255); //set motor speed to max
	
	Serial.being(9600);	
}

void loop()
{
	String inString;
	char inChar;
	while(Serial.available())
	{
		delay(2);
		inChar = Serial.read();
		inString.concat(inChar);
	}
	actionResponse(inString);
}