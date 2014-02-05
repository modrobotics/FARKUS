

  //  0x = Utility blocks
#define Passive_block	    	00 //= Passive_block, should passive calculate a Block Value and pass it on, currently it does not.
#define Blocker_block    		01 //= Blocker_block //  can't test on pogo. maybe start as passive, and after uptime of x switch to passive
#define Power_block    			02 //= Power_block 
#define Comm_block    			03 //= Comm_block 		// can't test on pogo
#define	Bluetooth_block 		04 //= Bluetooth_block //  make clean; make MyID0=105 BLOCK_TYPE=05 F_CPU=16000000UL USB_OBJ_0=usbdrvasm.o USB_OBJ_1=oddebug.o
#define	Usb_block		 		05 //= USB Block // make clean;
#define	Serial_bootloader		06 //= HAS_USB_BOOTLOADER //
#define	Serial_bootloader_surrogate		07 //A figure of authority who takes the place of the father or mother in a person's unconscious or emotional life.



//  2x = Sensor blocks
#define Light_block    			21 //= Light_block
#define Knob_block    			22 //= Knob_block
#define	Touch_block 			23 //= Touch_block
#define	Distance_block 			24 //= Distance_block
#define Temperature_block    	25 //= Temperature_block
#define	Constant_block			26 //= Constant_block

#define	MOBO_POGO		 		27 //= POGO Test code for MOBO/ 
#define	DOBO_POGO		 		28 //= POGO Test code for DOBO NOT implimented
 
  //  4x = Action blocks
#define  LED_Bar_Graph_block   	43 //= LED_Bar_Graph_block  // Fails for LED bar (IC2 issue) // fixed by timing out the IC2 com functions
#define	 Tread_block 			45 //= Tread_block (dc motor)
#define  Rotation_block   		46 //= Rotation_block (dc motor)
#define  Speaker_block   		47 //= Speaker_block
#define  Flashlight_block   	48 //= Flashlight_block  not working, use motor block!!
#define	 Hinge_block 			49 //= Hinge_block (servo)
 
  //  6x = Think blocks
#define  Sum_block  			60 //= Sum_block			yellow
#define  Inverse_block   		61 //= Inverse_block		red
#define  Max_block   			62 //= Max_block			blue
#define  Min_block   			63 //= Min_block			green
#define	 Threshold_block 		64 //= Threshold_block	

#define	 Function_Ramp_block 	65 //= Function	
#define	 Function_Square_block 	66 //= Function	
#define  Function_Sin_block 	67

#define value_of_other  64


//  I each byte of the packet is defined for each packet type.  I need to go through and note them here.
#define PT_Classic			0x00
#define PT_Map				0x10  // PT_Map_X =>face*2+ID+1+PT_Map bit stuffed with face and ID "m"
//					....
//					...
//					...
//					.....
//
#define PT_Map_Face			0x11
#define PT_Map_ID0			0x12
#define PT_Map_ID1			0x13
#define PT_Drive			0x20	// "d"
#define PT_Listen			0x30	// "l"
#define PT_Ping				0x40	// "p"
#define PT_Blink			0x41	// "b"
#define PT_Delete			0x50

#define PT_BT			0x14

#define PT_Boot				0x60	//96 Jump to boot loader
#define PT_Jump2Boot				0x61	//96 Jump to boot loader
//#define PT_Program			0x61	//(once in boot land packet could totally change shape?) addy0 addy1 value, value, cksum, PT
//#define PT_Program_respond	0x62	//(once in boot land packet could totally change shape?) addy0 addy1 value, value, cksum, PT
#define PT_I_am_alive		0x63	// in Safe mode this packet is added to the buffer to help test block com.
#define PT_U_R_Safe			0x64	// if a freshly flashed mobo in safe mode heres this command it knows com is good and it can toggel the Safe Bit.


//ORIGINALLY COMMENTED OUT BY MILES
#define PT_WritePage		0x65	//101 move the page from the page loader to flash
//#define PT_SetPage			0x66	//102 set the page number

//ORIGINALLY COMMENTED BY MILES
#define PT_ProgramPage		0x61	//97 (once in boot land packet could totally change shape?) addy1 addy0 data1 data0 TO HP

#define PT_ProgramEntirePage 	0x67	// This packet Says that the next 128 bytes of the buffer or untill a pageend packet is page data! it also clears the buffer and tells the usb cube not to talk
//								[target Adress A0,A1,A2] [page#] [HP][TO][PT]
#define PT_Clear_Buffer		 	0x69
#define PT_ProgramEntirePageEnd	0x68	// clear the data store
//								[target Adress ] [page checksum] [HP][TO][PT]
#define PT_Shhh			0x6A		//robot be quite
#define PT_Party			0x6D		//robot talk
#define PT_Fill_Page_Data	0x6C			//using a clasic packet for now so that other cubes don't trip out on the data, and so that i can look at the BV for debug
#define PT_Check_SUM			0x6B			// This packet returns the last varaified page.  Tells power switch to move on to the next page.
#define PT_Target2BT			0x01
//								[XXX] [varified page #] [HP][TO][PT]

#define PT_DiffuseIDs	0x6F

#define PT_GetFullMap				0x6E
#define PT_DeleteMapPackets			0x62
#define PT_AllowResendingOfFullMap	0x55

#define PT_Neighbour_OnFace_0		0x80
#define PT_Neighbour_OnFace_1		0x81
#define PT_Neighbour_OnFace_2		0x82
#define PT_Neighbour_OnFace_3		0x83
#define PT_Neighbour_OnFace_4		0x84
#define PT_Neighbour_OnFace_5		0x85

#define PT_TurnOnMyLed			0x87
#define PT_TurnOffMyLed			0x88

#define PT_SetBlockValue		0x86
#define PT_RemoveSetBlockValue	0x89

#define PT_SendMyMapInfo		0x59 //brute force full map hack

#define PT_SendMyBatteryLevel		0x30
#define PT_MyBatteryLevel		0x31


///pic PT//////////////////////////////////////////////
#define PT_Pic_Goto_Bootloader		0x62
#define PT_Pic_Led_Off		0x66
#define PT_Pic_Ping_Neighbor		0x70
#define PT_Pic_Ping_Ack 		0x71
#define PT_Flashing_Start 		0x72
#define PT_Pic2BT_Check			0x73
#define PT_Pic_Ping_Ack2		0x90
#define PT_Pic2BT_Check2		0x84

/*
#define PT_Map_Closed			0xED
#define PT_Map_Open				0xEE
#define PT_Map_PathBack			0xEF

#define PT_UsefulMapInfo		0xE0
*/

// remap the AD for specific sensors y=mx+b having trouble with floats
#define	TVmax 1.5	// just over body temp // in practice i think these need to be dynamic based on current conditions.  These points failed miserably when the ac went out!
#define TVmin 1.2	// just under room temp
#define	Vcc 2.5		// 
#define temp_prescaler_m  2		//#define temp_prescaler_m  Vcc/(4*(TVmax-TVmin))  // these are approximate equations, you just have to solve them your self
#define temp_prescaler_b  -900			//#define temp_prescaler_b  -0xFF*TVmin/(TVmax-TVmin)		 // these are approximate equations, you just have to solve them your self

#define	DVmax 2.5	// 
#define DVmin .4	// 
#define	Vcc 2.5		// 
#define dist_prescaler_m  1 //0.3		//#define temp_prescaler_m  Vcc/(4*(DVmax-DVmin))  // these are approximate equations, you just have to solve them your self
#define dist_prescaler_b  -15			//#define temp_prescaler_b  -0xFF*DVmin/(DVmax-DVmin)		 // these are approximate equations, you just have to solve them your self

#define Threshold_Var 128


//#define F_CPU 8000000UL  // 8 MHz sMiles in make file
//#define F_CPU 16000000UL


// USB commands
#define PSCMD_ECHO  0		// test	
#define PSCMD_GET   1		// returns block value
//#define PSCMD_drive    2	// depreciated 
//#define PSCMD_OFF   3		// depreciated 
//#define Read_Sig   0x30		// depreciated 
//#define PSCMD_ON	100		// depreciated 
#define PSCMD_ADDPACKET 101 // add packet to buffer
//#define PSCMD_SETPAGE 103 	// depreciated 
//#define PSCMD_CUBE_FOCUS 104 // run as a cublet
//#define PSCMD_USB_FOCUS 105 // focus on USB connection
//#define PSCMD_Fast_Page 106 // focus on USB connection
#define PSCMD_Page_Check_Sum 107 // 
#define PSCMD_Upload_Data_Buffer 102 // upload buffer to computer
#define PSCMD_Upload_Neighbor_Map 103
/* These are the vendor specific SETUP commands implemented by our USB device */



volatile unsigned char BlockValue;
volatile unsigned char value_to_send;





void ScopeHigh(void);
void ScopeSwitch(void);
//void BlueOff(void);
void RedOff(void);
void GreenOff(void);
void LedSwitch(void);
void ScopeLow(void);
//void BlueOn(void);
void RedOn(void);
void GreenOn(void);
void Mux(unsigned int);
void FlushBuffer(void);
void EnableMux(void);
//void init_pins(void);
void InitializeMCU(void);
void InitializeTimer0(void);
void USART_Transmit(unsigned char);
void USART_Transmit_BT(unsigned char);  //new and not used yet
//void EnanleTx(void);
void DisableTx(void);
void TxInput(void);
void TxOutputLow(void);
void RxInput(void);
void RxOutputLow(void);
//void EnablePCI(void);
void DisablePCI(void);
void EnableRx(void);
void DisableRx(void);
void PCIonTx(void);
void PCIonRx(void);

void InitializeTimer2(void);

void EnableTimer2(void);
void DisableTimer2(void);

void DisableTimer0(void);

void InitializeUART(void);
void EnableTransmitter(void);
void EnableTransmitter_BT(void);
void DisableTransmitter(void);
void EnableReceiver(void);
void EnableReceiver_BT(void);
void DisableReceiver(void);
void InitializeUARTforBLUETOOTH(void);
void EnterBluetoothCMDmode(void);

void InitializeUART2(void);
void ProcessData(void);
//int RandomNumber(unsigned int); //  replaced with a simpler function

void ClearInputBuffer(unsigned int);

void CalculateChecksums(void);
void IncrementHopCount(void);
void UpdateBuffer(void);
//void CalculateBlockValue(void);
void CalculateBlockValue2(void);
void Actuate(void);
void Actuate2(void);

void AddPacket(unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char); 
void DeletePacket(unsigned int);


//for bar graph:::::::::::::
void ResetSPI(void);
void InitializeMCUspi(void);
void InitializeI2C(void);

void I2CsendLS0(void);
void I2CsendLS1(void);
void I2CsendLS2(void);

void ERROR(void);
void DisplayBarGraph(unsigned char);

//knob
void InitializeADCknob(void);

//distance
void InitializeADCdistance(void);

//flashlight
void InitializeTimer1Flashlight(void);

///motor
void InitializeTimer1Motor(unsigned char);
unsigned char motor_direction; //0=clockwise , 1=anti-clockwise

//speaker
void InitializeTimer1(void);  //old
void InitializeMCUspeaker2(void);  //old

void InitializeMCUspeaker(void);
void InitializeTimer1Speaker(void);

//sMiles
void error_code(char code);			// for debug
void not_error_code(char code);		// for debug
void ProcessDataPacketType(void);	// parse the new packet for op_codes
char isSense(void);					// fixed the warnings
char isThink(void);
char isAction(void);
void EnableTimer0(void);
void InitializeADCtempature(void);	// should makes this more general at least three sensors can use this
void Sense(void);

void Talk(void);

void SendMap_BT(void);
void SendMap_BT2(void);
void Send_ID_Diffusion_BT(void);

//debug
void transmit_BT_ascii_num(unsigned char);
