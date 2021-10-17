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
#include "hw_cmd_lists.h"  // Defined Commands IAW Interface Standard III


/*           Serial Word with Command Flag:
    ________________________________________________
   [       16 Bit        | 5 bit  | 3 b |   8 bit   ]
   [       Embedded      | Instr- |Addr.|  Command  ]  NOTE: Command Flag
   [       Data          | uction |     |   Flag    ]        always = 0xFF
   [---------------------+--------+-----+-----------]
   [ xxxx xxxx xxxx xxxx | xxxx x | xxx | 1111 1111 ]
*/
#define numBytesInSerialWord 4
uint32_t serialWord;                         // Serial Word as 32 bits
byte* serialWordAsBytes = (byte*)&serialWord;  // Serial Word as a byte array


/* Several of the LO2/3 commands have an extra 6 bits of Embedded Data <21:16>
   that tells the Arduino how many 32 bit Words are to follow. */
int16_t wordCount;  // See 'case LO2:' below for masking of Data & LOx_Bits
int16_t NthWord;

/* Max block size is 128 bytes and because each Data Word
   contains 4 bytes the maximum number of Data Words is 32. */
const byte szArrayWordBuff = 4;
uint32_t dataWordBuf[szArrayWordBuff];
uint16_t* dataWordBufInts = (uint16_t*)&dataWordBuf;

// Command Flag 0xFF indicates that a new instruction was received by the Arduino
bool newCommandReceived = false;

/* Move this into a struct
   All the values required by the spi_write() command */
int miso_pin;  // Read values from an external A2D chip and Mux pin of LO's
int mosi_pin;  // Set to write register values to the LO's or the Attenuator
int spi_clock;
int spi_select;    // Selects the device to be written to
uint32_t spiWord;  // Holds the register contents to be written to the selected device


/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data;    // 16 bits
uint32_t Data32;  // Needed for bit shifting and masking with LO registers
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
const int Attenuator = 0;
const int LO1_addr   = 1;
const int LO2_addr   = 2;
const int LO3_addr   = 3;
const int RefClock   = 4;
const int PLL_Mux    = 5;
//                   = 6;
const int LED        = 7;


// BitMask for programming the registers of the Attenuator IC
const short ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data



/*********** HARDWARE DEFINITIONS END *******/


ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows me to write one set of code for LO2 and LO3


// Decide if one of the LO's received a command to update a register
bool writeToSPI;

bool displayVersionInfo = true;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(miso_pin, OUTPUT);
  pinMode(mosi_pin, INPUT);
  pinMode(spi_clock, OUTPUT);
  pinMode(spi_select, OUTPUT);

  Serial.setTimeout(200);
  Serial.begin(2000000);

  writeToSPI = true;  // This goes false for commands that don't program a chip

}


uint16_t idx;
uint16_t analog;

void loop() {
  if (displayVersionInfo) {
    Serial.println("\n Welcome to WN2A Spectrum Analyzer CmdProcessor \n 10/10/2021 2Mbaud");
    displayVersionInfo = false;
  }

  while (Serial.available())
  {
    Serial.readBytes(serialWordAsBytes, numBytesInSerialWord);
    // If a General Command is received, then...
    if (serialWordAsBytes[0] != CommandFlag)
    {
      /* If a request for buffering data is received, then... */
      if (NthWord < wordCount) {
        idx = NthWord % szArrayWordBuff;
        dataWordBuf[idx] = serialWord;
//        Serial.print(" Buf[");
//        Serial.print(NthWord);
//        Serial.print("] = ");
//        Serial.println(dataWordBuf[idx], HEX);
        NthWord++;

        if ((NthWord % szArrayWordBuff) == 0) {
          for (int i = 0; i < szArrayWordBuff; i++) {
            spi_write(dataWordBuf[i]);
            delay(1);   // bullshit delay between programming a register and readAnalog
            dataWordBufInts[i] = analogRead(A0);      // if this were the 315 LogAmp
          }
          for (int i = 0; i < szArrayWordBuff; i++) {
            Serial.print("Analog 16 bit value = ");
            Serial.println(dataWordBufInts[i]);
          }
          NthWord = 0;
        }
      }
    }
    /* If a Command Word is found then parse into Data, Command and Address */
    else
    {
      Data = (uint16_t)(serialWord >> 16);
      Command = (serialWordAsBytes[1] & CommandBits) >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      newCommandReceived = true;
    }
  }  // End While


  /* Hardware selection and operations. This is where the processing of
     Specific commands occurs. */
  if (newCommandReceived) {
    /* Start by selecting the device that you want to control. Then under
       each device you can select the operation that you want to perform. */
    switch (Address) {
      case Attenuator:
        mosi_pin = ATTEN_MOSI;
        spi_clock = ATTEN_CLOCK;
        spi_select = ATTEN_SEL;
        Data &= ATTEN_Data_Mask;
        spiWord = ((uint32_t)Data << 16) | 0xFF;
        spi_write(spiWord);
        break;

      case LO1_addr:
        mosi_pin = LO1_MOSI;
        spi_clock = SPI_CLOCK_PIN;
        spi_select = LO1_SEL;
        Data32 = ((uint32_t)Data << 4); /* Needed to align <N16:N1> with R[0]<DB19:DB4> */
        switch (Command) {
          case RF_off:
            LO1.Curr.R[6] = LO1.Curr.R[6] & LO1.RFpower_off;
            spiWord = LO1.Curr.R[6];
            break;
          // Set the RFoutB power level, RFoutA is not used and can be disabled.
          case neg_4dBm:
            LO1.Curr.R[6] = (LO1.Curr.R[6] & LO1.Power_Level_Mask) | LO1.neg4dBm;  // neg4dBm is only for documentation
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
            LO1.Curr.R[4] = (LO1.Curr.R[4] & LO1.Mux_Set_TRI);  // Turns on Tristate
            spiWord = LO1.Curr.R[4];
            break;
          case Mux_DLD:
            LO1.Curr.R[4] = LO1.Curr.R[4] | LO1.Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
            spiWord = LO1.Curr.R[4];
            break;
          default:
            writeToSPI = false;  // Do not write to SPI. No commands were received
            break;
        }
        if (writeToSPI) {
          spi_write(spiWord);
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
        wordCount = Data;           // Number of frequencies points to be read
        Serial.print("word count = ");
        Serial.println(wordCount);
        switch (Command) {
          case RF_off:
            LO->Curr.R[4] = LO->Curr.R[4] & LO->RFpower_off;
            spiWord = LO->Curr.R[4];
            break;
          case neg_4dBm:
            // ' | LO->neg4dBm' does nothing and is only for documentation
            LO->Curr.R[4] = (LO->Curr.R[4] & LO->Power_Level_Mask) | LO->neg4dBm;
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
            LO->Curr.R[2] = LO->Curr.R[2] & LO->Mux_Set_TRI;  // Set MuxOut to Tristate
            spiWord = LO->Curr.R[2];
            break;
          case Mux_DLD:
            LO->Curr.R[2] = LO->Curr.R[2] | LO->Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
            spiWord = LO->Curr.R[2];
            break;
          default:
            writeToSPI = false;  // Do not write to SPI. No commands were received
            break;
        }
        if (writeToSPI) {
          spi_write(spiWord);
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

      case LED:
        switch (Command) {
          case LED_off:
            digitalWrite(LED_BUILTIN, LOW);
            break;
          case LED_on:
            digitalWrite(LED_BUILTIN, HIGH);
            break;
          case MSG_REQ:
            displayVersionInfo = true;
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

    newCommandReceived = false;

  } /* End newCommandReceived */
} /* End loop() */


// Program the selected device using shiftOut()
/* This will be replaced by SPI.transfer() for the Ver 3 board */
void spi_write(uint32_t spiData) {
  for (int i = 0; i < numBytesInSerialWord; i++) {
    shiftOut(mosi_pin, spi_clock, MSBFIRST, spiData);
  }
  digitalWrite(spi_select, HIGH);
  digitalWrite(spi_select, LOW);
}


// This function is for development testing. Remove at production
void getLOstatus(MAX2871_LO LO) {
  for (int i = 0; i < LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.R[i], HEX);
  }
}


// This function is for development testing. Remove at production
void getLOstatus(ADF4356_LO LO) {
  for (int i = 0; i < LO.Curr.numRegisters; i++) {
    Serial.print("R[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.println(LO.Curr.R[i], HEX);
  }
}
