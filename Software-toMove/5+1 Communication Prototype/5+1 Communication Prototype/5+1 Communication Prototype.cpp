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

#include "Servo.h"

//Defines
#define ID "`0003"

#define PISTON_PIN 7 //pin to raise/lower piston. 
#define OPEN_PIN 5 //PWM servo pin to open/close faces
#define OPEN_POS 1 //degrees to send open servo for open
#define CLOSED_POS 180 //degrees to send open servo for close
#define START_PIN 9 //start button pin. Pulled from high to low when pressed.
#define RAISED_PIN A0 //pin to detect if arm is in raised position. must be set as INPUT_PULLUP, pulls LOW when raised.
#define RED_PIN 4
#define GREEN_PIN 6

#define SELECT_0 12 //LSB of select logic
#define SELECT_1 11 //middle bit of select logic
#define SELECT_2 10 //MSB of select logic


#define CAPTURE_PIN 8 //input capture pin
#define INTERRUPT_PIN 2 //interrupt pin

#define FAIL_THRESHOLD 100 //total number of individual connection failures allowed

Servo open;

long failCount = 0;
volatile int pinState = LOW;

void interruptServiceRoutine(void);

void setFace(unsigned int face);

void startTest(void);

void handleError(void);

enum State
{
	IDLE,
	WORKING,
	ERROR
	};
	
State stateVar = IDLE;

enum Result {UNKNOWN, FAIL, PASS};
Result testResult = UNKNOWN; //default unknown result

enum Error {NONE, RAISE_FAIL, LOWER_FAIL,};
Error errorVar = NONE;

int actionResponse(String inString);

CubeletsShield cubelets(true, 13);

void setup()
{
	Serial.begin(9600);
	Serial.println(ID);
	pinMode(PISTON_PIN, OUTPUT);
	pinMode(START_PIN, INPUT);
	pinMode(INTERRUPT_PIN, INPUT);
	pinMode(CAPTURE_PIN, INPUT);
	pinMode(SELECT_0, OUTPUT);
	pinMode(SELECT_1, OUTPUT);
	pinMode(SELECT_2, OUTPUT);
	pinMode(RAISED_PIN, INPUT_PULLUP);
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	//open.attach(OPEN_PIN);
	//select face 1 for power whatever
	cubelets.initialize();
	digitalWrite(SELECT_0, LOW);
	digitalWrite(SELECT_1, HIGH);
	digitalWrite(SELECT_2, HIGH);
	digitalWrite(PISTON_PIN, HIGH); //raise piston when idle
	int startTime = millis();
	while(digitalRead(RAISED_PIN))
	{
		if((millis() - startTime) >= 2000)
		{
			errorVar = RAISE_FAIL;
			stateVar = ERROR;
			handleError();
			break;
		}
}
	open.attach(OPEN_PIN);
	open.write(OPEN_POS); //open faces
	delay(1000);
	open.detach();
	digitalWrite(GREEN_PIN, HIGH);
	Serial.println("RDY");
}

void loop()
{
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
	
	//button check
	if(!digitalRead(START_PIN))
	{
		if(stateVar == IDLE)
		{
			Serial.println("GO");
			stateVar = WORKING; //set next state to working
			startTest();
		}

	}
	
	//while(digitalRead(START_PIN)); //wait for start button to be pressed
	//Serial.println("GO");
}

void interruptServiceRoutine(void)
{
			failCount++;	
}

void setFace(unsigned int face)
{
	switch(face)
	{
		case 1:
		{
			digitalWrite(SELECT_0, LOW);
			digitalWrite(SELECT_1, LOW);
			digitalWrite(SELECT_2, LOW);
		} break;
		case 2:
		{
			digitalWrite(SELECT_0, LOW);
			digitalWrite(SELECT_1, LOW);
			digitalWrite(SELECT_2, HIGH);
		} break;
		case 3:
		{
			digitalWrite(SELECT_0, LOW);
			digitalWrite(SELECT_1, HIGH);
			digitalWrite(SELECT_2, LOW);
		} break;
		case 4:
		{
			digitalWrite(SELECT_0, LOW);
			digitalWrite(SELECT_1, HIGH);
			digitalWrite(SELECT_2, HIGH);
		} break;		
	}
}

int actionResponse(String inString) //act on serial commands and generate response
{
	//lots of ifs since can't switch Strings
	/*if(stateVar == ERROR);
	{
		Serial.println("ERROR"); //only do this. requires manual reset to clear error.
		return 0;
	}*/
	
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
		//Serial.println("State, Config, subState:");
		Serial.println(stateVar);
		return 0;
	}
};

void startTest(void)
{
	testResult = UNKNOWN;
	digitalWrite(RED_PIN, LOW);
	digitalWrite(GREEN_PIN, LOW); //clear both pins when result is unknown
	cubelets.stopAllCubeletCommunication();
	cubelets.greenOffAllCubelets();
	
	delay(50);
		
	digitalWrite(PISTON_PIN, LOW); //lower piston over cubelet
		
	long startTime = millis();
	while(!digitalRead(RAISED_PIN)) //wait for arm raised sensor to go HIGH, indicating that arm has dropped 
	{
		if((millis() - startTime) >= 2000) //if it doesn't drop in 2 seconds, something is wrong
		{
			stateVar = ERROR; //error handling here
			errorVar = LOWER_FAIL;
			handleError();
			return;
		}
	}
	
	delay(1000); //still want to wait a second to pass the drop		
		
	open.attach(OPEN_PIN);
	
	open.write(CLOSED_POS); //close faces
	
	delay(1000); //wait for faces to close
	
	open.detach();
	
	unsigned char testFace[5] = {2, 3, 4, 5, 1};
	
	delay(500); //response
	
	for(unsigned char face = 1; face <= 5; face++)
	{				 
// 		Serial.print("Starting comm on face ");
// 		Serial.println(face);
		cubelets.startCommunicationByFace(face);
		delay(1000);
// 		Serial.print("Setting face ");
// 		Serial.print(face);
// 		Serial.print(" to ");
// 		Serial.println(51*face);		
		while(cubelets.setBroadcastBlockValueByFace(face, 51*face) != 0); //keep trying until returns zero
/*		Serial.println("Set.");*/
		//cubelets.setBroadcastBlockValueByFace(face, 51*face);

		cubelets.greenOnByFace(face);
		
		delay(1000); //time for response
		
// 		Serial.print("Starting comm on face ");
// 		Serial.println(testFace[face-1]);
		cubelets.startCommunicationByFace(testFace[face-1]);
		delay(1000);
// 		unsigned char faceReading;
// 		do 
// 		{
// 			faceReading = cubelets.getNeighborBlockValueByFace(testFace[face-1]);
// 			delay(100);
// 		} while (faceReading != false);
		unsigned char faceReading = cubelets.getNeighborBlockValueByFace(testFace[face-1]);
		//delay(500); //time for response
	
//   		Serial.print("Face ");
//   		Serial.print(testFace[face-1]);
//   		Serial.print(" reads ");
//   		Serial.println(faceReading);

		//check that number was set correctly...set tests input, check tests output.
		if(faceReading != 51*face)
		{
			/*Serial.println("failing");*/
			testResult = FAIL;
			break; //ends test as soon as a failure is found
		}
		//testing done.
		//cubelets.setBroadcastBlockValueByFace(face, 0);
		//delay(1000);
		//delay(500);
/*		Serial.println("Stopping all communication");*/
		cubelets.stopAllCubeletCommunication();
		delay(500);
		cubelets.greenOffByFace(face);
		//delay(500); //response
	}
	
	open.attach(OPEN_PIN);
	open.write(OPEN_POS); //open faces
	delay(1000); //wait for faces to open
	open.detach();
	
	digitalWrite(PISTON_PIN, HIGH); //raise arm
	
	startTime = millis();
	while(digitalRead(RAISED_PIN)) //wait for arm to raise
	{
		if((millis() - startTime) >= 2000) //if arm doesn't raise in 2 seconds, throw error
		{

			errorVar = RAISE_FAIL;
			stateVar = ERROR;
			handleError();
			return;
		}
	}
	//Serial.print("Failures: ");
	//Serial.println(failCount);
	if(testResult != FAIL)
	{
		testResult = PASS;
		Serial.println("PASS");
		digitalWrite(GREEN_PIN, HIGH);
	}
	else
	{
		Serial.println("FAIL");
		digitalWrite(RED_PIN, HIGH);
	}
	delay(1000); //give it a sec.
	stateVar = IDLE;
};

void handleError(void)
{
	open.detach(); //detach servos
	
	Serial.print("ERROR: ");
	switch(errorVar)
	{
		case NONE:
		Serial.println("NONE?");
		break;
		
		case LOWER_FAIL:
		Serial.println("LOWER_FAIL");
		break;
		
		case RAISE_FAIL:
		Serial.println("RAISE_FAIL");
		break;
		
		default:
		Serial.println("UNKNOWN");
	}
	while(true) //loop forever to require manual reset
	{
		digitalWrite(RED_PIN, HIGH);
		delay(500);
		digitalWrite(RED_PIN, LOW);
		delay(500);
	}
	
};