/*
 * Brightness_Flashlight_Tester.cpp
 *
 * Created: 9/9/2013 11:50:00 AM
 *  Author: Nev
 */ 
#include "Arduino.h"
#include "CubeletsShield.h"
#include <String.h>

#define ID "`0001" //module identity String

#define READ_PIN A0
#define LED_PIN 3
#define SELECT_PIN 2
#define START_PIN 8
#define RED_PIN 9
#define GREEN_PIN 10

#define FACE 1

#define FT_SAMPLES 3
#define FT_MEASUREMENTS 3
#define FT_ERROR_THRESHOLD 10
#define FT_MIN_DELTA 60

#define BT_SAMPLES 3
#define BT_MEASUREMENTS 3
#define BT_ERROR_THRESHOLD 10
#define BT_MIN_DELTA 75

void setup();
void loop();

int ft_Values[FT_MEASUREMENTS] = {0,	255,	100};
int ft_Target[FT_MEASUREMENTS] = {25,	115,	200};
	
int bt_Values[BT_MEASUREMENTS] = {0,	255,	1};
int bt_Target[BT_MEASUREMENTS] = {10,	130,	220};

enum State {IDLE, WORKING, ERROR, ESTOP}; //state types
State stateVar;
enum SubState {WAIT, ACT};

enum Config {FLASHLIGHT, BRIGHTNESS, NONE};
Config configVar;

CubeletsShield cubelets(false, 13);
bool initFlashlightTest(void);
bool initBrightnessTest(void);
int actionResponse(String inString);
enum result {UNKNOWN, FAIL, PASS}; //test result possibilities. (UNKNOWN = 0, FAIL = 1, PASS = 2)
result testResult = UNKNOWN; //default to unknown, which should be read as fail

void setup()
{
	Serial.begin(9600);
	Serial.println(ID);
	pinMode(READ_PIN, INPUT);
	pinMode(SELECT_PIN, INPUT_PULLUP);
	pinMode(START_PIN, INPUT);
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
	cubelets.initialize();
	cubelets.greenOffAllCubelets();
	stateVar = IDLE; //set initial next state to IDLE.
	Serial.println("RDY");
}

SubState subStateVar = ACT; //acts first
unsigned int error = 0; //sum of errors
unsigned int ft_measurements[FT_MEASUREMENTS];
unsigned int ft_measurement_counter = 0;
unsigned int ft_samples[FT_SAMPLES];
unsigned int ft_sample_counter = 0;

unsigned int bt_measurements[FT_MEASUREMENTS];
unsigned int bt_measurement_counter = 0;
unsigned int bt_samples[FT_SAMPLES];
unsigned int bt_sample_counter = 0;

unsigned int delta = 0;
unsigned int sum = 0;
long subWaitStart = 0; //holder for the start time of a WAIT state
long subWaitTime; //variable to set number of milliseconds a WAIT state should take
bool subWaitSentinel = true; //sentinel to see if waiting is occuring...
enum LoopType {MEASURE, SAMPLE, ANALYZE};
LoopType CurrentLoop = MEASURE; //enter measurement loop first

void loop() //get to state machine.
{
	//serial check
	String inString;
	char inChar;
	while(Serial.available())
	{
		delay(2);
		inChar = Serial.read();
		inString.concat(inChar);
	}
	actionResponse(inString);
	
	//button check
	if(!digitalRead(START_PIN))
	{
		if(stateVar == IDLE)
		{
			Serial.println("GO");
			stateVar = WORKING; //set next state to working
		}
	}
	
	switch(stateVar)
	{
		case IDLE:
		{
			if(testResult == UNKNOWN) //test results not available
			{
				digitalWrite(RED_PIN, LOW);
				digitalWrite(GREEN_PIN, LOW); //both status lights off
			}
			else if (testResult == FAIL)
			{
				digitalWrite(RED_PIN, HIGH);
				digitalWrite(GREEN_PIN, LOW);				
			}
			else if (testResult == PASS)
			{
				digitalWrite(RED_PIN, LOW);
				digitalWrite(GREEN_PIN, HIGH);
			}
		} break;
		case WORKING:
		{
			digitalWrite(RED_PIN, LOW);
			digitalWrite(GREEN_PIN, LOW);
			//if there were a yellow pin, it could go high here...
			
			testResult = UNKNOWN; //set test result to unknown since starting new test...
			
			switch(configVar) //selects which test will be run based on configVar which is set by serial.
			{
				case FLASHLIGHT:
				{
					switch(subStateVar)
					{
						case WAIT:
						{
							if(subWaitSentinel) //checks if waiting has just started or is continuing
							{
								subWaitStart = millis(); //set holding variable to current time
								subWaitSentinel = false; //set sentinel to false so next loop will not reset holding var
								//Serial.print("WAIT_START: ");
								//Serial.println(subWaitStart);
							}
						
							//Serial.print("Checking ");
							//Serial.print(millis());
							//Serial.print("-");
							//Serial.print(subWaitStart);
							//Serial.print("=");
							//Serial.print(millis()-subWaitStart);
							//Serial.print(" against ");
							//Serial.println(subWaitTime);
							
							if((millis() - subWaitStart) >= subWaitTime) //if wait time has passed
							{
								//Serial.println("ACT");
								subStateVar = ACT; //switch back to ACT loop
								subWaitSentinel = true; //set sentinel to true so next invocation of wait loop will update holding variable
							}
						} break;
						case ACT:
						{
							switch(CurrentLoop)
							{
								case MEASURE:
								{
									//Serial.print("Setting ");
									//Serial.println(ft_Values[ft_measurement_counter]);
									cubelets.setBroadcastBlockValueByFace(FACE, ft_Values[ft_measurement_counter]);
									//digitalWrite(RED_PIN, HIGH); derp
									CurrentLoop = SAMPLE;
									subWaitTime = 500; //set wait time to 500ms. waiting for block to respond.
									//Serial.println("MEASURE_WAIT");
									subStateVar = WAIT; //set next state to wait
								} break;
								case SAMPLE:
								{
									ft_samples[ft_sample_counter] = analogRead(READ_PIN)/4; //div 4 for 10-bit to 8-bit
									sum += ft_samples[ft_sample_counter]; //summing samples
									//CurrentLoop = MEASURE;
									ft_sample_counter++;
									subWaitTime = 100;
									//Serial.println("SAMPLE_WAIT");
									subStateVar = WAIT;
									if(ft_sample_counter >= FT_SAMPLES) //when all samples are taken
									{
										CurrentLoop = MEASURE; //switch back to measure loop
										ft_sample_counter = 0; //reset sample counter
										ft_measurements[ft_measurement_counter] = sum / FT_SAMPLES; //set measurement to avg of samples
										//Serial.print("Measurement ");
										//Serial.println(ft_measurements[ft_measurement_counter]);
										sum = 0; //zero out sum for next set of samples
										
										if(ft_measurement_counter >= FT_MEASUREMENTS) //when all measurements and samples are taken
										{
											CurrentLoop = ANALYZE; //switch to analyze loop
											subStateVar = ACT; //switch to ACT so analysis happens
											ft_measurement_counter = 0; //reset measurement counter
											cubelets.setBroadcastBlockValueByFace(FACE, 0); //set target to 0 before going to analyze
										}
										
										else
										{
											ft_measurement_counter++; //increment measurement counter after doing e'r'thing else and only if not all samples have been taken.
										}
										
									}
								} break;
								case ANALYZE:
								{
									//Serial.println("ANALYZE");
									delta = abs(ft_measurements[1] - ft_measurements[0]);
									if(delta < FT_MIN_DELTA)
									{
										//TODO: set up fail stuff (delta too small)
										testResult = FAIL;
										Serial.println("FAIL");
										stateVar = IDLE;
									}
									else if((ft_measurements[1] - ft_measurements[2] > 20) && (ft_measurements[2] - ft_measurements[0] > 20))
									{
										//TODO: set up pass stuff
										testResult = PASS;
										Serial.println("PASS");
										stateVar = IDLE;
									}
									else
									{
										testResult = FAIL;
										Serial.println("FAIL");
										stateVar = IDLE;
										//TODO: set up more fail stuff (threshold not met)
									}
									
// 									Serial.print("Measurement 0: ");
// 									Serial.println(ft_measurements[0]);
// 									Serial.print("Measurement 1: ");
// 									Serial.println(ft_measurements[1]);
// 									Serial.print("Measurement 2: ");
// 									Serial.println(ft_measurements[2]);
// 									Serial.println("Measurements done. Switching to idle...");
									//reset WORKING control variables...
									CurrentLoop = MEASURE;
									subWaitTime = 0;
									subStateVar = ACT;
									stateVar = IDLE; //set state to IDLE, since tasks are finished. now waiting for GO.
									//TODO: fill in data analysis tasks
								} break;
							}
							
						} break;
					}
					
				} break;
				case BRIGHTNESS:
				{
					switch(subStateVar)
					{
						case WAIT:
						{
							if(subWaitSentinel) //checks if waiting has just started or is continuing
							{
								subWaitStart = millis(); //set holding variable to current time
								subWaitSentinel = false; //set sentinel to false so next loop will not reset holding var
								//Serial.print("WAIT_START: ");
								//Serial.println(subWaitStart);
							}
							
							//Serial.print("Checking ");
							//Serial.print(millis());
							//Serial.print("-");
							//Serial.print(subWaitStart);
							//Serial.print("=");
							//Serial.print(millis()-subWaitStart);
							//Serial.print(" against ");
							//Serial.println(subWaitTime);
							
							if((millis() - subWaitStart) >= subWaitTime) //if wait time has passed
							{
								//Serial.println("ACT");
								subStateVar = ACT; //switch back to ACT loop
								subWaitSentinel = true; //set sentinel to true so next invocation of wait loop will update holding variable
							}
						} break;
						case ACT:
						{
							switch(CurrentLoop)
							{
								case MEASURE:
								{
									//Serial.print("Setting ");
									//Serial.println(bt_Values[bt_measurement_counter]);
									analogWrite(LED_PIN, bt_Values[bt_measurement_counter]);
									//digitalWrite(RED_PIN, HIGH); derp
									CurrentLoop = SAMPLE;
									subWaitTime = 500; //set wait time to 500ms. waiting for block to respond.
									//Serial.println("MEASURE_WAIT");
									subStateVar = WAIT; //set next state to wait
								} break;
								case SAMPLE:
								{
									bt_samples[bt_sample_counter] = cubelets.getNeighborBlockValueByFace(FACE);
									sum += bt_samples[bt_sample_counter]; //summing samples
									//CurrentLoop = MEASURE;
									bt_sample_counter++;
									subWaitTime = 100;
									//Serial.println("SAMPLE_WAIT");
									subStateVar = WAIT;
									if(bt_sample_counter >= BT_SAMPLES) //when all samples are taken
									{
										CurrentLoop = MEASURE; //switch back to measure loop
										bt_sample_counter = 0; //reset sample counter
										bt_measurements[bt_measurement_counter] = sum / BT_SAMPLES; //set measurement to avg of samples
										//Serial.print("Measurement ");
										//Serial.println(bt_measurements[bt_measurement_counter]);
										sum = 0; //zero out sum for next set of samples
										
										if(bt_measurement_counter >= BT_MEASUREMENTS) //when all measurements and samples are taken
										{
											CurrentLoop = ANALYZE; //switch to analyze loop
											subStateVar = ACT; //switch to ACT so analysis happens
											bt_measurement_counter = 0; //reset measurement counter
											cubelets.setBroadcastBlockValueByFace(FACE, 0); //set target to 0 before going to analyze
										}
										
										else
										{
											bt_measurement_counter++; //increment measurement counter after doing e'r'thing else and only if not all samples have been taken.
										}
										
									}
								} break;
								case ANALYZE:
								{
									//Serial.println("ANALYZE");
									delta = abs(bt_measurements[1] - bt_measurements[0]);
									if(delta < BT_MIN_DELTA)
									{
										//TODO: set up fail stuff (delta too small)
										testResult = FAIL;
										Serial.println("FAIL");
										stateVar = IDLE;
									}
									else if((bt_measurements[1] - bt_measurements[2] > 20) && (bt_measurements[2] - bt_measurements[0] > 20))
									{
										//TODO: set up pass stuff
										testResult = PASS;
										Serial.println("PASS");
										stateVar = IDLE;
									}
									else
									{
										Serial.println("FAIL");
										testResult = FAIL;
										stateVar = IDLE;
										//TODO: set up more fail stuff (threshold not met)
									}
									
// 									Serial.print("Measurement 0: ");
// 									Serial.println(bt_measurements[0]);
// 									Serial.print("Measurement 1: ");
// 									Serial.println(bt_measurements[1]);
// 									Serial.print("Measurement 2: ");
// 									Serial.println(bt_measurements[2]);
// 									Serial.println("Measurements done. Switching to idle...");
									//reset WORKING control variables...
									CurrentLoop = MEASURE;
									subWaitTime = 0;
									subStateVar = ACT;
									stateVar = IDLE; //set state to IDLE, since tasks are finished. now waiting for GO.
									//TODO: fill in data analysis tasks
								} break;
							}
							
						} break;
					}
					
				}break;
			}
			
		}
		
		case ERROR:
		{
			//TODO: fill in the ERROR state stuff
		} break;
		
		case ESTOP:
		{
			/*TODO: fill in the ESTOP state stuff*/
			//blink red light for estop state
// 			digitalWrite(RED_PIN, HIGH);
// 			if(
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
			configVar = FLASHLIGHT;
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
			configVar = BRIGHTNESS;
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
		Serial.println(configVar);
		Serial.println(subStateVar);
		return 0;
	}	
};