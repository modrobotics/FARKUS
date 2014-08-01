/*
  Mobo Automation jig master program
 */

#include <Time.h>
#include "Enums.h"

//pin defs
int PinCancelButton = 2;
int PinPress = 3;
int PinGoButton = 4;
int PinCarraigeAdv = 5;
int PinGarbage = 6;
int PinBoardGate = 9;
int PinUpperPhotoInt = 14; //0+14
int PinLowerPhotoInt = 15; //1+14
int PinAdvancePhotoInt = 16; //2+14
int PinRedLED = 17; //ERROR
int PinYellowLED = 18; //BUSY
int PinGreenLED = 19; //READY

Mode CurrentMode;
volatile bool EmergencyStop; //if this is true stop everything!
volatile ErrorType CurrentErrorType; 

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(PinCancelButton, INPUT);
  digitalWrite(PinCancelButton, HIGH); //turns on pull-up resistor
  attachInterrupt(0, EmergencyStopPushed, FALLING);
  pinMode(PinPress, OUTPUT); 
  pinMode(PinGoButton, INPUT);
  digitalWrite(PinGoButton, HIGH); //turns on pull-up resistor
  pinMode(PinCarraigeAdv, OUTPUT); 
  pinMode(PinBoardGate, OUTPUT);   
  pinMode(PinUpperPhotoInt, INPUT); 
  pinMode(PinLowerPhotoInt, INPUT); 
  pinMode(PinAdvancePhotoInt, INPUT); 
  pinMode(PinRedLED, OUTPUT); 
  pinMode(PinYellowLED, OUTPUT); 
  pinMode(PinGreenLED, OUTPUT); 
  
  EmergencyStop = false;
  CurrentErrorType = ERR_NONE;
  
  //CurrentMode = MODE_IDLE;
  //CurrentMode = MODE_TESTLEDS;
  //CurrentMode = MODE_TESTPHOTOINTS;
  //CurrentMode = MODE_TESTBUTTONS;
  //CurrentMode = MODE_TESTBOARDPRESS;
  CurrentMode = MODE_TESTCARRAIGE;
}

// the loop routine runs over and over again forever: Modes may ONLY be changed in this function...
void loop() {
  //pre-check mode changes
  if (CurrentMode == MODE_IDLE && EmergencyStop) EmergencyStop = false; //prevent ESTOP in idle mode. can't emergency stop when not doing anything!
  if (CurrentErrorType != ERR_NONE && CurrentMode != MODE_ERROR){AllOff(); WaitUntilCancelUnpressed(); CurrentMode = MODE_ERROR; } //catch if an error has been set!
  if (EmergencyStop && CurrentMode != MODE_ESTOPPED){AllOff(); WaitUntilCancelUnpressed(); CurrentMode = MODE_ESTOPPED;} //catch if emergency stop hit  (overrides error mode)
    
  switch (CurrentMode){
    case MODE_ESTOPPED: //Cancel goes to idle mode. Go begins working mode.
       RedOn(); YellowOff(); GreenOff();
       CurrentErrorType = ERR_NONE; //clears any errors
       delay(2);
       if (IsCancelPressed()){WaitUntilCancelUnpressed(); EmergencyStop=false; CurrentMode = MODE_IDLE;}
       else if (IsGoPressed()){EmergencyStop=false; CurrentMode = MODE_WORKING;}
    break;
    case MODE_ERROR: FlashError(CurrentErrorType); break; //Cancel goes to EStopped mode via ISR
    case MODE_IDLE: RedOff(); YellowOff(); GreenOn(); delay(2); if (IsGoPressed()) CurrentMode = MODE_WORKING; break;
    case MODE_WORKING: {
      RedOff(); YellowOn(); GreenOff();
     // Purge(); 
     
     boolean Success = ProcessBoard();
     
//      boolean Success = WaitForBoardPass(PinUpperPhotoInt, -1);
//  //    if (!Success) FlashError(ERR_BOARDPASS);
//  //    YellowOff();
//      
//      if (Success){
//       
//        digitalWrite(PinPress, HIGH); 
//        delay(200); 
//        digitalWrite(PinPress, LOW); 
//        delay(200);
//        digitalWrite(PinBoardGate, HIGH); 
//        GreenOn();
//        delay(200);
//        digitalWrite(PinBoardGate, LOW); 
//        GreenOff();
//        delay(100);  
//        
//       }
       
       if (Success) CurrentMode = MODE_IDLE;
    }
    break;
    
    
    //simple test modes
    case MODE_TESTLEDS: TestLEDs(); break;
    case MODE_TESTPHOTOINTS: TestPhotoInterruptor(); break;
    case MODE_TESTBUTTONS: TestButtons(); break;
    case MODE_TESTBOARDPRESS: TestBoardPress(); break;
    case MODE_TESTCARRAIGE: TestCarraige(); break;
  }
  

}

boolean ProcessBoard(void){
  if (!AdvanceCarraige()) return false;
  if (!WaitForBoardPass(PinUpperPhotoInt, 500)) return false;
  
        digitalWrite(PinPress, HIGH); 
        delay(200); 
        
        //Test sequence, return reuslt, handle result.
        
        digitalWrite(PinPress, LOW); 
        delay(200);
        digitalWrite(PinBoardGate, HIGH); 
        GreenOn();
        delay(200);
        digitalWrite(PinBoardGate, LOW); 
        GreenOff();
        delay(100);  
  
  retrun true;
}


//waits for a board to pass. Returns in TimeoutMs unless = -1;
//watches for the photointerrupter to go low. Then waits for it to be high at least "WaitAfterDetectMs" ms
boolean WaitForBoardPass(int PhotoIntPin, int TimeoutMs)
{
  unsigned long StartTimeMs = millis();
  int WaitAfterDetectMs = 200; //amount in ms to wait after last detected board. Should make it immune to noise and/or holes in the board. 
  boolean DetectedBoard = false; //has the sensor been interrupted at all yet?
  unsigned long LastDetectedTime;
  
  while (!EmergencyStop) {
    if (IsOccluded(PhotoIntPin)){ //if there is a board in front of the sensor
      DetectedBoard = true; //a board has tripped the sensor
      LastDetectedTime = millis(); //last known point the sensor was blocked
    }
    
    if (DetectedBoard && millis()-LastDetectedTime > WaitAfterDetectMs) return true; //if we had detected a board but it's been at least WaitAfterDetectMs ms
    
    if (TimeoutMs != -1 && millis()-StartTimeMs > TimeoutMs){CurrentErrorType = ERR_BOARDPASS; return false;}
    //delay(2); 
  }
  
  return false; //stopped prematurely from emergency stop
}

boolean AdvanceCarraige(void) {
  if (EmergencyStop) return false;
  int TimeoutMs = 300;
  unsigned long StartTimeMs = millis();
  
  if (!IsOccluded(PinAdvancePhotoInt)){ CurrentErrorType = ERR_BOARDADVANCE; return false;} //if not loaded or not aligned
  
  CarraigeEscapementOn();
  while (IsOccluded(PinAdvancePhotoInt)){
    if (EmergencyStop) return false;
    if (millis()-StartTimeMs > TimeoutMs){CarraigeEscapementOff(); CurrentErrorType = ERR_BOARDADVANCE; return false; }//if didn't advance first step
    else (delay(5));
  }
  CarraigeEscapementOff();
  while (!IsOccluded(PinAdvancePhotoInt)){
    if (EmergencyStop) return false;
    if (millis()-StartTimeMs > TimeoutMs){CurrentErrorType = ERR_BOARDADVANCE; return false;} //if didn't advance second step
    else (delay(5));
  }
  return true;
}


void FlashError(ErrorType ThisError)
{
  YellowOff(); GreenOff();
  
  switch (ThisError){
   case ERR_BOARDPASS: PulseRedLong(); PulseRedShort(); PulseRedShort(); break;//Long short short
   case ERR_BOARDADVANCE: PulseRedShort(); PulseRedLong(); PulseRedShort(); break;//short long short
   case ERR_GENERAL: PulseRed(1000, 200); break;
  } 
  if (!EmergencyStop) delay(500); //distinguish when on repeat
}

//ISR
void EmergencyStopPushed(void){EmergencyStop = true;}

void AllOff(void){GreenOff(); YellowOff(); RedOff(); BoardPressOff(); CarraigeEscapementOff(); BoardGateOff();} //deactivates EVERYTHING (safe to sit without, say, cooking a solenoid)
void Purge(void){YellowOn(); BoardPressOn(); delay(200); BoardPressOff(); GarbageOn(); delay(200); BoardGateOn(); delay(200); BoardGateOff(); delay(200); GarbageOff(); delay(200);} //tries to clear any boards in the system (TODO: make estop-comatible)

//LED Utility functions
void RedOn(){digitalWrite(PinRedLED, HIGH);}
void RedOff(){digitalWrite(PinRedLED, LOW);}
void YellowOn(){digitalWrite(PinYellowLED, HIGH);}
void YellowOff(){digitalWrite(PinYellowLED, LOW);}
void GreenOn(){digitalWrite(PinGreenLED, HIGH);}
void GreenOff(){digitalWrite(PinGreenLED, LOW);}

void PulseRedLong(){PulseRed(300, 200);}
void PulseRedShort(){PulseRed(100, 200);}
void PulseRed(int MsOn, int MsOff){if (EmergencyStop) return;  RedOn(); delay(MsOn); RedOff(); delay(MsOff);} //pulses the red LED on for MsOn the waits MsOff

void TestLEDs(void){RedOn(); delay(333); RedOff(); YellowOn(); delay(333); YellowOff(); GreenOn(); delay(333); GreenOff();} //pulses each one for 1/3 of a second

//PhotoInterruptor utility functions
boolean IsOccluded(int PhotoIntPin){return digitalRead(PhotoIntPin) == LOW;}
void TestPhotoInterruptor(void){if (IsOccluded(PinAdvancePhotoInt)) RedOn(); else RedOff(); if (IsOccluded(PinUpperPhotoInt)) YellowOn(); else YellowOff(); if (IsOccluded(PinLowerPhotoInt)) GreenOn(); else GreenOff();} //LED lights when sensor is occluded

//Button Utility functions
boolean IsGoPressed(void){for (int i=0; i<10; i++){if (digitalRead(PinGoButton)==HIGH) return false; delay(2);} return true;}; //reads 10 times to avoid bounce
boolean IsCancelPressed(void){for (int i=0; i<10; i++){if (digitalRead(PinCancelButton)==HIGH) return false; delay(2);} return true;}; //reads 10 times to avoid bounce
void WaitUntilCancelUnpressed(void){int NumHighInARow = 0; while(NumHighInARow<10){if (digitalRead(PinCancelButton)==LOW) NumHighInARow = 0; else NumHighInARow++; delay(2);}}//hangs out inside function until cancle button is un-pressed

void TestButtons(void) {while(true){if (IsGoPressed()) GreenOn(); else GreenOff();   if (IsCancelPressed()) RedOn(); else RedOff();}} //red lights up when Cancel pressed, green when Go pressed

//Pneumatic Utility functions
void BoardPressOn(void) {digitalWrite(PinPress, HIGH);}
void BoardPressOff(void) {digitalWrite(PinPress, LOW);}
void CarraigeEscapementOn(void) {digitalWrite(PinCarraigeAdv, HIGH);}
void CarraigeEscapementOff(void) {digitalWrite(PinCarraigeAdv, LOW);}
void GarbageOn(void) {digitalWrite(PinGarbage, HIGH);}
void GarbageOff(void) {digitalWrite(PinGarbage, LOW);}

void TestBoardPress(void) {while(true){if (IsGoPressed()){BoardPressOn(); YellowOn();} else if (IsCancelPressed()){BoardPressOff(); YellowOff();}}} //cylinder activates when green pressed and deactivates when red pressed.
void TestCarraige(void) {if (IsGoPressed() && !AdvanceCarraige()){ FlashError(ERR_BOARDADVANCE); CurrentErrorType = ERR_NONE;} else delay(20);} //Advances carraige every time green button pressed. 

//Solenoid Utility functions
void BoardGateOn(void) {digitalWrite(PinBoardGate, HIGH);}
void BoardGateOff(void) {digitalWrite(PinBoardGate, LOW);}
void BardGateOpen(void){BoardGateOn(); delay(200); BoardGateOff();} //TODO: use lower photoint to verify this...?

//Communication utilities




