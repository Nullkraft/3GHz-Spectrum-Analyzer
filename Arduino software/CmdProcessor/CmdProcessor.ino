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
uint32_t serialWord;                             // Serial Word as 32 bits
uint8_t* serialWordAsBytes = (byte*)&serialWord; // Serial Word as a byte array
uint16_t* serialWordAsInts = (int*)&serialWord;  // Serial Word as a byte array


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
bool NEW_INSTRUCTION_RECEIVED = false;

/* Move this into a struct
   All the values required by the spi_write() command */
uint8_t spi_select; // Copy of *_SEL pin for the chip you want to program
uint32_t spiWord;   // Holds the register contents to be written to the selected device


/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data16;    // 16 bits
uint32_t Data32;  // Needed for bit shifting and masking with LO registers
byte Command;
const byte CommandBits = 0xF8;  // Mask to select 5 bits of Command from serialWord[1]
byte Address;
const byte AddressBits = 0x07;  // Mask to select 3 bits of Address from serialWord[1]
const byte CommandFlag = 0xFF;  // Byte pattern to identify a Control Word


/*********** ARDUINO PIN DEFINITIONS ***********/
const int LO1_SEL     = A3;
const int LO2_SEL     =  3;
const int LO3_SEL     = A4;
const int REF060_SEL  =  8;
const int REF100_SEL  =  9;
const int ADC_SEL_315 = A0;   // ADC for LO2
const int ADC_SEL_045 = A1;   // ADC for LO3
const int PLL_MUX     = A2;   // Equals physical pin 16 on Port C (use PCMSK1)
const int ATTEN_SEL   = A5;
//const int SPI_MOSI  = 11;   // These 3 pins are controlled by the SPI Library
//const int SPI_MISO  = 12;
//const int SPI_CLOCK = 13;

int adc_pin;    // Set this to either ADC_SEL_### to read the ADC for LO2 or LO3


// Addresses for selecting the various hardware ICs
const int Attenuator = 0;
const int LO1_addr   = 1;
const int LO2_addr   = 2;
const int LO3_addr   = 3;
const int RefClock   = 4;
const int PLL_Mux    = 5;
const int LED        = 7;


// BitMask for programming the registers of the Attenuator IC
const uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data

const uint8_t pauseMillis = 20;



/*********** HARDWARE DEFINITIONS END *******/


// Assign reference designators from the schematic to the LO ojbect of choice
ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows a single function to select and operate on LO2 or LO3

enum STATE {WAIT, SEND, RECEIVE, PROCESS} state = WAIT; // Initial state == WAIT

byte buf_index = 0;
uint32_t frac_div_F;
uint32_t frac_mod_M;
uint32_t counter_N;
uint32_t z;
uint8_t* byteZ = (byte*)&z;   // Tmp as a byte array
uint16_t* intZ = (int*)&z;    // Tmp as an int array

char* nameLO = "aaa";


/******** SETUP *********************************************************************/
void setup() {
  // MUX pin interrupt
  PCICR |= 0b00000010;    // turn on port C pin-change interrupt(s)
  PCMSK1 |= 0b00000100;   // PCINT10

  Serial.setTimeout(200);
  Serial.begin(2000000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(REF060_SEL, OUTPUT);
  pinMode(REF100_SEL, OUTPUT);
  pinMode(LO1_SEL, OUTPUT);
  pinMode(LO2_SEL, OUTPUT);
  pinMode(LO3_SEL, OUTPUT);
  pinMode(PLL_MUX, INPUT);
  pinMode(ATTEN_SEL, OUTPUT);

  digitalWrite(REF060_SEL, HIGH);  //Enable 60MHz
  digitalWrite(REF100_SEL, LOW);   //Disable 100MHz

//  digitalWrite(LED_BUILTIN, LOW);

  num_data_points = 0;      // Used when sending LO2 and LO3 ADC outputs  to  the  PC
  num_points_processed = 0;

  // Initialize Hardware IC's on Spectrum Analyzer
  spiWriteAtten(0x7F, ATTEN_SEL);   // Maximum attenuation

 /* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent.  Why?
  *  
  *  TODO: Investigate LO1 locking anomaly
  *  
  *  Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's documentation
  */
  nameLO = "LO3";
  loadLO3(LO3_SEL, true);
  nameLO = "LO2";
  loadLO2(LO2_SEL, true);
  nameLO = "LO1";
  loadLO1(LO1_SEL);
  delay(pauseMillis);
  nameLO = "LO3";
  loadLO3(LO3_SEL, false);
  nameLO = "LO2";
  loadLO2(LO2_SEL, false);
  nameLO = "LO1";
  loadLO1(LO1_SEL);
}



uint8_t numBytes;

/******** MAIN LOOP ******************************************************************/

uint8_t* Rbyte;

void loop() {

  while (Serial.available())
  {
    // Blocks until 4 bytes (numBytesInSerialWord) have been received
    Serial.readBytes(serialWordAsBytes, numBytesInSerialWord);

    // If a General LO Instruction or LO Data Packet is received, then...
    if (serialWordAsBytes[0] != CommandFlag)
    {
      // During the RECEIVE state 8 words will be buffered for LO2 programming
      if (state == RECEIVE) {
        Serial.println("Received a frequency data packet");
        data_buf_as_word[buf_index] = serialWord; //serialWord = 32bit value from serialWordAsBytes
        buf_index++;
        Serial.print("buf_index = ");
        Serial.print(buf_index);
        Serial.print("  Size of data buffer = ");
        Serial.print(size_data_buf);
        if (buf_index > size_data_buf) {
          buf_index = 0;
          state = PROCESS;
        }
        num_points_processed++;
        // When the frequency sweep is done...
        if (num_points_processed >= num_data_points) {
          state = WAIT;
          num_points_processed = 0;   // Reset to zero for before next use
        }
      }

      // Here's where the 8 words get processed into F, M and N values then sent to
      // LO2 or LO3 and then associated A2D is read back.
      if (state == PROCESS)
      {
        for (buf_index = 0; buf_index < size_data_buf; buf_index++)
        {
          // M:  Clear R[1], bits [14:3], to accept M word
          LO->Curr.Reg[1] &= (~LO->M_mask);
          // N and F:  Clear R[0], bits [22:3], to accept N and F words
          LO->Curr.Reg[0] &= (~LO->NF_mask);

          // M:  Shift and mask data_buf_as_word[buf_index] to form M
          // where data_buf_as_word[buf_index] contains the serialWord
          LO->Curr.Reg[1] |= ((data_buf_as_word[buf_index] >> 5) & LO->M_mask);

          // N:  Shift and mask data_buf_as_word[buf_index] to form N
          // where data_buf_as_word[buf_index] contains the serialWord
          LO->Curr.Reg[0] |= ((data_buf_as_word[buf_index] << 15) & LO->N_mask);

          // F:  Shift and mask data_buf_as_word[buf_index] to form F
          // where data_buf_as_word[buf_index] contains the serialWord
          LO->Curr.Reg[0] |= ((data_buf_as_word[buf_index] >> 17) & LO->F_mask);

          // Program the selected LO starting with the higher numbered registers first
          //          spi_write(LO->Curr.Reg[1], numBytesInSerialWord);
          //          spi_write(LO->Curr.Reg[0], numBytesInSerialWord);

          // Now we read the ADC and store it for later Serial.writing()
          data_buf_as_int[buf_index] = analogRead(adc_pin);  // Buffer now used for analog output

          Serial.print("Reg[1]:M = ");
          Serial.print(LO->Curr.Reg[1] &= LO->M_mask, HEX);
          Serial.print(" : Reg[0]:NF = ");
          Serial.println(LO->Curr.Reg[0] &= LO->NF_mask, HEX);

          buf_index++;
        }
        state = SEND;
      }

      // Send the ADC readings back to the PC
      if (state == SEND) {
        for (buf_index = 0; buf_index < size_data_buf; buf_index++) {
          Serial.write(data_buf_as_int[buf_index]);
        }
        Serial.write(0xFF);
        Serial.write(0xFF);   // End-of-Block (EOB) transmission
        state = RECEIVE;
        buf_index = 0;    // Reset buffer index for receiving next block of data packets
      }
    }

    /******** COMMAND FLAG **************************************************************/
    // If a Command Flag is found then parse the 32 bits into Data, Command and Address
    else
    {
      Data16 = (uint16_t)(serialWord >> 16);
      Command = (serialWordAsBytes[1] & CommandBits) >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      NEW_INSTRUCTION_RECEIVED = true;
    }
  }  // End While



  /******** SPECTRUM ANALYZER INSTRUCTIONS **********************************************/
  /* Hardware selection and operations. This is where the processing of
     Specific commands occurs. */
  if (NEW_INSTRUCTION_RECEIVED) {
    /* Start by selecting the device that you want to control. Then under
       each device you can select the operation that you want to perform. */
    switch (Address) {
      case Attenuator:
        Data16 &= ATTEN_Data_Mask;
        spiWriteAtten((uint8_t)Data16, ATTEN_SEL);
        break;

      case LO1_addr:
        nameLO = "LO1";
        Data32 = ((uint32_t)Data16 << 4);  /* Aligns INT_N bits <N16:N1> with R[0]<DB19:DB4> */
        LO1.Curr.Reg[0] &= LO1.INT_N_Mask; /* Clear old INT_N bits from Regist 0 */
        LO1.Curr.Reg[0] |= Data32;         /* Insert new INT_N bits into Register 0*/
        Serial.println(LO1.Curr.Reg[0]);
        switch (Command) {
          case RF_off:
            LO1.Curr.Reg[6] = LO1.Curr.Reg[6] & LO1.RFpower_off;
            spiWord = LO1.Curr.Reg[6];
            break;
          // Set the RFoutB power level, RFoutA is not used and is disabled.
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
        getLOstatus(LO1);
        break;

      case LO2_addr:
        /* Making LO2 active */
        nameLO = "LO2";
        LO = &LO2;
        spi_select = LO2_SEL;
        adc_pin = ADC_SEL_315;  // Selects the ADC associated with LO2 output
      // Now fall through
      case LO3_addr:
        /* Making LO3 active */
        if (Address == LO3_addr) {
          nameLO = "LO3";
          LO = &LO3;
          spi_select = LO3_SEL;
          adc_pin = ADC_SEL_045;  // Selects the ADC associated with LO3 output
        }
        // Set the remainder of the SPI pins
        num_data_points = Data16;    // Number of frequency points to be read
        if (num_data_points > 0) {
          state = RECEIVE;           // Time to start sweeping frequencies
        }
        Serial.print("Number of requested data points = ");
        Serial.println(num_data_points);
        switch (Command) {
          case RF_off:
            LO->Curr.Reg[4] = LO->Curr.Reg[4] & LO->RFpower_off;
            spiWord = LO->Curr.Reg[4];
            break;
          case neg_4dBm:
            // ' | LO->neg4dBm' does nothing and is only for documentation
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
        //        getLOstatus(*LO);
        break;

      case RefClock:
        switch (Command) {
          // Turn 60 MHz ref_clock on and 100 MHz ref_clock off
          case ref_60:
            Serial.println("Enabled 60 MHz ref_clock");
            digitalWrite(REF060_SEL, HIGH);
            digitalWrite(REF100_SEL, LOW);
            break;
          // Turn 100 MHz ref_clock on and 60 MHz ref_clock off
          case ref_100:
            Serial.println("Enabled 100 MHz ref_clock");
            digitalWrite(REF060_SEL, LOW);
            digitalWrite(REF100_SEL, HIGH);
            break;
          // Turn both ref_clocks off
          default:
            Serial.println("Disabled both ref_clocks");
            digitalWrite(REF060_SEL, LOW);
            digitalWrite(REF100_SEL, LOW);
            break;
        }
        break;

      case LED:
        switch (Command) {
          case LED_off:
            digitalWrite(LED_BUILTIN, LOW);
            break;
          case LED_on:
            digitalWrite(LED_BUILTIN, HIGH);
            break;
          case MSG_REQ:
            Serial.print("Welcome to WN2A Spectrum Analyzer CmdProcessor 10/10/2021 2Mbaud\n");
            break;
          case RTS:
            Serial.print("PC Application is requesting to send more data.");
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

    // Restore LED functionality because it shares it's pin with SPI_CLOCK
    //    SPI.end();
    //    pinMode(LED_BUILTIN, OUTPUT);

    NEW_INSTRUCTION_RECEIVED = false;

  } /* End NEW_INSTRUCTION_RECEIVED */
} /* End loop() */





// MUX Interrupt for tracking the LO1, LO2, and LO3 lock pins
ISR(PCINT1_vect) {
  Serial.print(nameLO);
  Serial.println(" det");
}


void loadLO1(uint8_t selectPin) {
  for (int x = 13; x >= 0; x--) {
    z = LO1.Curr.Reg[x];
    spiWriteLO(z, selectPin);                 // Program LO1=37Byte33[x]76.52 MHz with LD on Mux
  }
  delay(2);                                   // Short delay before reading Register 6
  MuxTest("LO1");                             // Now read and report the Mux Pin status
  spiWriteLO(LO1.Curr.Reg[14], selectPin);    // Tri-stating the mux output disables LO1 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
 * pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
 * register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
 */
void loadLO2(uint8_t selectPin, bool initialize) {
  spiWriteLO(LO2.Curr.Reg[5], selectPin);    // First we program LO2 Register 5
  if (initialize) { delay(20); }              // And only if it's our first time must we wait 20 mSec
  for (int x = 4; x >= 0; x--) {
    z = LO2.Curr.Reg[x];                     // Program remaining registers where LO2=3915 MHz
    spiWriteLO(z, selectPin);                 // and Lock Detect is enabled on the Mux pin
  }
  delay(1);                                   // Short delay before reading Register 6
  MuxTest("LO2");                             // Check if LO2 is locked by reading the Mux pin
  spiWriteLO(LO2.Curr.Reg[6], selectPin);    // Tri-stating the mux output disables LO2 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
 * pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
 * register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
 */
void loadLO3(uint8_t selectPin, bool initialize) {
  spiWriteLO(LO3.Curr.Reg[5], selectPin);    // First we program LO3 Register 5
  if (initialize) { delay(20); }              // And only if it's our first time must we wait 20 mSec
  for (int x = 4; x >= 0; x--) {
    z = LO3.Curr.Reg[x];                     // Program remaining registers where LO3=270 MHz
    spiWriteLO(LO3.Curr.Reg[x], selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  delay(1);                                   // Short delay before reading Register 6
  MuxTest("LO3");                             // Check if LO3 is locked by reading the Mux pin
  spiWriteLO(LO3.Curr.Reg[6], selectPin);    // Tri-stating the mux output disables LO3 lock detect
}


// Program the Digital Attenuator by sending and latching a single byte
void spiWriteAtten(uint8_t level, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(selectPin, LOW);
  SPI.transfer(level);
  digitalWrite(selectPin, HIGH);
  SPI.end();
}


// Program a single register of the selected LO by sending and latching 4 bytes
void spiWriteLO(uint32_t reg, uint8_t selectPin) {
//  Serial.print("Write ");
//  Serial.println(nameLO);
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
const void getLOstatus(MAX2871_LO LO)
{
  for (int i = 0; i < LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.Reg[i], HEX);
  }
}


// This function is for development testing. Remove at production
const void getLOstatus(ADF4356_LO LO) {
  for (int i = 0; i < LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.Reg[i], HEX);
  }
}
