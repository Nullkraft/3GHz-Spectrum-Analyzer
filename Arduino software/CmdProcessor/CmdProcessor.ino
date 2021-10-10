/* Process incoming serial commands and device data registers
    for the LO's.
*/

/************* PREMPTIVE STRIKE for Ver3 board *************/
#include <SPI.h>

/* TODO: Lookup SPI speeds for the various chips */
SPISettings atten_SPI_Config(SPISettings(14000000, MSBFIRST, SPI_MODE0));
SPISettings LO_SPI_Config(SPISettings(20000000, MSBFIRST, SPI_MODE0));

/************* END PREMPTIVE STRIKE for Ver3 board *************/

#include "max2871.h"
#include "adf4356.h"
#include "hw_cmd_lists.h"       // Defined Commands IAW Interface Standard III

size_t numBytesRead;

#define szSerialWord 4    // Size in number of bytes
uint32_t serialWord;                            // Serial word as a single 32 bits
byte* serialWordArray = (byte *) &serialWord;   // Serial word as an array of bytes


/* For any given LO2/3 command there will be a 4 byte Instruction Word
   (more specifically bits <20:16>) that informs the Arduino how many
   32 bit Words are to follow.  */
byte numDataWords; // See case LO2/3 below for masking of Data & LOx_Bits

/* Max block size is 128 bytes and each Word contains 4 bytes.
   So the max number of Data Words can be 32.  */
const byte maxNumDataWords = 32;
uint32_t dataWordArray[maxNumDataWords];

// Indicates that a new Command has been sent to the Arduino
// as denoted by a Command Flag, 0xFF, having been received.
bool newCommandReceived = false;

/* Move this into a struct
   All the values required by the spi_write() command */
int miso_pin;       // Arduino pin for reading Reg6 or A2D's
int mosi_pin;       // Set this equal to the pin number of the device to be written to
int spi_clock;
int spi_select;     // Selects the device to be written to
uint32_t spiWord;   // Holds the register contents to be written to the selected device


/*** 32 bit SERIAL WORD SPLITS ***/
uint16_t Data;            // 16 bits
uint32_t Data32;          // Needed for bit shifting and masking with LO registers
byte Command;
byte CommandBits = 0xF8;  // Mask to select 5 bits of Command from serialWord[1]
byte Address;
byte AddressBits = 0x07;  // Mask to select 3 bits of Address from serialWord[1]
byte CommandFlag = 0xFF;  // Byte pattern to identify a Control Word


/*********** HARDWARE DEFINITIONS ***********/
// Pin assignments, for Ver2 board, used for shiftOut() style communication
const int REF1_SEL      = 11;
const int REF2_SEL      = 12;
const int ATTEN_SEL     = A3;
const int ATTEN_MOSI    = A5;
const int ATTEN_CLOCK   = A4;
const int LO1_SEL       = A1;
const int LO1_MOSI      = A0;
const int LO2_SEL       =  5;
const int LO2_MOSI      =  4;
const int LO3_SEL       =  2;
const int LO3_MOSI      =  3;
const int PLL_MUX_MISO  = A2;
const int SPI_CLOCK_PIN = 10;  // Common SPI clock for LO's and Attenuator


// Addresses for selecting the various hardware ICs
const int Attenuator    = 0;
const int LO1_addr      = 1;
const int LO2_addr      = 2;
const int LO3_addr      = 3;
const int RefClock      = 4;
const int PLL_Mux       = 5;
const int Reserved      = 6;
const int LED           = 7;


// BitMask for programming the registers of the Attenuator IC
const short ATTEN_Data_Mask     = 0x7F;      // 7 bits of Embedded Data



/*********** HARDWARE DEFINITIONS END *******/


ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows me to write one set of code for LO2 and LO3


// Set by Embedded Data bits <BT5:BT0> when an LO2 or 3 command has been received
// Denotes how many General Purpose register updates will be following.
uint16_t block_count = 0;

// Decide if one of the LO's received a command to update a register
bool writeToSPI;



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(miso_pin, OUTPUT);
  pinMode(mosi_pin, INPUT);
  pinMode(spi_clock, OUTPUT);
  pinMode(spi_select, OUTPUT);

  Serial.setTimeout(200);
  Serial.begin(2000000);
  Serial.println("\nWelcome to Vapor Ware\n");

  writeToSPI = true;  // Assume we'll get a command that requires programming a chip...
}



void loop()
{
  while (Serial.available())
  {
    numBytesRead = Serial.readBytes(serialWordArray, szSerialWord);
    /* Check for a Command Word and parse into Data, Command and Address */
    if (serialWordArray[0] == CommandFlag)
    {
      Data = (uint16_t)(serialWord >> 16);
      Command = (serialWordArray[1] & CommandBits) >> 3;
      Address = serialWordArray[1] & AddressBits;
      newCommandReceived = true;
    }
    else
    {
      /* TODO: Buffer Directdata Words */
      block_count = numDataWords;
      if (block_count > 0) {
        block_count--;          // decrement block_count
        dataWordArray[block_count] = serialWord;
        numDataWords--;   // decrement
      }
      if (numDataWords == 0) {
      }
      //      spi_write();    // Write Directdata Word to the selected device
    }
  } // End While


  // Hardware selection and operations.
  if (newCommandReceived) {
    // Start by selecting the device that you want to control. Then under
    // each device you can select the operation that you want to perform.
    switch (Address) {
      case Attenuator:
        mosi_pin = ATTEN_MOSI;
        spi_clock = ATTEN_CLOCK;
        spi_select = ATTEN_SEL;
        Data &= ATTEN_Data_Mask;
        spiWord = ((uint32_t)Data << 16) | 0xFF;
        spi_write();
        Serial.print("Digital Attenuator selected and commanded to set ");
        Serial.println(spiWord, HEX);
        break;

      case LO1_addr:
        mosi_pin = LO1_MOSI;
        spi_clock = SPI_CLOCK_PIN;
        spi_select = LO1_SEL;
        Data32 = ((uint32_t)Data << 4); /* Needed to align <N16:N1> with R[0]<DB19:DB4> */
        switch (Command) {
          case RF_off:
            LO1.Curr.R[6] =  LO1.Curr.R[6] & LO1.RFpower_off;
            spiWord = LO1.Curr.R[6];
            break;
          // Set the RFoutB power level, RFoutA is not used and can be disabled.
          case neg_4dBm:
            LO1.Curr.R[6] = (LO1.Curr.R[6] & LO1.Power_Level_Mask) | LO1.neg4dBm; // neg4dBm is only for documentation
            spiWord = LO1.Curr.R[6];
            break;
          case neg_1dBm:
            LO1.Curr.R[6] = (LO1.Curr.R[6] & LO1.Power_Level_Mask) | LO1.neg1dBm;
            spiWord = LO1.Curr.R[6];
            break;
          case pos_2dBm:
            LO1.Curr.R[6] = (LO1.Curr.R[6] & LO1.Power_Level_Mask) | LO1.pos2dBm;
            spiWord = LO1.Curr.R[6];
            break;
          case pos_5dBm:
            LO1.Curr.R[6] = (LO1.Curr.R[6] & LO1.Power_Level_Mask) | LO1.pos5dBm;
            spiWord = LO1.Curr.R[6];
            break;
          case Mux_TRI:
            LO1.Curr.R[4] = (LO1.Curr.R[4] & LO1.Mux_Set_TRI); // Turns on Tristate
            spiWord = LO1.Curr.R[4];
            break;
          case Mux_DLD:
            LO1.Curr.R[4] = LO1.Curr.R[4] | LO1.Mux_Set_DLD; // Set MuxOut to Dig. Lock Det.
            spiWord = LO1.Curr.R[4];
            break;
          default:
            writeToSPI = false;    // Do not write to SPI. No commands were received
            break;
        }
        if (writeToSPI) {
          spi_write();
        }
        writeToSPI = true;  // Reset for next incoming serial command
        getLOstatus(LO1);
        break;

      case LO2_addr:
        /* Making LO2 active */
        LO = &LO2;
        mosi_pin = LO2_MOSI;
        spi_select = LO2_SEL;
        // NOTE: break is missing so the code falls through to 'case LO3_addr'
      case LO3_addr:
        /* Making LO3 active */
        if (Address = LO3_addr) {
          LO = &LO3;
          mosi_pin = LO3_MOSI;
          spi_select = LO3_SEL;
        }
        // Set the remainder of the SPI pins
        spi_clock = SPI_CLOCK_PIN;
        Data &= LO->Data_Mask;      // Parse the incoming serial Embedded Data
        numDataWords = (byte)Data;
        switch (Command) {
          case RF_off:
            LO->Curr.R[4] =  LO->Curr.R[4] & LO->RFpower_off;
            spiWord = LO->Curr.R[4];
            break;
          case neg_4dBm:
            LO->Curr.R[4] = (LO->Curr.R[4] & LO->Power_Level_Mask) | LO->neg4dBm; // neg4dBm is only for documentation
            spiWord = LO->Curr.R[4];
            break;
          case neg_1dBm:
            LO->Curr.R[4] = (LO->Curr.R[4] & LO->Power_Level_Mask) | LO->neg1dBm;
            spiWord = LO->Curr.R[4];
            break;
          case pos_2dBm:
            LO->Curr.R[4] = (LO->Curr.R[4] & LO->Power_Level_Mask) | LO->pos2dBm;
            spiWord = LO->Curr.R[4];
            break;
          case pos_5dBm:
            LO->Curr.R[4] = (LO->Curr.R[4] & LO->Power_Level_Mask) | LO->pos5dBm;
            spiWord = LO->Curr.R[4];
            break;
          case Mux_TRI:
            LO->Curr.R[2] = LO->Curr.R[2] & LO->Mux_Set_TRI; // Set MuxOut to Tristate
            spiWord = LO->Curr.R[2];
            break;
          case Mux_DLD:
            LO->Curr.R[2] = LO->Curr.R[2] | LO->Mux_Set_DLD; // Set MuxOut to Dig. Lock Det.
            spiWord = LO->Curr.R[2];
            break;
          default:
            writeToSPI = false;    // Do not write to SPI. No commands were received
            break;
        }
        if (writeToSPI) {
          spi_write();
        }
        writeToSPI = true;  // Reset for next incoming serial command
        getLOstatus(*LO);
        break;

      case RefClock:
        switch (Command) {
          case sel_ref1:
            digitalWrite(REF2_SEL, LOW);
            digitalWrite(REF1_SEL, HIGH);
            break;
          case sel_ref2:
            digitalWrite(REF1_SEL, LOW);
            digitalWrite(REF2_SEL, HIGH);
            break;
          default:
            digitalWrite(REF1_SEL, LOW);
            digitalWrite(REF2_SEL, LOW);
            break;
        }
        break;

      // To use the PLL Mux you must first set the selection pin for one of the LO's.
      // Once that's done you send the Command for READ_REG_6, do an spi_read() to
      // get the register's contents, then send the Command TRI_STATE before moving
      // on to the next LO.
      case PLL_Mux:
        switch (Command) {
          default:
            Serial.println("Shouldn't this be part of the LO's?");
            break;
        }



      // Not part of the Interface Standard - For in-house testing only.
      case LED:
        switch (Command) {
          case LED_off:
            digitalWrite(LED_BUILTIN, LOW);
            break;
          case LED_on:
            digitalWrite(LED_BUILTIN, HIGH);
            break;
          case MSG_REQ:
            Serial.println("Arduino CmdProcessor.ino 10 October 2021  2 Megabaud");
            break;
          default:
            break;
        }
        break;

      default:
        Serial.println("Invalid device address");
        break;
    }   /* End switch(Address) */

    /* Limit set by the size of the dataWordArray[maxNumDataWords] */
    if (numDataWords > maxNumDataWords) {
      numDataWords = maxNumDataWords;
    }
    newCommandReceived = false;
    
  }  /* End newCommandReceived */
}  /* End loop() */


// Program the selected device using shiftOut()
/* This will be replaced by SPI.transfer() for the Ver 3 board */
void spi_write() {
  for (int i = 0; i < szSerialWord; i++) {
    shiftOut(mosi_pin, spi_clock, MSBFIRST, spiWord);
  }
  digitalWrite(spi_select, HIGH);
  digitalWrite(spi_select, LOW);
}


// This function is for development testing. Remove at production
void getLOstatus(MAX2871_LO LO) {
  for (int i=0; i<LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.R[i], HEX);
  }
}


// This function is for development testing. Remove at production
void getLOstatus(ADF4356_LO LO) {
  for (int i=0; i<LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.R[i], HEX);
  }
}
