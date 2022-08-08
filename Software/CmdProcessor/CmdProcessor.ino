/* Process incoming serial commands and device data registers
    for the LO's.

    All incoming serial data must be formed into 4 byte instruc-
    tions or data packets.  There  are  2  distinct  instruction
    types and one Data packet type. They are defined as follows:

    1. Specific Instruction - It consists of 16 bits of Embedded
       Data, 5 bits of Instruction, 3 bits of Address and 6 bits
       of a fixed value, 0xFF, Command Flag.

    2. Direct LO Instruction -  These consist of the values used
       to directly program the chip registers as defined  within
       the manufacturer's specification sheet.

    3. LO Data Packet -     These consist of a 12 bit Fractional
       Counter (F), a 12 bit Modulus Counter (M)  and  an  8 bit
       Integer Counter (N). A number of these will be sent based
       on the number sent in the Embedded Data section of an LO2
       or LO3 Specific Command.
*/

#include <SPI.h>
#include <avr/interrupt.h>

#include "max2871.h"
#include "adf4356.h"
#include "hw_cmd_lists.h"  // Defined Commands IAW Interface Standard III


/*           Serial Word with Command Flag:
    ________________________________________________
   [       16 Bit        | 5 bit  | 3 b |   8 bit   ]
   [       Embedded      | Instr- |Addr.|  Command  ]  NOTE: Command Flag
   [       Data          | uction |     |   Flag    ]        always = 0xFF
   [---------------------+--------+-----+-----------]
   [ xxxx xxxx xxxx xxxx | xxxx x | xxx | 1111 1111 ]
*/
const uint8_t numBytesInSerialWord = 4;
uint32_t serialWord;                                  // Serial Word as 32 bits
uint8_t* serialWordAsBytes = (uint8_t*)&serialWord;   // Serial Word as a byte array
uint16_t* serialWordAsInts = (uint16_t*)&serialWord;  // Serial Word as a int array


/* Several of the LO2/3 commands have an extra 6 bits of Embedded Data <21:16>
   that tells the Arduino how many 32 bit Words are to follow. */
uint16_t num_data_points;  // If this is >0 it means we are sweeping frequencies
uint16_t num_points_processed;


/* Max block size is 128 bytes and because each Data Word
   contains 4 bytes the maximum number of Data Words is 32. */
const uint8_t size_data_buf = 8;
uint32_t data_buf_as_word[size_data_buf];
uint16_t* data_buf_as_int = (uint16_t*)&data_buf_as_word;

// Command Flag 0xFF indicates that a new instruction was received by the Arduino
bool COMMAND_FLAG = false;
// Define incoming serial data as direct register programming for LO2 and LO3
bool BLOCK_TRANSFER = false;

/* Move this into a struct
   All the values required by the spi_write() command */
uint8_t spi_select; // Currently selected LO (1, 2 or 3) that is to be programmed
uint32_t spiWord;   // Holds the register contents to be written to the selected device


/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data16;    // 16 bits
uint32_t Data32;  // Needed for bit shifting and masking with LO registers
byte Command;
const byte CommandBits = 0xF8;  // Mask to select 5 bits of 'Command' from serialWord[1]
byte Address;
const byte AddressBits = 0x07;  // Mask to select 3 bits of 'Address' from serialWord[1]
const byte CommandFlag = 0xFF;  // Byte pattern to identify a 'Control Word'


/*********** ARDUINO PIN DEFINITIONS ***********/
const int LO1_SEL     = A3;
const int LO2_SEL     =  3;
const int LO3_SEL     = A4;
const int REF_LO_SEL  =  8;
const int REF_HI_SEL  =  9;
const int ADC_SEL_045 = A0;   // ADC for LO2
const int ADC_SEL_315 = A1;   // ADC for LO3
const int PLL_MUX     = A2;   // Equals physical pin 16 on Port C (use PCMSK1)
const int ATTEN_SEL   = A5;
//const int SPI_MOSI  = 11;   // Reserved by the SPI Library
//const int SPI_MISO  = 12;   // Reserved by the SPI Library
//const int SPI_CLOCK = 13;   // Reserved by the SPI Library

int adc_pin;    // Set this to either ADC_SEL_### to read the ADC for LO2 or LO3


// Addresses for selecting the various hardware ICs
const int Attenuator = 0;
const int LO1_addr   = 1;
const int LO2_addr   = 2;
const int LO3_addr   = 3;
const int RefClock   = 4;
const int MISC_addr  = 7;


// BitMask for programming the registers of the Attenuator IC
const uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data

const uint8_t pauseMillis = 20;



/*********** HARDWARE DEFINITIONS END *******/


// Assign reference designators from the schematic to the LO ojbect of choice
ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows a single function to select and operate on LO2 or LO3


byte buf_index = 0;
uint32_t frac_div_F;
uint32_t frac_mod_M;
uint32_t counter_N;
uint32_t z;
uint8_t* byteZ = (byte*)&z;   // Tmp as a byte array
uint16_t* intZ = (uint16_t*)&z;    // Tmp as an int array

char const *nameLO;

#define USE_BINARY  // Comment out for ASCII serial commuinication

uint32_t t_start;

/******** SETUP *********************************************************************/
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(REF_LO_SEL, OUTPUT);
  pinMode(REF_HI_SEL, OUTPUT);
  pinMode(LO1_SEL, OUTPUT);
  pinMode(LO2_SEL, OUTPUT);
  pinMode(LO3_SEL, OUTPUT);
  pinMode(PLL_MUX, INPUT);
  pinMode(ATTEN_SEL, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);   // Make sure the LED is off

  Serial.setTimeout(200);
  Serial.begin(2000000);
//  Serial.begin(9600);

//  MUX pin interrupt
  PCICR |= 0b00000010;    // turn on port C pin-change interrupt(s)
  PCMSK1 |= 0b00000100;   // PCINT10

  num_data_points = 0;      // Used when sending LO2 and LO3 ADC outputs  to  the  PC
  num_points_processed = 0;

  // Presets for LO3
  LO3.Curr.Reg[0] = 0x002081C8; // LO3 = 270 MHz with 66 MHz ref clock
  LO3.Curr.Reg[1] = 0x400103E9;
  LO3.Curr.Reg[2] = 0x98005F42;
  LO3.Curr.Reg[3] = 0x00001F23;
  LO3.Curr.Reg[4] = 0x63CE803C;
  LO3.Curr.Reg[5] = 0x00400005;
  LO3.Curr.Reg[6] = 0x40008042;

  // Hardware Initialization for testing of early hardware builds.
  spiWriteAtten(0x0, ATTEN_SEL);    // Set 0dB on the digital attenuator
  digitalWrite(REF_LO_SEL, HIGH);   // Enable low frequency referenc clock
  digitalWrite(REF_HI_SEL, LOW);    // Disable high frequency referenc clock
  /* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent.  Why?
     TODO: Investigate LO1 locking anomaly
     Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's documentation
  */
  nameLO = "LO3";
  initialize_LO3(LO3_SEL, true);
  nameLO = "LO2";
  initialize_LO2(LO2_SEL, true);
  nameLO = "LO1";
  initialize_LO1(LO1_SEL);
  delay(pauseMillis);
  nameLO = "LO3";
  initialize_LO3(LO3_SEL, false);
  nameLO = "LO2";
  initialize_LO2(LO2_SEL, false);
  nameLO = "LO1";
  initialize_LO1(LO1_SEL);
}



/******** MAIN LOOP ******************************************************************/

void loop() {
TopLoop:
  while (Serial.available())
  {
#ifdef USE_BINARY
    // Binary Communication for normal usage:
    // Blocks until numBytesInSerialWord==4 has been received
    Serial.readBytes(serialWordAsBytes, numBytesInSerialWord);
#else
    // ASCII Communication for testing Mike's code:
    // Blocks until numBytesInSerialWord==4 has been received
    serialWord = Serial.parseInt();
    if (serialWord == 0) {
      goto TopLoop;
    }
#endif

    // LO2/3 Instruction arrived...
//    if ((serialWordAsBytes[0] != CommandFlag) && BLOCK_TRANSFER)
    if (BLOCK_TRANSFER)
    {
      Serial.print("BLOCK (FMN) programming!\n");
      Serial.print("\tSerial bytes = ");
      Serial.println(serialWord, HEX);
      // M:  Clear Reg[1], bits [14:3], to accept M word
      LO->Curr.Reg[1] &= (~LO->M_mask);
      // M:  Shift and mask serialWord to form M
      LO->Curr.Reg[1] |= ((serialWord >> 5) & LO->M_mask);

      // N and F:  Clear Reg[0], bits [22:3], to accept N and F words
      LO->Curr.Reg[0] &= (~LO->NF_mask);
      // N:  Shift and mask serialWord to form N
      LO->Curr.Reg[0] |= ((serialWord << 15) & LO->N_mask);

      // F:  Shift and mask serialWord to form F
      LO->Curr.Reg[0] |= ((serialWord >> 17) & LO->F_mask);

      // Program the selected LO starting with the higher numbered registers first
      spiWriteLO(LO->Curr.Reg[1], spi_select);
      spiWriteLO(LO->Curr.Reg[0], spi_select);

      // Now we read the ADC
      serialWordAsInts[0] = analogRead(adc_pin);  // Buffer now used for analog output

      // And send the collected data to the PC for plotting
      Serial.write(serialWordAsBytes[0]);
      Serial.write(serialWordAsBytes[1]);
//      break;
    }

    /******** COMMAND FLAG **************************************************************/
    // If a Command Flag is found then parse the 32 bits into Data, Command and Address
//    else if((serialWordAsBytes[0] == CommandFlag) && !BLOCK_TRANSFER)
    else if(serialWordAsBytes[0] == CommandFlag)
    {
      Serial.println("Command FLAG programming!\n");
      Serial.print("\tSerial bytes: ");
      Serial.print(serialWord, HEX);
      Data16 = serialWordAsInts[1];
      Command = (serialWordAsBytes[1] & CommandBits) >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      Serial.print("\tAddress: ");
      Serial.print(Address, HEX);
      Serial.print("\tCommand: ");
      Serial.println(Command, HEX);
      Serial.println();
      COMMAND_FLAG = true;
    }

    /******* Direct programming of LO2 and LO3 **********/
    // Set spi_select to the LO to be programmed before starting a block transfer
    // You can do so by calling ANY of the commands for the given LO
    else
    {
      Serial.println("DIRECT programming!\n");
      Serial.print("\tSerial bytes = ");
      Serial.println(serialWord, HEX);
      Serial.println();
      spiWriteLO(serialWordAsBytes, spi_select);
    }
    
  }  // End While



  /******** SPECTRUM ANALYZER INSTRUCTIONS **********************************************/
  /* Hardware selection and operations. This is where the processing of
     Specific commands occurs. */
  if (COMMAND_FLAG) {
    /* Start by selecting the device that you want to control. Then under
       each device you can select the operation that you want to perform. */
    switch (Address) {
      case Attenuator:
        Data16 &= ATTEN_Data_Mask;
        spiWriteAtten((uint8_t)Data16, ATTEN_SEL);
        break;

      case LO1_addr:
        nameLO = "LO1";
        spi_select = LO1_SEL;
        Data32 = ((uint32_t)Data16 << 4);  /* Aligns INT_N bits <N16:N1> with R[0]<DB19:DB4> */
        LO1.Curr.Reg[0] &= LO1.INT_N_Mask; /* Clear old INT_N bits from Regist 0 */
        LO1.Curr.Reg[0] |= Data32;         /* Insert new INT_N bits into Register 0*/
        switch (Command) {
          case RF_off:
            LO1.Curr.Reg[6] = LO1.Curr.Reg[6] & LO1.RFpower_off;
            spiWord = LO1.Curr.Reg[6];
            break;
          // Set the RFoutB power level and disable RFoutA.
          case neg_4dBm:
            LO1.Curr.Reg[6] = (LO1.Curr.Reg[6] & LO1.Power_Level_Mask) | LO1.neg4dBm;
            spiWord = LO1.Curr.Reg[6];
            break;
          case neg_1dBm:
            LO1.Curr.Reg[6] = (LO1.Curr.Reg[6] & LO1.Power_Level_Mask) | LO1.neg1dBm;
            spiWord = LO1.Curr.Reg[6];
            break;
          case pos_2dBm:
            LO1.Curr.Reg[6] = (LO1.Curr.Reg[6] & LO1.Power_Level_Mask) | LO1.pos2dBm;
            spiWord = LO1.Curr.Reg[6];
            break;
          case pos_5dBm:
            LO1.Curr.Reg[6] = (LO1.Curr.Reg[6] & LO1.Power_Level_Mask) | LO1.pos5dBm;
            spiWord = LO1.Curr.Reg[6];
            break;
          case Mux_TRI:
            LO1.Curr.Reg[4] = (LO1.Curr.Reg[4] & LO1.Mux_Set_TRI);  // Turns on Tristate
            spiWord = LO1.Curr.Reg[4];
            break;
          case Mux_DLD:
            LO1.Curr.Reg[4] = LO1.Curr.Reg[4] | LO1.Mux_Set_DLD;    // Set MuxOut to Dig. Lock Det.
            spiWord = LO1.Curr.Reg[4];
            break;
          default:
            break;
        }
        // Now program LO1 with the new settings
        spiWriteLO(spiWord, spi_select);
        spiWriteLO(LO1.Curr.Reg[1], spi_select);
        spiWriteLO(LO1.Curr.Reg[0], spi_select);
        //getLOstatus(LO1);
        break;

      case LO2_addr:
        /* Making LO2 active */
        nameLO = "LO2";
        LO = &LO2;
        spi_select = LO2_SEL;
      // Now fall through
      case LO3_addr:
        /* Making LO3 active */
        if (Address == LO3_addr) {
          nameLO = "LO3";
          LO = &LO3;
          spi_select = LO3_SEL;
        }
        // Set the remainder of the SPI pins
        num_data_points = Data16;    // Number of frequency points to be read
        switch (Command) {
          case RF_off:
            LO->Curr.Reg[4] = LO->Curr.Reg[4] & LO->RFpower_off;
            spiWord = LO->Curr.Reg[4];
            break;
          case neg_4dBm:
            // OR'ing with LO->neg4dBm does nothing and is only added for clarity
            LO->Curr.Reg[4] = (LO->Curr.Reg[4] & LO->Power_Level_Mask) | LO->neg4dBm;
            spiWord = LO->Curr.Reg[4];
            break;
          case neg_1dBm:
            LO->Curr.Reg[4] = (LO->Curr.Reg[4] & LO->Power_Level_Mask) | LO->neg1dBm;
            spiWord = LO->Curr.Reg[4];
            break;
          case pos_2dBm:
            LO->Curr.Reg[4] = (LO->Curr.Reg[4] & LO->Power_Level_Mask) | LO->pos2dBm;
            spiWord = LO->Curr.Reg[4];
            break;
          case pos_5dBm:
            LO->Curr.Reg[4] = (LO->Curr.Reg[4] & LO->Power_Level_Mask) | LO->pos5dBm;
            spiWord = LO->Curr.Reg[4];
            break;
          case Mux_TRI:
            LO->Curr.Reg[2] = LO->Curr.Reg[2] & LO->Mux_Set_TRI;  // Set MuxOut to Tristate
            spiWord = LO->Curr.Reg[2];
            break;
          case Mux_DLD:
            LO->Curr.Reg[2] = LO->Curr.Reg[2] | LO->Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
            spiWord = LO->Curr.Reg[2];
            break;
          default:
            break;
        }
        // Now program the currently selected LO
        spiWriteLO(spiWord, spi_select);
        //getLOstatus(*LO);
        break;    // End case LO2 OR case LO3

      case RefClock:
        switch (Command) {
          // Turn both ref_clocks off
          case all_ref_off:
            digitalWrite(REF_LO_SEL, LOW);
            delay(2);
            digitalWrite(REF_HI_SEL, LOW);
            break;
          // Turn on 66.000 MHz ref_clock AND turn off 66.666 MHz ref_clock
          case ref_LO:
            digitalWrite(REF_LO_SEL, HIGH);
            delay(2);
            digitalWrite(REF_HI_SEL, LOW);
            break;
          // Turn on 66.666 MHz ref_clock AND turn off 66.000 MHz ref_clock
          case ref_HI:
            digitalWrite(REF_LO_SEL, LOW);
            delay(2);
            digitalWrite(REF_HI_SEL, HIGH);
            break;
          default:
            break;
        }
        break;

      case MISC_addr:
        switch (Command) {
          case LED_off:
            digitalWrite(LED_BUILTIN, LOW);
            break;
          case LED_on:
            digitalWrite(LED_BUILTIN, HIGH);
            break;
          case MSG_REQ:
            Serial.print("- WN2A Spectrum Analyzer CmdProcessor Oct. 2021");
            break;
          case RTS:
            Serial.print("PC Application is requesting to send more data.");
            break;
          case ADC_315:
            adc_pin = ADC_SEL_315;  // Selects the ADC associated with LO2 output
            break;
          case ADC_045:
            adc_pin = ADC_SEL_045;  // Selects the ADC associated with LO3 output
            break;
          case SWEEP_START:
            BLOCK_TRANSFER = true;
            break;
          case SWEEP_END:
            Serial.println("Sweep stop was called");
            BLOCK_TRANSFER = false;
            Serial.write(0xFF);
            Serial.write(0xFF);
            break;
          default:
            break;
        }
        break;

      default:
        Serial.print("Requested Address:");
        Serial.print(Address);
        Serial.println(" not found");
        break;

    } /* End switch(Address) */
    COMMAND_FLAG = false;
  } /* End COMMAND_FLAG */
} /* End loop() */





// MUX Interrupt for tracking the LO1, LO2, and LO3 lock pins
ISR(PCINT1_vect) {
  Serial.print(nameLO);
  Serial.print(" LD ");
  Serial.println(micros()-t_start);
}


void initialize_LO1(uint8_t selectPin) {
  for (int x = 13; x >= 0; x--) {
    z = LO1.Curr.Reg[x];
    spiWriteLO(z, selectPin);                 // Program LO1=3776.52 MHz with LD on Mux
    t_start = micros();
  }
//  delay(3);                                   // Short delay before reading Register 6
//  MuxTest("LO1");                             // Now read and report the Mux Pin status
  spiWriteLO(LO1.Curr.Reg[14], selectPin);    // Tri-stating the mux output disables LO1 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void initialize_LO2(uint8_t selectPin, bool initialize) {
  spiWriteLO(LO2.Curr.Reg[5], selectPin);    // First we program LO2 Register 5
  if (initialize) {
    delay(20);  // And only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    z = LO2.Curr.Reg[x];                     // Program remaining registers where LO2=3915 MHz
    spiWriteLO(z, selectPin);                // and Lock Detect is enabled on the Mux pin
    t_start = micros();
  }
//  delay(1);                                  // Short delay before reading Register 6
//  MuxTest("LO2");                            // Check if LO2 is locked by reading the Mux pin
  spiWriteLO(LO2.Curr.Reg[6], selectPin);    // Tri-stating the mux output disables LO2 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void initialize_LO3(uint8_t selectPin, bool initialize) {
  spiWriteLO(LO3.Curr.Reg[5], selectPin);    // First we program LO3 Register 5
  if (initialize) {
    delay(20);  // And only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    z = LO3.Curr.Reg[x];                     // Program remaining registers where LO3=270 MHz
    spiWriteLO(LO3.Curr.Reg[x], selectPin);  // and Lock Detect is enabled on the Mux pin
    t_start = micros();
  }
//  delay(1);                                  // Short delay before reading Register 6
//  MuxTest("LO3");                            // Check if LO3 is locked by reading the Mux pin
  spiWriteLO(LO3.Curr.Reg[6], selectPin);    // Tri-stating the mux output disables LO3 lock detect
}


// Program the Digital Attenuator by sending and latching a single byte
void spiWriteAtten(uint8_t level, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  SPI.transfer(level);
  digitalWrite(selectPin, HIGH);
  digitalWrite(selectPin, LOW);
  SPI.end();
}


// Program a single register of the selected LO by sending and latching 4 bytes
void spiWriteLO(uint32_t reg, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(selectPin, LOW);
  SPI.transfer(reg >> 24);
  SPI.transfer(reg >> 16);
  SPI.transfer(reg >> 8);
  SPI.transfer(reg);
  digitalWrite(selectPin, HIGH);
  SPI.end();
}


// When the mux pin is configured for Digital Lock Detect output
// we can read the status of the pin from here.
void MuxTest(String chipName) {
  if (digitalRead(PLL_MUX)) {
    Serial.print(chipName);
    Serial.println(" LOCK   ");
  }
  else {
    Serial.print(chipName);
    Serial.println(" UNLOCK   ");
  }
}












// This function is for development testing. Remove at production
void getLOstatus(MAX2871_LO LO)
{
  for (int i = 0; i < LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.Reg[i], HEX);
  }
}


// This function is for development testing. Remove at production
void getLOstatus(ADF4356_LO LO) {
  for (int i = 0; i < LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.Reg[i], HEX);
  }
}
