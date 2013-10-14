/*
 * _5_1_Communication_Prototype.cpp
 *
 * Created: 9/30/2013 2:53:20 AM
 *  Author: Nev
 */ 
#include "Arduino.h"
#include <string.h>
#include "CubeletsShield.h"

void setup();
void loop();

//#include "Servo.h"

//Defines
#define ID "`0007"

#define PISTON_PIN 7 //pin to raise/lower piston.
#define OPEN_PIN 5 //PWM servo pin to open/close faces
#define OPEN_POS 1 //degrees to send open servo for open
#define CLOSED_POS 180 //degrees to send open servo for close
#define START_PIN 9 //start button pin. Pulled from high to low when pressed.

#define SELECT_0 12 //LSB of select logic
#define SELECT_1 11 //middle bit of select logic
#define SELECT_2 10 //MSB of select logic


#define CAPTURE_PIN 8 //input capture pin

volatile int pinState = LOW;

void startTest(void);

enum State
{
	IDLE,
	WORKING
};

State stateVar = IDLE;

enum Result {UNKNOWN, FAIL, PASS};
Result testResult = UNKNOWN; //default unknown result

int actionResponse(String inString);

CubeletsShield cubelets(false, 13);

void setup()
{
	Serial.begin(9600);
	Serial.println(ID);
	pinMode(PISTON_PIN, OUTPUT);
	pinMode(START_PIN, INPUT);
	pinMode(CAPTURE_PIN, INPUT);
	//select face 1 for power whatever
	digitalWrite(SELECT_0, LOW);
	digitalWrite(SELECT_1, HIGH);
	digitalWrite(SELECT_2, HIGH);
	cubelets.initialize();
	delay(500);
	Serial.println("RDY");
}

void loop()
{
	digitalWrite(PISTON_PIN, HIGH); //raise piston when idle
	
	//serial check
	String inString;
	char inChar;
	while(Serial.available())
	{
		delay(2); //tiny delay is necessary to clock in serial...
		inChar = Serial.read(); //get one character from buffer
		inString.concat(inChar); //concat single character to holding string so we can pass it to actionResponse.
	}
	actionResponse(inString); //acts and responds to serial
}

int actionResponse(String inString) //act on serial commands and generate response
{
	//lots of ifs since can't switch Strings
	if(inString.equalsIgnoreCase("I"))
	{
		Serial.println(ID); //send ID to host
		return 0;
	}
	
	if(inString.equalsIgnoreCase("C0")) //configuration
	{
		if(stateVar == WORKING) //can't change configuration while working
		{
			Serial.println("WORKING");
			return 0;
		}
		else
		{
			Serial.println("C0");
			return 0;
		}

	}
	
	if(inString.equalsIgnoreCase("C1")) //configuration
	{
		if(stateVar == WORKING) //can't change configuration while working
		{
			Serial.println("WORKING");
			return 0;
		}
		else
		{
			Serial.println("C1");
			return 0;
		}
	}
	
	if(inString.equalsIgnoreCase("GO")) //init test command
	{
		if(stateVar == WORKING)
		{
			Serial.println("WORKING");
			return 0;
		}
		Serial.println("GO");
		stateVar = WORKING; //set next state to working
		startTest();
		//digitalWrite(GREEN_PIN, HIGH);
		return 0;
	}
	
	if(inString.equalsIgnoreCase("ESTOP"))
	{
		Serial.println("ESTOP");
		return 0;
	}
	
	if(inString.equalsIgnoreCase("RESULT"))
	{
		if(testResult == FAIL)
		{
			Serial.println("FAIL");
		}
		else if(testResult == PASS)
		{
			Serial.println("PASS");
		}
		else
		{
			Serial.println("UNKNOWN");
		}
	}
	
	if(inString.equalsIgnoreCase("STATE")) //status query
	{
		Serial.println("State, Config, subState:");
		Serial.println(stateVar);
		return 0;
	}
};

void startTest(void)
{
	for(int i = 0; i <= 70; i++)
	{
		cubelets.greenOnByFace(6);
		delay(100);
		cubelets.greenOffByFace(6);
		delay(100);	
	}
	
	stateVar = IDLE;
};