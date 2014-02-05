//FUSE
//brown out 2.7V
//un program ckdiv8
//unprogram jtag
//Ext Crystal Osc 8MHz 16k ck + 4.1ms
//bootrst
//bootflash size = 1024 words
//wdton
#include "./define.h"

//#include <avr/signal.h> 
#include <stdio.h> 

// Includes					// Will compile with out most of these!!
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#include <math.h> //to include the math library see http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_libm
//#include <avr/eeprom.h> // remember varibles after reboot.
#include <avr/wdt.h>  
#include <avr/boot.h>  

#include "./define_ids.h"

#include <util/delay.h>
//void (*boot)(void)=0x1C00;  //word address of bootloader



void ProgramPage(unsigned int,unsigned int); 
void USART_Transmit_BT(unsigned char);   
unsigned char CheckSums(void);
unsigned char SPI_Tx_Rx(unsigned char);
void mem_read(unsigned long int, unsigned char);
void ByteWriteSingle(unsigned long int, unsigned char); //add,byte
void BlockErase(void);
void SendMap_BT_Bootloader(void);

static void (*nullVector)(void) __attribute__((__noreturn__));

static void leaveBootloader()
{
  
   
   cli();
   GICR = (1 << IVCE);     /* enable change of interrupt vectors */
   GICR = (0 << IVSEL);    /* move interrupts to application flash section */
/* We must go through a global function pointer variable instead of writing
*  ((void (*)(void))0)();
* because the compiler optimizes a constant 0 to "rcall 0" which is not
* handled correctly by the assembler.
*/
   nullVector();
}

#define RecArrayDepth  132 
volatile unsigned char ReceivedByte_BT=0xdd;
volatile unsigned char RecArray_BT[RecArrayDepth]={};
volatile unsigned int RecNo_BT=0;
volatile unsigned char reset=0;
volatile unsigned int rec_address=0;
//volatile unsigned char page_buffer[SPM_PAGESIZE];
volatile unsigned char page_buffer[132]; //128 data bytes + 2 addresses + 2 checksums
volatile unsigned char ck=0;

volatile unsigned char sum=0;
volatile unsigned char xor=0;
volatile unsigned char led=0;
volatile unsigned char reprog_bt=0;
volatile unsigned char receiving_led=0;
volatile unsigned char na=0;
volatile unsigned char busy=1;
volatile unsigned char status=0;
volatile unsigned char flashing_bt=0;
volatile unsigned char coming_from_app=0; //undefined

volatile unsigned char mem_rec[2]={}; //2 length max

// Moved these variables to define_ids.h for production
//uint16_t ID2 __attribute__ ((section(".id2section"))) =1;// 0;//0;//0; 
//uint16_t ID1 __attribute__ ((section(".id1section"))) =2;// 0x25;////10 
//uint16_t ID0 __attribute__ ((section(".id0section"))) =3;// 0x26;////46 

volatile unsigned char MyID2=0;//1;
volatile unsigned char MyID1=0;//2;
volatile unsigned char MyID0=0;//10;
volatile unsigned char wait_for_t=0;
volatile unsigned char safe=2; //undefined

int main()
{
	cli();

	coming_from_app=0;
	 wdt_enable(WDTO_2S); //original
	 wdt_reset(); //original

	InitializeMCU();

	safe = eeprom_read_byte((uint8_t*) 30);
	EEAR=50;// dont leave parked at useful address

	if(safe==1)
	{
		leaveBootloader();
	}

	//	RN42(38);

	  MyID0=pgm_read_byte_near(0x3FFA);//2
   MyID1=pgm_read_byte_near(0x3FFC);//2
    MyID2=pgm_read_byte_near(0x3FFE);//2




	InitializeUARTforBLUETOOTH();
	EnableTransmitter_BT();
	EnableReceiver_BT();

	

	flashing_bt=0;

	/*
	InitializeSPI();


		
	AssertCS();
	na=SPI_Tx_Rx(0x06); //send opcode for Write Enable
	DeassertCS();

	AssertCS();
	na=SPI_Tx_Rx(0x39); //send opcode for Unprotect section
	na=SPI_Tx_Rx(0x00); //address to unprotect (sector 0)
	na=SPI_Tx_Rx(0x00);
	na=SPI_Tx_Rx(0x00);
	DeassertCS();
	*/

	/*
	AssertCS();
	na=SPI_Tx_Rx(0x39); //send opcode for Unprotect section
	na=SPI_Tx_Rx(0x07); //address to unprotect (sector 8)
	na=SPI_Tx_Rx(0x80);
	na=SPI_Tx_Rx(0x00);
	DeassertCS();
	
	*/

	//mem_read(0x078000,1); //add,size  --start of secor 8
	//answer in mem_rec[1];
	//coming_from_app = mem_rec[1];
	coming_from_app = eeprom_read_byte((uint8_t*)10);
	EEAR=50;// dont leave parked at useful address

	/*
	BlockErase();
	ByteWriteSingle(0, 0x24);

	mem_read(0,1); //add,size
	//answer in mem_rec[1];
	*/


	
		GreenOn();
	
    GICR = (1 << IVCE);  /* enable change of interrupt vectors */
    GICR = (1 << IVSEL); /* move interrupts to boot flash section */



	sei();

//	USART_Transmit_BT('!'); //tell pc that bootloader is ready --TO ADD LATER WHEN COMING FROM APPLICATION
	RecNo_BT=0;


	if(coming_from_app==1)
	{
	
		coming_from_app=0;
		wait_for_t=0;
		//reset the bit in memory
			eeprom_write_byte((uint8_t*)10,0);
			EEAR=50;// dont leave parked at useful address

		flashing_bt=1;
		USART_Transmit_BT('!');
		RecNo_BT=0;
	
	}
	else
	{
		wait_for_t=1;
	}

	while(1)
	{
		if(reset==0) 
		{
			wdt_reset();//original///////////////////////////
		}
			if(RecArray_BT[0]=='L' && flashing_bt==0 && receiving_led==0) //t_f==1 )//&& reprogramming==0)
			{
				//SendMap_BT();
				RecNo_BT=0;
				RecArray_BT[0]=0xdd; //new
			leaveBootloader();
	

			}

			if(RecArray_BT[0]=='m' && flashing_bt==0 && receiving_led==0) //t_f==1 )//&& reprogramming==0)
			{
				//SendMap_BT();
				RecNo_BT=0;
				RecArray_BT[0]=0xdd; //new
				//SendMap_BT2();
				SendMap_BT_Bootloader();
				RecNo_BT=0;
			}

			if(RecArray_BT[0]=='a' && flashing_bt==0 && receiving_led==0)// alive signal and full map
			{
					RecNo_BT=0;
					RecArray_BT[0]=0xdd;
				//ReceivedByte_BT=0xdd;	
				USART_Transmit_BT('<');
				USART_Transmit_BT('l'); //send im alive
				USART_Transmit_BT(0);
				USART_Transmit_BT('>');
				RecNo_BT=0;
	
			}

		//re-flashing Bluetooth Block:
			if(RecArray_BT[0]=='T' && flashing_bt==0 && receiving_led==0 && wait_for_t==1)// alive signal and full map //PIC
			{

				    receiving_led=1;
					//RecNo_BT=0;
		

				if( RecNo_BT == 4 )  // T + 3 ID's
				{

					if(RecArray_BT[1]==MyID2 && RecArray_BT[2]==MyID1 && RecArray_BT[3]==MyID0)//IDs match
					//if(RecArray_BT[1]==0 && RecArray_BT[2]==0x25 && RecArray_BT[3]==0x26)//IDs match
					{
							//boot();
							//GreenOn();
							USART_Transmit_BT('!');
							RecNo_BT=0;
							flashing_bt=1;
							wait_for_t=0;
					}
						


				}
			
				receiving_led=0;
			}

		/*
		if(ReceivedByte_BT == 'x')
		{
			BlockErase();
			ReceivedByte_BT=0xdd;
			ByteWriteSingle(0,'x');
		}
		
		if(ReceivedByte_BT == 'r')
		{
			ReceivedByte_BT=0xdd;
		//	mem_read(0,1); //add,size
			USART_Transmit_BT(MyID2);
			USART_Transmit_BT(MyID1);
			USART_Transmit_BT(MyID0);
		}
		if(ReceivedByte_BT == 's')
		{
			ReceivedByte_BT=0xdd;
		//	mem_read(0,1); //add,size
			USART_Transmit_BT('s');
			USART_Transmit_BT('t');
			USART_Transmit_BT(MyID0);
		}
		*/

	}//end while(1)
	return 0;
}//end main


ISR(USART1_RXC_vect)
{

  	ReceivedByte_BT = UDR1;
   	RecArray_BT[RecNo_BT]=ReceivedByte_BT;
   	RecNo_BT++;  //buffer index
	
	
	if(RecArray_BT[0]==0xFE && RecArray_BT[1]==0xFD) //if file is over
	{
		//BUG: LEAVE BOOTLOADER IS CALLED TOO SOON AFTER SENDING THE CHARACTER

			RecNo_BT=0;
		flashing_bt=0;
		ReceivedByte_BT=0xdd;

		USART_Transmit_BT('@'); //tell PC that we will just start in application.
	//	USART_Transmit_BT('@'); //tell PC that we will just start in application.
		
	//		while ( !( UCSR1A & (1<<UDRE1)) )
	//		;
	
		/*	
		for(unsigned long int c=0; c<100000;c++)
		{
			unsigned char x=c;
		}
		*/
		_delay_ms(10);  //at 9600 baud, wait for last '@' to be sent before jumping to application.
		

	
		RecArray_BT[0]=0xdd;
		RecArray_BT[1]=0xdd;
		
		leaveBootloader();
	}
	

	/*
	if(RecArray_BT[0]=='x') //if file is over
	{
		USART_Transmit_BT('X');
	}
	*/

	if(RecNo_BT>=132)
	{
		switch(led)
		{
			case 0:
			led=1;
			GreenOn();
			break;

			case 1:
			led=0;
			GreenOff();
			break;
		}

		ck=CheckSums();
		//ck=1;
		if(ck==1)
		{	
			rec_address= ( ( (unsigned int)RecArray_BT[0])<<8 )  + (  (unsigned int)RecArray_BT[1] );

			for(unsigned char j=0; j<128; j++)
			{
				page_buffer[j]=RecArray_BT[j+4];
			}	
			if(rec_address<0x3800)
			{
				ProgramPage(128,rec_address);// /2 
				USART_Transmit_BT('G');
			}
			else
			{
				USART_Transmit_BT('B');
			}
			
		}
		else
		{
			USART_Transmit_BT('B');
			//USART_Transmit_BT(xor);	
		}
		RecNo_BT=0;
	}
	
	
}

unsigned char CheckSums(void)
{
	sum=0;
	xor=0;

	for(unsigned char u=4; u<132; u++) //4-132: 128 bytes
	{
		sum = sum + RecArray_BT[u];
		xor = xor ^ RecArray_BT[u];
	}

	//add the two address bytes to the checksum calculations:
	sum = sum + RecArray_BT[0] + RecArray_BT[1];
	xor = xor ^ RecArray_BT[0];// ^ RecArray_BT[1];
	xor = xor ^ RecArray_BT[1];

	if(sum==RecArray_BT[2] && xor==RecArray_BT[3])
	{
		return 1;
	}
	else
	{
		return 0;
	}


		 
}

void ProgramPage(unsigned int size, unsigned int address) 
{ 
   unsigned int tempAddress = address; 

   unsigned int i;
   unsigned int tempWord; 


	/*
   // store values to be programmed in temporary buffer 
   for (cnt=0; cnt<UART_RX_BUFFER_SIZE; cnt++) { 
      if (cnt<size) pageBuffer[cnt]=receiveByte(); 
      else pageBuffer[cnt]=0xFF; 
   } 
   cnt=0; 
  */

	//manually fill pageBuffer:
	/*
	 for (i=0; i<128; i++) 
	 { 
		page_buffer[i]=0xCD;
	 }
	*/

    boot_page_erase(address);   // Perform page erase 
    boot_spm_busy_wait();      // Wait until the memory is erased. 

    for(i = 0; i < size; i=i+2) 
   { 
      tempWord = page_buffer[i]; // load the little end then increment i 
      //tempWord += (page_buffer[i+1] << 8); // load the big end 
	  tempWord = tempWord +  (page_buffer[i+1] << 8); // load the big end 
        boot_page_fill(address,tempWord); 
                          
        address = address + 2;     // word increment 
    } 
    
    boot_page_write(tempAddress); 
    boot_spm_busy_wait();    
    boot_rww_enable();            // Re-enable the RWW section    

   //sendByte('\r'); 
} 



void RN42(unsigned char baud)
{
	switch(baud)
	{
		case 38:
			USART_Transmit_BT('$'); //cmd mode
			USART_Transmit_BT('$');
			USART_Transmit_BT('$');

			USART_Transmit_BT('S'); //38.4K
			USART_Transmit_BT('U');
			USART_Transmit_BT(',');
			USART_Transmit_BT('3'); 
			USART_Transmit_BT('8');

			//enter:
			USART_Transmit_BT(0x0D); //CR
			USART_Transmit_BT(0x0A); //LF

			USART_Transmit_BT('-'); //exit cmd mode
			USART_Transmit_BT('-');
			USART_Transmit_BT('-');

			//enter:
			USART_Transmit_BT(0x0D); //CR
			USART_Transmit_BT(0x0A); //LF

			//do some checks on ACK etc:


		break;

		case 96:

				USART_Transmit_BT('$'); //cmd mode
			USART_Transmit_BT('$');
			USART_Transmit_BT('$');

			USART_Transmit_BT('S'); //38.4K
			USART_Transmit_BT('U');
			USART_Transmit_BT(',');
			USART_Transmit_BT('9'); 
			USART_Transmit_BT('6');

			//enter:
			USART_Transmit_BT(0x0D); //CR
			USART_Transmit_BT(0x0A); //LF

			USART_Transmit_BT('-'); //exit cmd mode
			USART_Transmit_BT('-');
			USART_Transmit_BT('-');

			//enter:
			USART_Transmit_BT(0x0D); //CR
			USART_Transmit_BT(0x0A); //LF

			//do some checks on ACK etc:

		break;
	}
}


	//GETTING BLUETOOTH RUNNING STUFF
void InitializeUARTforBLUETOOTH(void)
{
	//initialize uart1 for bluetooth
	//115200 bps, parity none, 8bits, 1 stop bit, hrdarew flow control enabled.

	UCSR1B |= _BV(RXCIE1); //RX complete interrupt enable
	//UCSR1B |= _BV(7);

	UCSR1A |= _BV(U2X1); // improves baud rate error
	//UCSR1A |= _BV(1);	

	////////careful..--------need to access UBRL register !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	//	UBRR1L=8; //115.2K
//		UBRR1L=103; //9600 original

	//	UBRR1L=51; //19.2k
	UBRR1L=25; //38.4k //pretty good //original for bootloader

	//UCSR1C |= _BV(UCSZ11) | _BV(UCSZ10);  //8bit
	//UCSR1C |= _BV(2);
	//UCSR1C |= _BV(1);

	//UCSR1C = (1<<7)|(1<<2)|(1<<1); //8bit
	UCSR1C = (1<<7)|(1<<UCSZ11)|(1<<UCSZ10); //8bit
}

void EnableTransmitter_BT(void)
{
	UCSR1B |= _BV(TXEN1); 
	//UCSR1B |= _BV(3); 
}

void EnableReceiver_BT(void)
{
	UCSR1B |= _BV(RXEN1); 
	//UCSR1B |= _BV(4);
}

void USART_Transmit_BT(unsigned char data)   
{
	//RedOn();
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) )
	;
	/* Put data into buffer, sends the data */
	UDR1 = data;

	
}


void InitializeMCU(void)
{

	DDRA |= _BV(2); //RTS to blutooth chip--------------
	
	DDRC |= _BV(2); //high LED OUT------------------

	PORTC &= ~_BV(2); //low led off-----------------

	DDRC |= _BV(7);	//bluetooth reset---------------------
	PORTC |= _BV(7); //high  do not reset----------------
	
	DDRC &= ~_BV(6); //bluetooth status -=-----or pcint14 now (need jumper)--------

	//mux port selection - same
	DDRD |= _BV(3);	//s0	-------------------------------
	DDRD |= _BV(4);	//s1----------------------------------
	DDRD |= _BV(5);	//s2-----------------------------------


	DDRB |= _BV(1);	//nothing  //smiles---------------
	DDRE |= _BV(2);	//pwm-----------------------

	
	DDRB &= ~_BV(4);	//CTS from blutooth chip--------------


	DDRD &= ~_BV(1); //low //tx pin input - same-------------
	DDRD &= ~_BV(0); //rx pin input - same-------------

	DDRB |= _BV(3);	//out - BT_tx----------------------------

	DDRB &= ~_BV(2); //in - BT_rx--------------------------
	PORTB &= ~_BV(2);	


	DDRC &= ~_BV(5); //bluetooth status -=-

}


void GreenOn(void)
{
	PORTC |= _BV(2); //high  led on
}
void GreenOff(void)
{
	PORTC &= ~_BV(2); //low led off
}





void SendMap_BT_Bootloader(void)
{
	/*
	if(currently_flashing != 0){
		return;
	}
	*/


	USART_Transmit_BT('<');
	USART_Transmit_BT('n');
	
	USART_Transmit_BT(21); //new

	USART_Transmit_BT('>');  //new
	
	USART_Transmit_BT(MyID2);
	USART_Transmit_BT(MyID1);
	USART_Transmit_BT(MyID0);

//	USART_Transmit_BT(BLOCK_TYPE);
	//transmit_BT_ascii_num(MyID2);
	//transmit_BT_ascii_num(MyID1);
	//transmit_BT_ascii_num(MyID0);

	ReceivedByte_BT=0xdd;

	
	for(unsigned char mc=0; mc<18;mc++)
	{
		USART_Transmit_BT(0); //dummy neighbours
	}
	

	/*
	for(unsigned char mc=0; mc<=2 ; mc++)
	{
		USART_Transmit_BT(0xee);
	}	
	for(unsigned char mc=4; mc<=6 ; mc++)
	{
		USART_Transmit_BT(0xee);
	}
	for(unsigned char mc=8; mc<=10 ; mc++)
	{
		USART_Transmit_BT(0xee);
	}
	for(unsigned char mc=12; mc<=14 ; mc++)
	{
		USART_Transmit_BT(0xee);
	}
	for(unsigned char mc=16; mc<=18 ; mc++)
	{
		USART_Transmit_BT(0xee);
	}
	for(unsigned char mc=20; mc<=22 ; mc++)
	{
		USART_Transmit_BT(0xee);
	}
	*/
//	USART_Transmit_BT('>');  //new
}





/*
void InitializeSPI(void)
{
	//talk to external flash memoert AT25DF041A-SSHF-B
	//b7 sck
	//b6 miso
	//b5 mosi
	//b4 ss'/ cs'
		
	// Set MOSI and SCK outp?t, all others input 
	//DDR_SPI = (1<<DD_MOSI)|(?<<DD_SCK);
	DDRB |= _BV(4);	//output chip select
	PORTB |= _BV(4);	// chip select initially high
	
	DDRB &= ~_BV(6); //input miso

	DDRB |= _BV(5);	//output mosi
	DDRB |= _BV(7);	//output sck

	//PORTB &= ~_BV(4); //chip select pulled low - high to low transition selects the chip.

	// Enable SPI, Master, s?t clock rate fck/16 
//	SPCR = (1<<SPE)|(1<<MSTR?|(1<<SPR0);
	
//	SPCR=0b01010001; //no interrupt, spi enable, MSB first, master mode, SCK is low when idle, clock phase, clock f/16
//	SPCR=0b01011101; //no interrupt, spi enable, MSB first, master mode, SCK is low when idle, clock phase, clock f/16
	SPCR=0b01011100; //no interrupt, spi enable, MSB first, master mode, SCK is low when idle, clock phase, clock f/4

//	SPSR=0b00000001; //fosc/2


	//CPOL=0, CPHA=0 => SPI mode 0
	//CPOL=1, CPHA=1 => SPI mode 3
}




void WaitForIdle(void)
{
		//wait until device is not busy :
						AssertCS();
						na=SPI_Tx_Rx(0x05); //send opcode for Read Status Register
	
						while(busy==1)
						{
							status=SPI_Tx_Rx(0xFF); //clk
						

							if(bit_is_clear(status,0) )
							{
								busy=0;
							}
						}
						busy=1;
						DeassertCS();
}


void AssertCS(void)
{
	PORTB &= ~_BV(4); //chip select pulled low - high to low transition selects the chip.
}
void DeassertCS(void)
{
 	PORTB |= _BV(4);	// chip select initially high
}





unsigned char SPI_Tx_Rx(unsigned char cData)
{
		// Start transmission 
	SPDR = cData;
	// Wait for transmission?complete 
	while(!(SPSR & (1<<SPIF)))
	;

	return SPDR;
}

void mem_read(unsigned long int ad, unsigned char size)
{
			volatile unsigned char add0= (unsigned char) (ad & 0x000000FF);
						volatile unsigned char add1= (unsigned char) ( (ad>>8) & 0x000000FF);
						volatile unsigned char add2= (unsigned char) ( (ad>>16) & 0x000000FF);



					unsigned char j=0;

					AssertCS();
					na=SPI_Tx_Rx(0x0B); //send opcode for Read
					na=SPI_Tx_Rx(add2); //address to Write (address 0)
						na=SPI_Tx_Rx(add1);
						na=SPI_Tx_Rx(add0);
					na=SPI_Tx_Rx('c'); //dont care byte needed	
				

					//byte swap before sendinfg to target::::
					for(unsigned char i=0; i<size; i++)
					{
						if((i%2)==0)
						{
							j=i+1;
						}
						else
						{
							j=i-1;
						}
	

						//mem_rec[i] = SPI_Tx_Rx(0xFF);
						mem_rec[j] = SPI_Tx_Rx(0xFF);

					//	transmit_BT_ascii_num(mem_rec[i]);
					//	USART_Transmit_BT(' ');
			
					}
					
					//USART_Transmit_BT('\n');
					//	USART_Transmit_BT('\r');
					DeassertCS();
}


void ByteWriteSingle(unsigned long int ad, unsigned char byte)
{
						volatile unsigned char add0= (unsigned char) (ad & 0x000000FF);
						volatile unsigned char add1= (unsigned char) ( (ad>>8) & 0x000000FF);
						volatile unsigned char add2= (unsigned char) ( (ad>>16) & 0x000000FF);

						AssertCS();
						na=SPI_Tx_Rx(0x06); //send opcode for Write Enable
						DeassertCS();

					//	transmit_BT_ascii_num(5);

						AssertCS();
						na=SPI_Tx_Rx(0x02); //send opcode for Write
						na=SPI_Tx_Rx(add2); //address to Write (address 0)
						na=SPI_Tx_Rx(add1);
						na=SPI_Tx_Rx(add0);
						na=SPI_Tx_Rx(byte); //data	
						DeassertCS();


}

void BlockErase(void)
{
						AssertCS();
							na=SPI_Tx_Rx(0x06); //send opcode for Write Enable
							DeassertCS();


					//	transmit_BT_ascii_num(3);

							AssertCS();
						//	crap=SPI_Tx_Rx(0xD8); //send opcode for Block Erase 64K
							na=SPI_Tx_Rx(0x52); //send opcode for Block Erase 32K
							//crap=SPI_Tx_Rx(0x20); //send opcode for Block Erase 4K
							na=SPI_Tx_Rx(0x00); //address to Erase (sector 0)
							na=SPI_Tx_Rx(0x00);
							na=SPI_Tx_Rx(0x00);
							DeassertCS();

					//	transmit_BT_ascii_num(3);

					
						WaitForIdle();


}

*/
