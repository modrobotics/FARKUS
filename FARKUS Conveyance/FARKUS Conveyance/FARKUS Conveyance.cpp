/*
 * FARKUS_Conveyance.cpp
 *
 * Created: 4/9/2013 11:09:00 AM
 *  Author: Nev
 */ 
#include "Arduino.h"
#include <String.h>

#define ID "`0000" //conveyance ID string

#define MOTORENABLE 3 //motor enable pin. pull high to enable.
#define MOTOR1 4 //H-bridge input 1. 1:HIGH; 2:LOW for forward...1:LOW; 2:high for reverse. both HIGH or LOW is brake.
#define MOTOR2 5 //H-bridge input 2
#define MOTORPWM 9 //PWM pin to control motor speed
#define BREAKBEAM 6 //breakbeam sensor for geneva mechanism. is HIGH during movement, LOW when in a stable position.
#define FWDBUTTON 7 //buttons for manual operation
#define REVBUTTON 8

#define OCCLUDE_TIME 500 //time in ms to allow the geneva to occlude the sensor before error.
#define CLEAR_TIME 500 //time in ms to allow the geneva to clear the sensor before error.

void setup();
void loop();

enum State
{
	IDLE,
	WORKING,
	ERROR,
	ESTOP
	};

enum SubState
{
	WAIT,
	ACT
	};

enum Config
{
	FORWARD,
	REVERSE
	};
	
int actionResponse(String inString);

State stateVar;
SubState subStateVar;
Config configVar;

long subWaitStart = 0; //holder for the start time of a WAIT substate
long subWaitTime; //this should be set before switching to a WAIT substate to define how long (ms) to wait
bool subWaitSentinel = true; //sentinel for keeping track of first wait cycles, since the start time of a wait must be set at the first cycle
long sensorTime; //holder to keep track of sensor timing for error checking
bool firstAct = true;
bool hasOccluded = false; //sets to true once sensor is occluded
bool hasCleared = false; //sets to true once sensor is cleared

void setup()
{
	Serial.begin(9600);
	Serial.println(ID);
	pinMode(MOTORENABLE, OUTPUT);
	pinMode(MOTOR1, OUTPUT);
	pinMode(MOTOR2, OUTPUT);
	pinMode(BREAKBEAM, INPUT);
	pinMode(FWDBUTTON, INPUT);
	pinMode(REVBUTTON, INPUT);
	pinMode(MOTORPWM, OUTPUT);
	//set motor speed
	analogWrite(MOTORPWM, 255);
	
	stateVar = IDLE;
	subStateVar = ACT;
	configVar = FORWARD;
	
	Serial.println("RDY"); //tell raspi that setup is complete and we're ready to rock
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
	if(digitalRead(FWDBUTTON))
	{
		if(stateVar==IDLE)
		{
			configVar = FORWARD;
			stateVar = WORKING;
		}
	}
	if(digitalRead(REVBUTTON))
	{
		if(stateVar==IDLE)
		{
			configVar = REVERSE;
			stateVar = WORKING;
		}
	}
	
	switch(stateVar) //main switch statement
	{
		case IDLE: //not doing anything.
		{
			//TODO: fill in idle
		} break;
		
		case WORKING: //time to convey
		{
			switch(configVar) //check if configured for fwd or rev
			{
				case FORWARD:
				{
					switch(subStateVar) //switch between wait and act
					{
						case WAIT: //for waitin'
						{
							if(subWaitSentinel)
							{
								subWaitStart = millis();
								subWaitSentinel = false;
							}
							
							if((millis() - subWaitStart) >= subWaitTime) //if wait time has passed
							{
								subStateVar = ACT; //switch back to ACT loop
								subWaitSentinel = true; //set sentinel back to true for next invocation of wait state
							}
						} break;
						
						case ACT: //for doin' stuff
						{
							digitalWrite(MOTORENABLE, HIGH);
							digitalWrite(MOTOR1, HIGH);
							digitalWrite(MOTOR2, LOW);
							
							if(firstAct) //if just starting the action state
							{
								sensorTime = millis(); //save current time
								firstAct = false;
							}
							
							if(hasOccluded)
							{
								if(!digitalRead(BREAKBEAM)) //is cleared again
								{
									//Serial.println("cleared");
									digitalWrite(MOTOR1, LOW); //done. stop motor.
									
									//reset stuff
									hasOccluded = false;
									firstAct = true;
									
									stateVar = IDLE; //go to idle state.						
								}
								else //has not yet cleared
								{
									if((millis() - sensorTime) >= CLEAR_TIME) //sensor has not cleared before timeout, indicating jam
									{
										Serial.println("ERROR:CLEAR_TIME");
										
										//reset stuff
										hasOccluded = false;
										firstAct = true;
										
										stateVar = ERROR; //probably jammed.
										
									}
								}
							}
							else //hasOccluded sentinel not yet set
							{
								if(digitalRead(BREAKBEAM)) //is occluded
								{
									//Serial.println("occluded");
									hasOccluded = true;
									sensorTime = millis(); //reset sensorTime to current time.
								}
								else
								{
									if((millis() - sensorTime) >= OCCLUDE_TIME) //check to see if time to occlude has taken longer than allowed
									{
										Serial.println("ERROR:OCCLUDE_TIME");
										
										//reset stuff
										hasOccluded = false;
										firstAct = true;
										
										stateVar = ERROR; //probably jammed.
										
									}
									//restart loop.
								}
							}
						} break;
					}
				} break;
				
				case REVERSE:
				{
					switch(subStateVar) //switch between wait and act
					{
						case WAIT: //for waitin'
						{
							if(subWaitSentinel)
							{
								subWaitStart = millis();
								subWaitSentinel = false;
							}
							
							if((millis() - subWaitStart) >= subWaitTime) //if wait time has passed
							{
								subStateVar = ACT; //switch back to ACT loop
								subWaitSentinel = true; //set sentinel back to true for next invocation of wait state
							}
						} break;
						
						case ACT: //for doin' stuff
						{
							digitalWrite(MOTORENABLE, HIGH);
							digitalWrite(MOTOR1, LOW);
							digitalWrite(MOTOR2, HIGH);
							
							if(firstAct) //if just starting the action state
							{
								sensorTime = millis(); //save current time
								firstAct = false;
							}
							
							if(hasOccluded)
							{
								if(!digitalRead(BREAKBEAM)) //is cleared again
								{
									//Serial.println("cleared");
									digitalWrite(MOTOR2, LOW); //done. stop motor.
									
									//reset stuff
									hasOccluded = false;
									firstAct = true;
									
									stateVar = IDLE; //go to idle state.
								}
								else //has not yet cleared
								{
									if((millis() - sensorTime) >= CLEAR_TIME) //sensor has not cleared before timeout, indicating jam
									{
										Serial.println("ERROR:CLEAR_TIME");
										
										//reset stuff
										hasOccluded = false;
										firstAct = true;
										
										stateVar = ERROR; //probably jammed.
										
									}
								}
							}
							else //hasOccluded sentinel not yet set
							{
								if(digitalRead(BREAKBEAM)) //is occluded
								{
									//Serial.println("occluded");
									hasOccluded = true;
									sensorTime = millis(); //reset sensorTime to current time.
								}
								else
								{
									if((millis() - sensorTime) >= OCCLUDE_TIME) //check to see if time to occlude has taken longer than allowed
									{
										Serial.println("ERROR:OCCLUDE_TIME");
										
										//reset stuff
										hasOccluded = false;
										firstAct = true;
										
										stateVar = ERROR; //probably jammed.
										
									}
									//restart loop.
								}
							}
						} break;
					}						
				} break;
			}		
		} break;
		
		case ERROR:
		{
			//stop everything!
			digitalWrite(MOTOR1, LOW);
			digitalWrite(MOTOR2, LOW);
			digitalWrite(MOTORENABLE, LOW);
		} break;
	
		case ESTOP:
		{
			//stop everything!
			digitalWrite(MOTOR1, LOW);
			digitalWrite(MOTOR2, LOW);
			digitalWrite(MOTORENABLE, LOW);
		} break;
	}
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
			configVar = FORWARD;
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
			configVar = REVERSE;
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
		//digitalWrite(GREEN_PIN, HIGH);
		return 0;
	}
	
	if(inString.equalsIgnoreCase("ESTOP"))
	{
		Serial.println("ESTOP");
		stateVar = ESTOP; //set next state to estop
		return 0;
	}
	
	/*if(inString.equalsIgnoreCase("RESULT"))
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
	}*/
	
	if(inString.equalsIgnoreCase("STATE")) //status query
	{
		Serial.println("State, Config, subState:");
		Serial.println(stateVar);
		Serial.println(configVar);
		Serial.println(subStateVar);
		return 0;
	}
};