/**
 * \class CubeletsShield
 *
 *
 * \note CubeletsShield.h - A Library for interacting with the Cubelets Arduino Shield.
 *
 * \author Jonathan Moyes
 *
 * \version 1.0
 *
 * \date 2013-07-10 12:00:00
 *
 * Contact: jmoyes@modrobotics.com
 *
 * Created on: Mon Jun 10 12:00:00 2013
 * 
 */
 
#ifndef CubeletsShield_h
#define CubeletsShield_h

#include "Arduino.h"
#include "I2C.h"       // Mo' Betta I2C Library.

#define ADDRESS_CUBE1 0b0100010
#define ADDRESS_CUBE2 0b0100011
#define ADDRESS_CUBE3 0b0100100    // Define Cubelet I2C Addresses Here
#define ADDRESS_CUBE4 0b0100101
#define ADDRESS_CUBE5 0b0100110
#define ADDRESS_CUBE6 0b0100111

#define COMMAND_LEDON 0x20
#define COMMAND_LEDOFF 0x21        // Define I2C Commands here
#define COMMAND_SETBVSELF 0x22
#define COMMAND_GETBVNEIGHBOR 0x23
#define COMMAND_GETBVSELF 0x26
#define COMMAND_STOPCOMM 0x24
#define COMMAND_STARTCOMM 0x25

#define RESPONSE_LENGTH_GETBVNEIGHBOR  1   // Number of bytes to expect from a read transaction
#define RESPONSE_LENGTH_GETBVSELF  1   // Number of bytes to expect from a read transaction
#define I2CRETRYCOUNT 5            // How many times to retry I2C transactions before giving up

class CubeletsShield
{
  public:
    CubeletsShield(boolean enableDebugOverUsart, unsigned char ledPinForI2C);  // Constructor
    
    /// This function must be called to initialize the Library
    void initialize(); /**< 
                       * We need to use a delay() call in this function, and since the timers 
                       * aren't running until after object instanciation, we can't do that in the 
                       * constructor.  This function must be called in setup().
                       */
                       
    // Public API Functions
    
    /// Call this function to prepare the Shield for use
    void initializeCubeletsShield(); /**< 
                                     * Sets the broadcast block_value to 0x00, 
                                     * illuminates the Debug LED, and enabled Cubelet communication on 
                                     * all of the Cubelets on the Shield
                                     */
                                     
    /// Illuminates the Debug LED on the Cubelet specified by faceNumber.                         
    unsigned char greenOffByFace(unsigned char faceNumber);
    
    /// Extinguishes the Debug LED on the Cubelet specified by faceNumber.                         
    unsigned char greenOnByFace(unsigned char faceNumber);
    
    /// Sets the value of block_value on the target Cubelet Face Number                      
    unsigned char setBroadcastBlockValueByFace(unsigned char faceNumber, unsigned char blockValue);
    
    /// Gets the value of block_value on the target Cubelet Face Number                      
    unsigned char getBroadcastBlockValueByFace(unsigned char faceNumber);
    
    /// Gets the block_value being broadcast from a neighboring Sense Cubelet                       
    unsigned char getNeighborBlockValueByFace(unsigned char faceNumber);
    
    /// Enables Cubelet Communication on the targeted Cubelet Face Number                          
    unsigned char startCommunicationByFace(unsigned char faceNumber);
    
    /// Disables Cubelet Communication on the targeted Cubelet Face Number                          
    unsigned char stopCommunicationByFace(unsigned char faceNumber);
    
    // Public API Helper functions
    
    /// Enables Cubelet Communication on ALL Cubelets on the shield                         
    void startAllCubeletCommunication();
    
    /// Disables Cubelet Communication on ALL Cubelets on the shield                         
    void stopAllCubeletCommunication();
    
    /// Illuminates the Debug LED on ALL Cubelets on the shield                         
    void greenOnAllCubelets();
    
    /// Extinguishes the Debug LED on ALL Cubelets on the shield                         
    void greenOffAllCubelets();
    
  private:    
    // Private Variables
    volatile unsigned char rec_byte_i2c;
    volatile int _I2CReturnCode;
    unsigned char _cubeletI2CAddresses[];
    unsigned char ledPin;
    boolean debugOverUsart;
    
    // Private Functions 
    void initializeI2C();
    void trafficLEDOn();
    void trafficLEDOff();
    unsigned char sendCommandI2C(unsigned char addr, unsigned char command, unsigned char retries);
    unsigned char sendCommandWithDataI2C(unsigned char addr, unsigned char command, unsigned char data, unsigned char retries );
    unsigned char getDataI2C(unsigned char addr, unsigned char command, unsigned char bytesToRead, unsigned char retries );
    unsigned char getI2CAddressByFace(unsigned char faceNumber);
};

#endif  // End #ifndef CubeletsShield_h
