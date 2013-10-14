/*
 * Brightness_Tester_Prototype.cpp
 *
 * Created: 4/9/2013 11:09:00 AM
 *  Author: Nev
 */ 
#include "Arduino.h"
#include "HardwareSerial.h"
#include "CubeletsShield.h"

#define READ_PIN A0
#define LED_PIN 3
#define SELECT_PIN 2
#define DETECT_PIN 8
#define FAIL_PIN 9
#define PASS_PIN 10
#define FACE 1 //which port on the shield the cubelet face is connected to
#define FT_SAMPLES 3 //number of samples to take per measurement for flashlight test
#define FT_MEASUREMENTS 3 //number of measurements to take per cubelet for flashlight test
#define FT_ERROR_THRESHOLD 10 //below threshold is pass, at or above threshold is fail for flashlight test
#define BT_SAMPLES 3
#define BT_MEASUREMENTS 3
#define BT_ERROR_THRESHOLD 10
#define FT_MIN_DELTA 60 //minimum allowable difference between max bright and min bright
//#define FT_MAXIMUM_AMBIENCE 155 //maximum brightness (determined experimentally)
//#define BT_MAXIMUM_AMBIENCE 120
#define BT_MIN_DELTA 75
#define do_pass {									\
					digitalWrite(PASS_PIN, HIGH);	\
					digitalWrite(FAIL_PIN, LOW);	\
				}
#define do_fail {									\
					digitalWrite(PASS_PIN, LOW);	\
					digitalWrite(FAIL_PIN, HIGH);	\
				}					

void setup();
void loop();

int ft_Values[FT_MEASUREMENTS] = {0,	255,	100}; //populate with setting values for each measurements
int ft_Target[FT_MEASUREMENTS] = {25,	115,	200}; //populate for target values for each measurement

int bt_Values[BT_MEASUREMENTS] = {0,	255,	1}; //pop with LED PWM values for each measurement
int bt_Target[BT_MEASUREMENTS] = {10,	130,	220}; //pop with target values for brightness block for each measurement

CubeletsShield cubelets(false, 13);
bool initFlashlightTest(void);
bool initBrightnessTest(void);

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
	digitalWrite(PASS_PIN, LOW); //clear pass pin
	digitalWrite(FAIL_PIN, LOW); //clear fail pin
	
	while(digitalRead(DETECT_PIN)); //wait for detect pin to be pulled low, indicating cubelet
	if(digitalRead(SELECT_PIN))
	{
		Serial.print("\n========Init Flashlight========\n");
		cubelets.greenOnAllCubelets();
		if(initFlashlightTest())
		{
			do_pass
		}
		else
		{
			do_fail
		}
		while(!digitalRead(DETECT_PIN));
		digitalWrite(PASS_PIN, LOW);
		digitalWrite(FAIL_PIN,LOW);
		cubelets.greenOffAllCubelets();
	}
	else
	{
		Serial.print("\n========Init Brightness========\n");
		cubelets.greenOnAllCubelets();
		if(initBrightnessTest())
		{
			digitalWrite(PASS_PIN, HIGH);
			digitalWrite(FAIL_PIN, LOW);
		}
		else
		{
			digitalWrite(PASS_PIN, LOW);
			digitalWrite(FAIL_PIN, HIGH);
		}
		while(!digitalRead(DETECT_PIN));
		digitalWrite(PASS_PIN, LOW);
		digitalWrite(FAIL_PIN,LOW);
		cubelets.greenOffAllCubelets();
		cubelets.greenOffAllCubelets();
	}
	
}

bool initSecondaryFlashlightTest(void)
{
	
}

bool initFlashlightTest(void)
{
	unsigned int error = 0;	//sum of errors
	int measurements[FT_MEASUREMENTS];
	unsigned int delta = 0; //difference between min and max
	int samples[FT_SAMPLES]; //array to hold sample ft_Values
	int avg = 0;
	for(int lol = 0; lol < FT_MEASUREMENTS; lol++) //measurement loop
	{
		Serial.print("Measurement ");
		Serial.print(lol);
		Serial.print(", value: ");
		Serial.print(ft_Values[lol]);
		Serial.print(", ft_Target: ");
		Serial.print(ft_Target[lol]);
		Serial.print("\n");
		cubelets.setBroadcastBlockValueByFace(FACE, ft_Values[lol]);
		delay(500);
		for(int i = 0; i < FT_SAMPLES; i++) //sample loop
		{
			samples[i]=analogRead(READ_PIN)/4; //div by 4 to scale from 10-bit to 8-bit
			Serial.print("Sample: ");
			Serial.print(samples[i]);
			Serial.print("\n");
			avg += samples[i]; //summing the samples into the avg var
			delay(100);
		}
		avg = avg/FT_SAMPLES; //divide sum by number of samples to get mean sample value = measurement value
		measurements[lol] = avg;
		if((lol == 0) && (measurements[lol] > (235-FT_MIN_DELTA)))
		{
			Serial.print("Too bright!");
			while(!digitalRead(DETECT_PIN))
			{
				digitalWrite(FAIL_PIN, HIGH);
				delay(100);
				digitalWrite(FAIL_PIN, LOW);
				delay(100);
			}
			return 0;
		}
		avg = 0; //zero out avg
	}
	cubelets.setBroadcastBlockValueByFace(FACE, 0); //zero out cubelet
	delta = abs(measurements[1] - measurements[0]);
	Serial.print("Max = ");
	Serial.print(measurements[1]);
	Serial.print("Min = ");
	Serial.print(measurements[0]);
	Serial.print(", Delta: ");
	Serial.print(delta);
	Serial.print(", Measured Median:");
	Serial.print(measurements[2]);
	Serial.print("\n");
	Serial.print("Error: ");
	Serial.print(error);
	Serial.print("\n");
	
	if(delta < FT_MIN_DELTA)
	{
		Serial.print("Failing, delta too small");
		return 0; //fail if delta is too small
	}		
	if((measurements[1] - measurements[2] > 20) && (measurements[2] - measurements[0] > 20))
	{
		Serial.print("Passing: ");
		Serial.print("m1 - m2 = ");
		Serial.print(measurements[1] - measurements[2]);
		Serial.print(", m2 - m0 = ");
		Serial.print(measurements[2] - measurements[0]);
		Serial.print("\n");	
		return 1; //pass if the middle is at least 25 away from min and max
	}	
	else
		Serial.print("Failing: ");
		Serial.print("m1 - m2 = ");
		Serial.print(measurements[1] - measurements[2]);
		Serial.print(", m2 - m0 = ");
		Serial.print(measurements[2] - measurements[0]);
		Serial.print("\n");	
		return 0;
}

bool initBrightnessTest(void)
{
	unsigned int error = 0;	//sum of errors
	int measurements[BT_MEASUREMENTS];
	unsigned int delta = 0; //difference between min and max
	int samples[BT_SAMPLES]; //array to hold sample BT_Values
	int avg = 0;
	
	//pre-test brightness check
	if(analogRead(READ_PIN) > 235-BT_MIN_DELTA)
	{
		Serial.print("Too bright!");
		while(!digitalRead(DETECT_PIN))
		{
			digitalWrite(FAIL_PIN, HIGH);
			delay(100);
			digitalWrite(FAIL_PIN, LOW);
			delay(100);
		}
		return 0;
	}
	
	for(int lol = 0; lol < BT_MEASUREMENTS; lol++) //measurement loop
	{
		Serial.print("Measurement ");
		Serial.print(lol);
		Serial.print(", value: ");
		Serial.print(bt_Values[lol]);
		Serial.print(", bt_Target: ");
		Serial.print(bt_Target[lol]);
		Serial.print("\n");
		analogWrite(LED_PIN, bt_Values[lol]);
		delay(500);
		for(int i = 0; i < BT_SAMPLES; i++) //sample loop
		{
			samples[i]=cubelets.getNeighborBlockValueByFace(FACE); //div by 4 to scale from 10-bit to 8-bit
			Serial.print("Sample: ");
			Serial.print(samples[i]);
			Serial.print("\n");
			avg += samples[i]; //summing the samples into the avg var
			delay(100);
		}
		avg = avg/BT_SAMPLES; //divide sum by number of samples to get mean sample value = measurement value
		measurements[lol] = avg;
		
		/*if((lol == 0) && (measurements[lol] > BT_MAXIMUM_AMBIENCE))
		{
			Serial.print("Too bright!");
			while(!digitalRead(DETECT_PIN))
			{
				digitalWrite(FAIL_PIN, HIGH);
				delay(100);
				digitalWrite(FAIL_PIN, LOW);
				delay(100);
			}
			return 0;
		}*/
		avg = 0; //zero out avg
	}
	analogWrite(LED_PIN, 0); //zero out cubelet
	delta = abs(measurements[1] - measurements[0]);
	Serial.print("Max = ");
	Serial.print(measurements[1]);
	Serial.print("Min = ");
	Serial.print(measurements[0]);
	Serial.print(", Delta: ");
	Serial.print(delta);
	Serial.print(", Measured Median:");
	Serial.print(measurements[2]);
	Serial.print("\n");
	Serial.print("Error: ");
	Serial.print(error);
	Serial.print("\n");
	
	if(delta < BT_MIN_DELTA)
	{
		Serial.print("Failing, delta too large");
		return 0; //fail if delta is too small
	}
	if((measurements[1] - measurements[2] > 20) && (measurements[2] - measurements[0] > 20))
	{
		Serial.print("Passing: ");
		Serial.print("m1 - m2 = ");
		Serial.print(measurements[1] - measurements[2]);
		Serial.print(", m2 - m0 = ");
		Serial.print(measurements[2] - measurements[0]);
		Serial.print("\n");
		return 1; //pass if the middle is at least 25 away from min and max
	}
	else
	Serial.print("Failing: ");
	Serial.print("m1 - m2 = ");
	Serial.print(measurements[1] - measurements[2]);
	Serial.print(", m2 - m0 = ");
	Serial.print(measurements[2] - measurements[0]);
	Serial.print("\n");
	return 0;
}

/*bool initBrightnessTest(void)
{
	unsigned int error = 0;	//sum of errors
	int samples[BT_SAMPLES]; //array to hold sample bt_Values
	int avg = 0;
	//delay(100);
	for(int lol = 0; lol < BT_MEASUREMENTS; lol++) //measurement loop
	{
		Serial.print("Measurement ");
		Serial.print(lol);
		Serial.print(", value: ");
		Serial.print(bt_Values[lol]);
		Serial.print(", bt_Target: ");
		Serial.print(bt_Target[lol]);
		Serial.print("\n");
		analogWrite(LED_PIN, bt_Values[lol]);
		delay(500);
		for(int i = 0; i < BT_SAMPLES; i++) //sample loop
		{
			samples[i] = cubelets.getNeighborBlockValueByFace(FACE);
			Serial.print("Sample: ");
			Serial.print(samples[i]);
			Serial.print("\n");
			avg += samples[i]; //summing the samples into the	 avg var
			delay(100);
		}
		avg = avg/BT_SAMPLES; //divide sum by number of samples to get mean sample value
		error += abs(bt_Target[lol] - avg); //compute error and add to total error
		Serial.print("Mean = ");
		Serial.print(avg);
		Serial.print(", Error: ");
		Serial.print(abs(bt_Target[lol] - avg));
		Serial.print("\n");
		avg = 0; //zero out avg
	}
	analogWrite(LED_PIN, 0); //zero out cubelet
	error = error/BT_MEASUREMENTS; //divide by number of measurements to get mean error
	Serial.print("Mean Error: ");
	Serial.print(error);
	Serial.print("\n");
	
	if(error < BT_ERROR_THRESHOLD)
	{
		//pass condition
		return true;
	}
	else
	{
		//fail condition
		return false;
	}
}*/