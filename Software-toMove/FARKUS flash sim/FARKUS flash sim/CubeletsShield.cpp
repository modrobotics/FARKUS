/*

  CubeletsShield.cpp - A Library for interacting with the Cubelets Arduino Shield.
  Modular Robotics, 2013
  Created by Jonathan Moyes, June 9, 2013
  
*/

#include "Arduino.h"
#include "CubeletsShield.h"

// Initialize Global working variables
volatile unsigned char rec_byte_i2c;;   // Worker variable to hold I2C Received Byte
volatile int I2CReturnCode;             // Worker variable to hold I2C Return Codes (See Atmel Datasheet)
unsigned char cubeletI2CAddresses[7]; 
unsigned char ledPin;
boolean debugOverUsart = 0;

CubeletsShield::CubeletsShield(boolean enableDebugOverUsart, unsigned char ledPinForI2C) {
  	// ############################################
	// #### Working Variable Initialization #######
	// ############################################

	rec_byte_i2c = 0x00;   // Worker variable to hold I2C Received Byte
	I2CReturnCode = -1;              // Worker variable to hold I2C Return Codes (See Atmel Datasheet)

        // Initialize Addresses Array
        cubeletI2CAddresses[0] = ADDRESS_CUBE1;
        cubeletI2CAddresses[1] = ADDRESS_CUBE2;
        cubeletI2CAddresses[2] = ADDRESS_CUBE3;
        cubeletI2CAddresses[3] = ADDRESS_CUBE4;
        cubeletI2CAddresses[4] = ADDRESS_CUBE5;
        cubeletI2CAddresses[5] = ADDRESS_CUBE6;
		cubeletI2CAddresses[6] = ADDRESS_FLASHER;
        
        // I2C Activity indicator light
        if(ledPinForI2C > 0){
          ledPin = ledPinForI2C;
    	  pinMode(ledPin, OUTPUT);
    	  trafficLEDOff();
        }

        // Debug over USART enabled?
        if(enableDebugOverUsart){
    
          Serial.begin(9600);
          debugOverUsart = 1;
        }
        trafficLEDOn();
}

void CubeletsShield::initialize(){
    
    // ############################################
    // #### Peripheral Configuration, Etc  ########
    // ############################################
	
    // Setup I2C for our needs
    initializeI2C();

    // Wait plenty of time for the Cubelets MCUs to start, configure their SSPs
    delay(10);
    
    // Clean Block Values, enable communication, light Debug LEDs
    initializeCubeletsShield();

    if(debugOverUsart){
	  Serial.println("Cubelet-Arduino Interface Loaded.  Ready.");
    } 
}

// #######################################################################
// I2C-RELATED FUNCTIONS
// ####################################################################### 

void CubeletsShield::initializeI2C(){
  I2c.begin();
  I2c.pullup(0);     // Disable the internal pullups
  I2c.setSpeed(1);   // 1=400kHz 0=
  I2c.timeOut(1000);  //100ms for a request to succeed
}

unsigned char CubeletsShield::sendCommandI2C(unsigned char addr, unsigned char command, unsigned char retries){
  trafficLEDOn();
  I2CReturnCode = I2c.write(addr, command); 
  trafficLEDOff();
  if(I2CReturnCode == 0){
    return I2CReturnCode;
  }
  else{
    if(debugOverUsart){
     Serial.print("Failed to send I2C Command addr: 0x");
     Serial.print(addr, HEX);
     Serial.print(" command: 0x");
     Serial.print(command, HEX);
     Serial.print(" retries remaining: ");
     Serial.println(retries, DEC);
    }
     if(retries >= 0 && retries >0){
       retries--;
       return sendCommandI2C(addr, command, retries);
     }
     else return 1;  // Return an error when we're out of retries
   }
}

unsigned char CubeletsShield::sendCommandWithDataI2C(unsigned char addr, unsigned char command, unsigned char data, unsigned char retries ){
  trafficLEDOn();
  I2CReturnCode = I2c.write(addr, command, data); 
  trafficLEDOff();
  
  if(I2CReturnCode == 0){
    return I2CReturnCode;
  }
  else{
    if(debugOverUsart){
     Serial.print("Failed to send I2C Command addr: 0x");
     Serial.print(addr, HEX);
     Serial.print(" command: 0x");
     Serial.print(command, HEX);
     Serial.print(" data: 0x");
     Serial.print(data, HEX);
     Serial.print(" retries remaining: ");
     Serial.println(retries, DEC);
    }
     if(retries >= 0 && retries >0){
       retries--;
       //delay(2);
       return sendCommandWithDataI2C(addr, command, data, retries);
     }
     else return 1;  // Return an error when we're out of retries
   }
}

// Returns 0 after `retries` attempts, else returns # of bytes available
unsigned char CubeletsShield::getDataI2C(unsigned char addr, unsigned char command, unsigned char bytesToRead, unsigned char retries ){
  trafficLEDOn();
  I2CReturnCode = I2c.read(addr, command, bytesToRead); 
  trafficLEDOff();
  
  if(I2c.available() == 1){
    return 1;
  }
  else{
    if(debugOverUsart){
     Serial.print("Failed to get data from I2C Slave addr: 0x");
     Serial.print(addr, HEX);
     Serial.print(" command: 0x");
     Serial.print(command, HEX);
     Serial.print(" bytes requested: ");
     Serial.print(bytesToRead, DEC);
     Serial.print(" retries remaining: ");
     Serial.println(retries, DEC);
    }
     if(retries >= 0 && retries >0){
       retries--;
       //delay(2);
       return getDataI2C(addr, command, bytesToRead, retries);
     }
     else return 0;  // Return an error when we're out of retries
   }
}

void CubeletsShield::trafficLEDOn(){
  digitalWrite(ledPin, HIGH); 
}

void CubeletsShield::trafficLEDOff(){
  digitalWrite(ledPin, LOW); 
}

// #######################################################################
// Cubelet Shield Functions
// ####################################################################### 

void CubeletsShield::initializeCubeletsShield(){
  
  for( unsigned char i = 1;i<=6;i++){
     greenOnByFace(i);
     setBroadcastBlockValueByFace(i, 0x00);
     startCommunicationByFace(i);
  }
}

void CubeletsShield::stopAllCubeletCommunication(){
  for( unsigned char i = 1;i<=6;i++){
     stopCommunicationByFace(i);
  }
}

void CubeletsShield::startAllCubeletCommunication(){
  for( unsigned char i = 1;i<=6;i++){
     startCommunicationByFace(i);
  }
}

void CubeletsShield::greenOnAllCubelets(){
  for( unsigned char i = 1;i<=6;i++){
     greenOnByFace(i);
  }
}

void CubeletsShield::greenOffAllCubelets(){
  for( unsigned char i = 1;i<=6;i++){
     greenOffByFace(i);
  }
}

unsigned char CubeletsShield::getBroadcastBlockValueByFace(unsigned char faceNumber){
  // Request the block value 
  if( getDataI2C(getI2CAddressByFace(faceNumber), char(COMMAND_GETBVSELF), char(RESPONSE_LENGTH_GETBVSELF), char(I2CRETRYCOUNT)) == char(RESPONSE_LENGTH_GETBVSELF)){
    // We have data
    while (I2c.available() > 0 ){     // Loop over the whole buffer, 
      rec_byte_i2c = I2c.receive();   // we only want the last char in the array
    }
    return rec_byte_i2c;
  }
  else{
    // Didn't get anything!
    return false;
  }
}

unsigned char CubeletsShield::getNeighborBlockValueByFace(unsigned char faceNumber){
  // Request the block value 
  if( getDataI2C(getI2CAddressByFace(faceNumber), char(COMMAND_GETBVNEIGHBOR), char(RESPONSE_LENGTH_GETBVNEIGHBOR), char(I2CRETRYCOUNT)) == char(RESPONSE_LENGTH_GETBVNEIGHBOR)){
    // We have data
    while (I2c.available() > 0 ){     // Loop over the whole buffer, 
      rec_byte_i2c = I2c.receive();   // we only want the last char in the array
    }
    return rec_byte_i2c;
  }
  else{
    // Didn't get anything!
    return false;
  }
}

// Returns 0 on success, error code on failure
unsigned char CubeletsShield::setBroadcastBlockValueByFace(unsigned char faceNumber, unsigned char blockValue){
  return sendCommandWithDataI2C(getI2CAddressByFace(faceNumber),
      char(COMMAND_SETBVSELF), blockValue, char(I2CRETRYCOUNT));
}

// Returns 0 on success, error code on failure 
unsigned char CubeletsShield::greenOnByFace(unsigned char faceNumber){
  return sendCommandI2C(getI2CAddressByFace(faceNumber), char(COMMAND_LEDON), char(I2CRETRYCOUNT));
}

// Returns 0 on success, error code on failure 
unsigned char CubeletsShield::greenOffByFace(unsigned char faceNumber){
  return sendCommandI2C(getI2CAddressByFace(faceNumber), char(COMMAND_LEDOFF), char(I2CRETRYCOUNT));
}

// Returns 0 on success, error code on failure 
unsigned char CubeletsShield::stopCommunicationByFace(unsigned char faceNumber){
  return sendCommandI2C(getI2CAddressByFace(faceNumber), char(COMMAND_STOPCOMM), char(I2CRETRYCOUNT));
}

// Returns 0 on success, error code on failure 
unsigned char CubeletsShield::startCommunicationByFace(unsigned char faceNumber){
  return sendCommandI2C(getI2CAddressByFace(faceNumber), char(COMMAND_STARTCOMM), char(I2CRETRYCOUNT));
}

// Returns I2C Address corresponding to the faceNumber on the Arduino Shields
unsigned char CubeletsShield::getI2CAddressByFace(unsigned char faceNumber){
  return cubeletI2CAddresses[faceNumber-1];
}
