/*
  Mobo Automation jig master program
 */

#include <Time.h>
#include "Enums.h"

//Hard coded delay times
#define PIN_PRESS_DELAY_MS 300
#define TEST_BED_TIMEOUT 30000 /*time out if no response to test initiation after 30 seconds*/

//pin defs
int PinCancelButton = 2;
int PinPress = 3;
int PinGoButton = 4;
int PinCarraigeAdv = 5;
int PinSorter = 6;
int PinBoardGate = 9;
int PinUpperPhotoInt = 14; //0+14
int PinLowerPhotoInt = 15; //1+14
int PinAdvancePhotoInt = 16; //2+14
int PinRedLED = 17; //ERROR
int PinYellowLED = 18; //BUSY
int PinGreenLED = 19; //READY
int PinCRSL_INITEST_RDY = 1; // pin1/tx
int PinCRSL_PFCLK = 0;       // pin0/rx
int PinCRSL_PFDAT = 7;
int PinCRSL_CLAMPED = 8;


//Initializations
Mode CurrentMode = MODE_IDLE;
//Mode CurrentMode = MODE_TESTCARRAIGE;
boolean DemoMode = false;
volatile bool EmergencyStop = false; //if this is true stop everything!
volatile ErrorType CurrentErrorType = ERR_NONE; 
int testInitPFCLKState = LOW;
boolean testInProgress = false;
testResultCode testResult = TEST_FAIL;
int pinCRSL_PFDAT_state = HIGH;
int pinCRSL_PFCLK_state = HIGH;
int pinCRSL_INITEST_RDY_state = HIGH;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(PinCancelButton, INPUT);
  digitalWrite(PinCancelButton, HIGH); //turns on pull-up resistor
  attachInterrupt(0, EmergencyStopPushed, FALLING);
  pinMode(PinPress, OUTPUT); 
  pinMode(PinGoButton, INPUT);
  digitalWrite(PinGoButton, HIGH); //turns on pull-up resistor
  pinMode(PinCarraigeAdv, OUTPUT); 
  pinMode(PinSorter, OUTPUT); 
  pinMode(PinBoardGate, OUTPUT);   
  pinMode(PinUpperPhotoInt, INPUT); 
  pinMode(PinLowerPhotoInt, INPUT); 
  pinMode(PinAdvancePhotoInt, INPUT); 
  pinMode(PinRedLED, OUTPUT); 
  pinMode(PinYellowLED, OUTPUT); 
  pinMode(PinGreenLED, OUTPUT); 
  pinMode(PinCRSL_INITEST_RDY, INPUT);
  //digitalWrite(PinCRSL_INITEST_RDY, HIGH); //turns on pull-up resistor
  pinMode(PinCRSL_PFCLK, INPUT);
  //digitalWrite(PinCRSL_PFCLK, HIGH); //turns on pull-up resistor
  pinMode(PinCRSL_PFDAT, INPUT);
  //digitalWrite(PinCRSL_PFDAT, HIGH); //turns on pull-up resistor
  pinMode(PinCRSL_CLAMPED, OUTPUT);
  digitalWrite(PinCRSL_CLAMPED, HIGH);
  
  if (IsCancelPressed()) DemoMode = true;
}

// the loop routine runs over and over again forever: CurrentMode may ONLY be changed inside this function... (for code-maintenance sake)
void loop() {
  //pre-check special mode-change conditions
  if (CurrentMode == MODE_IDLE && EmergencyStop) EmergencyStop = false; //prevent/reset ESTOP in idle mode. Can't emergency stop when not doing anything!
  if (CurrentErrorType != ERR_NONE && CurrentMode != MODE_ERROR){AllOff(); WaitUntilCancelUnpressed(); CurrentMode = MODE_ERROR; } //catch if an error has been set!
  if (EmergencyStop && CurrentMode != MODE_ESTOPPED){AllOff(); WaitUntilCancelUnpressed(); CurrentMode = MODE_ESTOPPED;} //catch if emergency stop hit  (overrides error mode)
   
  switch (CurrentMode){
    case MODE_IDLE: 
      if (!checkTestBedStatus()) break; //if test bed is not present/ready, kick out of the switch statement to enter error mode next time around.
      RedOff(); YellowOff(); GreenOn();
      if (DemoMode) YellowOn(); //flag to show we're in demo mode
      
      if (IsGoPressed()) CurrentMode = MODE_BEGIN_WORKING;
      break;
      
    case MODE_BEGIN_WORKING:
      Purge(); 
      RedOff(); YellowOn(); GreenOff();
      CurrentMode = MODE_WORKING;
    break;
      
    case MODE_WORKING:
      RedOff(); YellowOn(); GreenOff();
      if (DemoMode) GreenOn();
      ProcessBoard();
      break;
      
    case MODE_ESTOPPED: //Cancel goes to idle mode. Go begins working mode.
       RedOn(); YellowOff(); GreenOff();
       if (DemoMode){ YellowOn();}
       CurrentErrorType = ERR_NONE; //clears any errors
       if (IsCancelPressed()){WaitUntilCancelUnpressed(); EmergencyStop=false; Purge(); CurrentMode = MODE_IDLE;}
       else if (IsGoPressed()){EmergencyStop=false; CurrentMode = MODE_BEGIN_WORKING;}
    break;
    
    case MODE_ERROR: // Sits flashing error code. Green button begins working again
      if (CurrentErrorType == ERR_TESTBED_SELFTEST || CurrentErrorType == ERR_TESTBED_NOT_READY){ //if we're in an error state that auto-corrects by inserting test cartridge or waiting for it to self-finish
        if (checkTestBedStatus()){ //if we pass a check status
          CurrentErrorType = ERR_NONE; //clears any errors
          CurrentMode = MODE_IDLE; //back to idle mode!
          break;
        }
      }
    
      if (IsGoPressed()){CurrentErrorType = ERR_NONE; CurrentMode = MODE_BEGIN_WORKING;}
      else FlashError(CurrentErrorType);
    break; //Cancel goes to EStopped mode via ISR
       
    //simple test modes
    case MODE_TESTLEDS: TestLEDs(); break;
    case MODE_TESTPHOTOINTS: TestPhotoInterruptor(); break;
    case MODE_TESTBUTTONS: TestButtons(); break;
    case MODE_TESTBOARDPRESS: TestBoardPress(); break;
    case MODE_TESTCARRAIGE: TestCarraige(); break;
    case MODE_TESTGARBAGE: TestGarbage(); break;
  }
}


boolean ProcessBoard(void){ //Processes a board from start (advancing carousel) to finish (dropping in good or bad pile)
  if (!checkTestBedStatus()) return false; //ensure test cartidge not removed

  if (!AdvanceCarraige()) return false; //If false, there will be an error code set within AdvanceCarraige()
  if (!WaitForBoardPass(PinUpperPhotoInt, 500)) return false; //If false, there will be an error code set within WaitForBoardPass()
  
  digitalWrite(PinPress, HIGH); // Activate main board press
  delay(PIN_PRESS_DELAY_MS); 
  
  // Test sequence, return result, handle result.
  if (!checkTestBedStatus()) return false; //ensure test cartidge  still all ready to go
  testResult = startTestAndGetResult(); 
  
  SortGoodOff(); //should already be off, but just making sure!
  switch (testResult){
    case TEST_PASS: //if pass, turn on green and move flapper to the good pile
      SortGoodOn();
      GreenOn();
      break;
    case TEST_FAIL: RedOn(); break;
    case TEST_TIMEOUT: CurrentErrorType = ERR_TESTBED_TIMEOUT; return false;
    case TEST_ABORT: default: return false; //exit the function with a fail

  }
  
  digitalWrite(PinPress, LOW); // Deactivate main board press
  delay(PIN_PRESS_DELAY_MS); //wait to reach state
  digitalWrite(PinBoardGate, HIGH); //Open the gate to fall through
  if (!WaitForBoardPass(PinLowerPhotoInt, 1000)) return false; //If false, there will be an error code set within WaitForBoardPass()
  digitalWrite(PinBoardGate, LOW); //Close gate
  GreenOff(); //clear LED pass/fail signal
  RedOff();
  SortGoodOff(); //release the board sorter
  
  return true;
}


// Added by Jon Moyes 3/7/2013
// Handles the status of the testbed
// Changed JH 3-12 to be consistent with only changing modes only from main loop
bool checkTestBedStatus(){ //returns true if the testbed is ready to go. Otherwise returns false with an CurrentErrorType set.
  if (DemoMode) return true; //Demo mode doesn't need a testbed inserted.
  
  switch ( getTestBedStatus() ){
    case BED_READY: return true;
    case BED_SELF_TEST:
      CurrentErrorType = ERR_TESTBED_SELFTEST;
      return false;
    case BED_NOT_READY: default: // Unknown testBedStatus return code, assume ERR_TESTBED_NOT_READY
      CurrentErrorType = ERR_TESTBED_NOT_READY;
      return false;
  }
}

// Added by Jon Moyes 3/7/2013. Formatted JH 3-12
bedStatus getTestBedStatus(){
  if (DemoMode) return BED_READY;
  
  if(testInProgress) return BED_TEST_BUSY; // Test is in progress
  else{ 
      // Sample CRSL lines
      pinCRSL_PFDAT_state = digitalRead(PinCRSL_PFDAT);
      pinCRSL_PFCLK_state = digitalRead(PinCRSL_PFCLK);
      pinCRSL_INITEST_RDY_state = digitalRead(PinCRSL_INITEST_RDY);
      
      if ( pinCRSL_PFDAT_state == LOW &&
           pinCRSL_PFCLK_state == LOW &&
           pinCRSL_INITEST_RDY_state == HIGH ) {
        // Bed inserted, self-test in progress
        return BED_SELF_TEST;
      }
      else if ( pinCRSL_INITEST_RDY_state == LOW ) return BED_READY; // Bed inserted, self-test pass, ready for normal operations
      else return BED_NOT_READY; // State unknown?  Assume BED_NOT_READY
  }
}

// Added by Jon Moyes 3/7/2013 Formatted JH 3-12
testResultCode startTestAndGetResult(){
  if (DemoMode){ //demo mode: dummy test, fail boards to never put bad boards in the good box!!!
    delay (750);
    return TEST_FAIL;
  }
  
  testInitPFCLKState = digitalRead(PinCRSL_PFCLK); // Sample the CRSL_PFCLK line for initial state, when the testbed is complete it will transition this line.
  digitalWrite(PinCRSL_CLAMPED, LOW); // Assert the CLAMPED pin low, this instructs the testbed to start it's operations
  testInProgress = true; // Flag to show that test is in progress  
  digitalWrite(PinCRSL_CLAMPED, HIGH); // Assert the CLAMPED pin high, this stops the testing after 1 test cycle
  
  
  unsigned long StartTimeMs = millis();
  while(testInProgress){  // Wait here for the testbed to finish it's test.  Testbed will transition the PFCLK line when it's done.
    if(digitalRead(PinCRSL_PFCLK) != testInitPFCLKState){testInProgress = false;}// Test Complete, break from waiting loop
    if (EmergencyStop) {testInProgress = false; return TEST_ABORT; } //Added JH 3-12
    if (millis()-StartTimeMs > TEST_BED_TIMEOUT){testInProgress = false; return TEST_TIMEOUT;} //Added JH 4-5
       
    delay(200); // Chill out!
  }
    
  // Sample the PFDAT line for test outcome, return outcome 
  if(digitalRead(PinCRSL_PFDAT) == HIGH) return TEST_FAIL; // PFDAT == HIGH --> FAIL
  else return TEST_PASS; // PFDAT == LOW  --> PASS
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
    
    if (TimeoutMs != -1 && millis()-StartTimeMs > TimeoutMs){
      if (PhotoIntPin == PinUpperPhotoInt) CurrentErrorType = ERR_UPPER_BOARD_PASS; 
      else if (PhotoIntPin == PinLowerPhotoInt) CurrentErrorType = ERR_LOWER_BOARD_PASS;
      else CurrentErrorType = ERR_GENERAL;
    return false;}
  }
  
  return false; //stopped prematurely from emergency stop
}

boolean AdvanceCarraige(void) { //andvances the carraige one slot. returns false with error code set if problems arise doing this
  if (EmergencyStop) return false;
  int TimeoutMs = 300;
  unsigned long StartTimeMs = millis();
  
  if (!IsOccluded(PinAdvancePhotoInt)){ CurrentErrorType = ERR_CARRAIGE_ADVANCE; return false;} //if not loaded or not aligned
  
  CarraigeEscapementOn();
  while (IsOccluded(PinAdvancePhotoInt)){
    if (EmergencyStop) return false;
    if (millis()-StartTimeMs > TimeoutMs){CarraigeEscapementOff(); CurrentErrorType = ERR_CARRAIGE_ADVANCE; return false; }//if didn't advance first step
    else (delay(5));
  }
  CarraigeEscapementOff();
  while (!IsOccluded(PinAdvancePhotoInt)){
    if (EmergencyStop) return false;
    if (millis()-StartTimeMs > TimeoutMs){CurrentErrorType = ERR_CARRAIGE_ADVANCE; return false;} //if didn't advance second step
    else (delay(5));
  }
  return true;
}


void FlashError(ErrorType ThisError) //flashes a unique LED sequence for the current error code
{
  YellowOff(); GreenOff();
  if (DemoMode) YellowOn();
  unsigned int repeatDelay = 500;
  switch (ThisError){
   case ERR_CARRAIGE_ADVANCE: PulseRedLong(); PulseRedShort(); PulseRedShort(); break;//long short short
   case ERR_UPPER_BOARD_PASS: PulseRedShort(); PulseRedLong(); PulseRedShort(); break;//short long short
   case ERR_LOWER_BOARD_PASS: PulseRedShort(); PulseRedShort(); PulseRedLong(); break;//short short long
   case ERR_GENERAL: PulseRed(1000, 200); break;
   case ERR_TESTBED_NOT_READY: PulseRedLong(); PulseRedShort(); PulseRedLong(); break; //long short long
   case ERR_TESTBED_TIMEOUT: PulseRedLong(); PulseRedLong(); PulseRedShort(); break; //long long short
   case ERR_TESTBED_SELFTEST: PulseGreenVeryShort(); PulseYellowVeryShort(); PulseRedVeryShort(); PulseYellowVeryShort(); PulseGreenVeryShort(); PulseYellowVeryShort(); PulseRedVeryShort(); PulseYellowVeryShort(); repeatDelay = 100; break;

  } 
  if (!EmergencyStop) delay(repeatDelay); //distinguish when on repeat
}

//ISR
void EmergencyStopPushed(void){EmergencyStop = true;} //ISR handler to set emergency stop flag

void AllOff(void){GreenOff(); YellowOff(); RedOff(); BoardPressOff(); CarraigeEscapementOff(); BoardGateOff(); SortGoodOff();} //deactivates EVERYTHING (safe to sit without cooking a solenoid or blowing air out a leaky pnuematic cylinder)
void Purge(void){YellowOn(); BoardGateOn(); delay(200); BoardGateOff(); delay(200);} //Clears a board that may be left inside the machine to the "bad" pile

//LED Utility functions
void RedOn(){digitalWrite(PinRedLED, HIGH);}
void RedOff(){digitalWrite(PinRedLED, LOW);}
void YellowOn(){digitalWrite(PinYellowLED, HIGH);}
void YellowOff(){digitalWrite(PinYellowLED, LOW);}
void GreenOn(){digitalWrite(PinGreenLED, HIGH);}
void GreenOff(){digitalWrite(PinGreenLED, LOW);}

void PulseRedLong(){PulseRed(400, 150);}
void PulseRedShort(){PulseRed(150, 300);}
void PulseRedVeryShort(){PulseRed(50, 100);}
void PulseRed(int MsOn, int MsOff){if (EmergencyStop) return;  RedOn(); delay(MsOn); RedOff(); delay(MsOff);} //pulses the red LED on for MsOn the waits MsOff

void PulseYellowLong(){PulseYellow(400, 150);}
void PulseYellowShort(){PulseYellow(150, 300);}
void PulseYellowVeryShort(){PulseYellow(50, 100);}
void PulseYellow(int MsOn, int MsOff){if (EmergencyStop) return;  YellowOn(); delay(MsOn); YellowOff(); delay(MsOff);} //pulses the red LED on for MsOn the waits MsOff

void PulseGreenLong(){PulseGreen(400, 150);}
void PulseGreenShort(){PulseGreen(150, 300);}
void PulseGreenVeryShort(){PulseGreen(50, 100);}
void PulseGreen(int MsOn, int MsOff){if (EmergencyStop) return;  GreenOn(); delay(MsOn); GreenOff(); delay(MsOff);} //pulses the red LED on for MsOn the waits MsOff

void TestLEDs(void){RedOn(); delay(333); RedOff(); YellowOn(); delay(333); YellowOff(); GreenOn(); delay(333); GreenOff();} //pulses each one for 1/3 of a second

//PhotoInterruptor utility functions
boolean IsOccluded(int PhotoIntPin){return digitalRead(PhotoIntPin) == LOW;}
void TestPhotoInterruptor(void){if (IsOccluded(PinAdvancePhotoInt)) RedOn(); else RedOff(); if (IsOccluded(PinUpperPhotoInt)) YellowOn(); else YellowOff(); if (IsOccluded(PinLowerPhotoInt)) GreenOn(); else GreenOff();} //LED lights when sensor is occluded

//Button Utility functions
boolean IsGoPressed(void){for (int i=0; i<10; i++){if (digitalRead(PinGoButton)==HIGH) return false; delay(2);} return true;}; //reads 10 times to avoid bounce
boolean IsCancelPressed(void){for (int i=0; i<10; i++){if (digitalRead(PinCancelButton)==HIGH) return false; delay(2);} return true;}; //reads 10 times to avoid bounce
void WaitUntilCancelUnpressed(void){int NumHighInARow = 0; while(NumHighInARow<10){if (digitalRead(PinCancelButton)==LOW) NumHighInARow = 0; else NumHighInARow++; delay(2);}}//hangs out inside function until cancel button is un-pressed

void TestButtons(void) {while(true){if (IsGoPressed()) GreenOn(); else GreenOff();   if (IsCancelPressed()) RedOn(); else RedOff();}} //red lights up when Cancel pressed, green when Go pressed

//Pneumatic Utility functions
void BoardPressOn(void) {digitalWrite(PinPress, HIGH);}
void BoardPressOff(void) {digitalWrite(PinPress, LOW);}
void CarraigeEscapementOn(void) {digitalWrite(PinCarraigeAdv, HIGH);}
void CarraigeEscapementOff(void) {digitalWrite(PinCarraigeAdv, LOW);}
void SortGoodOn(void) {digitalWrite(PinSorter, HIGH);}
void SortGoodOff(void) {digitalWrite(PinSorter, LOW);}

void TestBoardPress(void) {while(true){if (IsGoPressed()){BoardPressOn(); YellowOn();} else if (IsCancelPressed()){BoardPressOff(); YellowOff();}}} //cylinder activates when green pressed and deactivates when red pressed.
void TestCarraigeBasic(void) {while(true){if (IsGoPressed()){CarraigeEscapementOn(); YellowOn();} else if (IsCancelPressed()){CarraigeEscapementOff(); YellowOff();}}} //Advances carraige every time green button pressed. 
void TestCarraige(void) {while(true){EmergencyStop = false; if (IsGoPressed() && !AdvanceCarraige()){ YellowOn(); delay(500); FlashError(ERR_CARRAIGE_ADVANCE); CurrentErrorType = ERR_NONE;} else delay(20);}} //Advances carraige every time green button pressed. 
void TestGarbage(void) {while(true){if (IsGoPressed()){SortGoodOn(); YellowOn();} else if (IsCancelPressed()){SortGoodOff(); YellowOff();}}} //cylinder activates when green pressed and deactivates when red pressed.


//Solenoid Utility functions
void BoardGateOn(void) {digitalWrite(PinBoardGate, HIGH);}
void BoardGateOff(void) {digitalWrite(PinBoardGate, LOW);}
//void BardGateOpen(void){BoardGateOn(); delay(200); BoardGateOff();} //TODO: use lower photoint to verify this...?

//Communication utilities




