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
uint8_t miso_pin;  // Read values from an external A2D chip and Mux pin of LO's
uint8_t mosi_pin;  // Set to write register values to the LO's or the Attenuator
uint8_t spi_clock;
uint8_t spi_select;    // Selects the device to be written to
uint32_t spiWord;  // Holds the register contents to be written to the selected device


/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data;    // 16 bits
uint32_t Data32;  // Needed for bit shifting and masking with LO registers
byte Command;
const byte CommandBits = 0xF8;  // Mask to select 5 bits of Command from serialWord[1]
byte Address;
const byte AddressBits = 0x07;  // Mask to select 3 bits of Address from serialWord[1]
const byte CommandFlag = 0xFF;  // Byte pattern to identify a Control Word


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

// When using the Arduino ADC's for testing
// NOTE: On the Ver2 Spectrum Analyzer you will need a separate Arduino
int adc_pin;    // Set this to match the currently selected LO2 or LO3
const int LO2_ADC_sel = A0;
const int LO3_ADC_sel = A1;

// Addresses for selecting the various hardware ICs
const int Attenuator = 0;
const int LO1_addr   = 1;
const int LO2_addr   = 2;
const int LO3_addr   = 3;
const int RefClock   = 4;
const int PLL_Mux    = 5;
// NOT_USED          = 6;
const int LED        = 7;


// BitMask for programming the registers of the Attenuator IC
const uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data



/*********** HARDWARE DEFINITIONS END *******/


// Assign reference designators from the schematic to the LO ojbect of choice
ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows a single function to select and operate on LO2 or LO3


// Decide if one of the LO's received a command to update a register
bool SPI_WRITE_TO_LO;

enum STATE{WAIT, SEND, RECEIVE, PROCESS} state = WAIT;  // Initial state == WAIT


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(miso_pin, OUTPUT);
  pinMode(mosi_pin, INPUT);
  pinMode(spi_clock, OUTPUT);
  pinMode(spi_select, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.setTimeout(200);
  Serial.begin(2000000);

  SPI_WRITE_TO_LO = true;   // This goes false for commands that don't program a chip
  num_data_points = 0;      // Used when sending LO2 and LO3 ADC outputs  to  the  PC
  num_points_processed = 0;
}


byte buf_index = 0;
uint32_t frac_div_F;
uint32_t frac_mod_M;
uint32_t counter_N;
uint32_t tmp;
uint8_t* byteTmp = (byte*)&tmp;   // Tmp as a byte array
uint16_t* intTmp = (int*)&tmp;    // Tmp as an int array



void loop() {

  while (Serial.available())
  {
    Serial.readBytes(serialWordAsBytes, numBytesInSerialWord);

    // If a General LO Instruction or LO Data Packet is received, then...
    if (serialWordAsBytes[0] != CommandFlag)
    {
      if (state == RECEIVE) {
        Serial.println("Received a frequency data packet");
        data_buf_as_word[buf_index] = serialWord; //serialWord = 32bit value from serialWordAsBytes
        buf_index++;
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

      // With a full buffer we parse the serial data into F, M and N values
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

          // Program the selected LO in descending order of register numbers
          spi_write(LO->Curr.Reg[1]);
          spi_write(LO->Curr.Reg[0]);

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
        for(buf_index = 0; buf_index < size_data_buf; buf_index++) {
          Serial.write(data_buf_as_int[buf_index]);
        }
        Serial.write(0xFF);
        Serial.write(0xFF);   // End-of-Block (EOB) transmission
        state = RECEIVE;
        buf_index = 0;    // Reset buffer index for receiving next block of data packets
      }
    }

    // If a Command Flag is found then parse the 32 bit word into
    // Data, Command and Address
    else
    {
      Data = (uint16_t)(serialWord >> 16);
      Command = (serialWordAsBytes[1] & CommandBits) >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      NEW_INSTRUCTION_RECEIVED = true;
      Serial.print("Serial command = ");
      Serial.println(serialWord, HEX);
    }
  }  // End While


  /* Hardware selection and operations. This is where the processing of
     Specific commands occurs. */
  if (NEW_INSTRUCTION_RECEIVED) {
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
        Data32 = ((uint32_t)Data << 4);    /* Aligns INT_N bits <N16:N1> with R[0]<DB19:DB4> */
        LO1.Curr.Reg[0] &= LO1.INT_N_Mask; /* Clear old INT_N bits from Regist 0 */
        LO1.Curr.Reg[0] |= Data32;         /* Insert new INT_N bits into Register 0*/
        switch (Command) {
          case RF_off:
            LO1.Curr.Reg[6] = LO1.Curr.Reg[6] & LO1.RFpower_off;
            spiWord = LO1.Curr.Reg[6];
            break;
          // Set the RFoutB power level, RFoutA is not used and can be disabled.
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
            LO1.Curr.Reg[4] = LO1.Curr.Reg[4] | LO1.Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
            spiWord = LO1.Curr.Reg[4];
            break;
          default:
            SPI_WRITE_TO_LO = false;  // Do not write to SPI. No commands were received
            break;
        }
        if (SPI_WRITE_TO_LO) {
          spi_write(spiWord);
          spi_write(LO1.Curr.Reg[0]);
          Serial.print("Current register 0 = ");
          Serial.println(LO1.Curr.Reg[0], HEX);
          Serial.print("Current register 6 = ");
          Serial.println(LO1.Curr.Reg[6], HEX);
        }
        SPI_WRITE_TO_LO = true;  // Reset for next incoming serial command
        getLOstatus(LO1);
        break;

      case LO2_addr:
        /* Making LO2 active */
        LO = &LO2;
        mosi_pin = LO2_MOSI;
        spi_select = LO2_SEL;
        adc_pin = LO2_ADC_sel;  // Selects the ADC associated with LO2 output
      // NOTE: break is missing so the code falls through to 'case LO3_addr'
      case LO3_addr:
        /* Making LO3 active */
        if (Address = LO3_addr) {
          LO = &LO3;
          mosi_pin = LO3_MOSI;
          spi_select = LO3_SEL;
          adc_pin = LO3_ADC_sel;  // Selects the ADC associated with LO3 output
        }
        // Set the remainder of the SPI pins
        spi_clock = SPI_CLOCK_PIN;
        num_data_points = Data;    // Number of frequency points to be read
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
            SPI_WRITE_TO_LO = false;  // Do not write to SPI. No commands were received
            break;
        }
        if (SPI_WRITE_TO_LO) {
          spi_write(spiWord);
        }
        SPI_WRITE_TO_LO = true;  // Reset for next incoming serial command
        //        getLOstatus(*LO);
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

    NEW_INSTRUCTION_RECEIVED = false;

  } /* End NEW_INSTRUCTION_RECEIVED */
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
