/*
 * Brightness_Calibration_Jig.cpp
 *
 * Created: 7/2/2013 2:09:00 PM
 *  Author: Nev
 */ 
#include "Arduino.h"
#include "HardwareSerial.h"
#include "CubeletsShield.h"

#define READ_PIN A0
#define LED_PIN 3
#define SELECT_PIN 2 //pin to select between brightness and flashlight
#define DETECT_PIN 8 //pin pulled low when cubelet is present
#define FAIL_PIN 9 //red LED pin
#define PASS_PIN 10 //green LED pin
#define FACE 1 //which port on the shield the cubelet face is connected to
#define MAX_VALUE 255 //maximum value for LED PWM, should be <=255 depending on LED
#define INC 10 //increment 
#define INC_DELAY 50 //time between each increment in milliseconds
#define SAMPLES 3 //number of samples to take between increments
#define SAMPLE_DELAY 10 //delay between samples
#define CUBELET_DELAY 50 //time between setting LED and taking samples to allow cubelet to respond

void setup();
void loop();

CubeletsShield cubelets(false, 13);
unsigned char neighborValue;
int runningSamples; //variable to hold running sum of samples
unsigned char dutyCycle = 0;

void setup()
{
	Serial.begin(9600);
	pinMode(READ_PIN, INPUT);
	pinMode(SELECT_PIN, INPUT_PULLUP);
	pinMode(DETECT_PIN, INPUT);
	pinMode(FAIL_PIN, OUTPUT);
	pinMode(PASS_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
	cubelets.initialize();
	cubelets.greenOffAllCubelets();
}


void loop()
{

	analogWrite(LED_PIN, 0); //zero out LED
	
	while(digitalRead(DETECT_PIN));
	
	while(neighborValue < 255 && dutyCycle < 255) //measurement loop, until cubelet is saturated
	{
		runningSamples = 0; //zero out sample summing variable
		analogWrite(LED_PIN, dutyCycle); //PWM LED at duty cycle
		//Serial.print("#>Duty Cycle: ");
		Serial.print(dutyCycle);
		Serial.print(", ");
		delay(CUBELET_DELAY); //give cubelet a bit of  time to respond
		for(int i = 0; i < SAMPLES; i++) //sample loop
		{
			//Serial.print("Getting block value...\n");
			runningSamples += cubelets.getNeighborBlockValueByFace(FACE); //sum samples of measurement
			//Serial.print("Running: ");
			//Serial.print(runningSamples);
			//Serial.print("\n");
		}
		//Serial.print("Avg: ");
		Serial.print(runningSamples/SAMPLES);
		Serial.print("\n"); //hax newline
		dutyCycle++; //increment duty cycle
	}
}