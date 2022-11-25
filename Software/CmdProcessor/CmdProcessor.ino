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
   [       Embedded      | Instr- |Addr.|  Command  ]  NOTE: Command Flag
   [       Data          | uction |     |   Flag    ]        always = 0xFF
   [---------------------+--------+-----+-----------]
   [ xxxx_xxxx_xxxx_xxxx | xxxx_x | xxx | 1111_1111 ]
*/
const uint8_t numBytesInSerialWord = 4;
uint32_t serialWord;                                  // Serial Word as 32 bits
uint8_t* serialWordAsBytes = (uint8_t*)&serialWord;   // Serial Word as a byte array
uint16_t* serialWordAsInts = (uint16_t*)&serialWord;  // Serial Word as a int array


// Command Flag 0xFF indicates that a new instruction was received by the Arduino
bool COMMAND_FLAG = false;

/* All the values required by the spi_write() command */
uint8_t spi_select;  // Currently selected LO (1, 2 or 3) that is to be programmed
uint32_t spiWord;    // Holds the register contents to be written to the selected device


/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data16;  // 16 bits
uint32_t Data32;  // Needed for bit shifting and masking with LO registers
byte Command;
const byte CommandBits = 0xF8;  // Mask to select 5 bits of 'Command' from serialWord[1]
byte Address;
const byte AddressBits = 0x07;  // Mask to select 3 bits of 'Address' from serialWord[1]
const byte CommandFlag = 0xFF;  // Byte pattern to identify a 'Control Word'


/*********** ARDUINO PIN DEFINITIONS ***********/
const int LO1_SEL = A3;
const int LO2_SEL = 3;
const int LO3_SEL = A4;
const int REF_LO_SEL = 8;
const int REF_HI_SEL = 9;
const int ADC_SEL_045 = A0;  // ADC for LO2
const int ADC_SEL_315 = A1;  // ADC for LO3
const int PLL_MUX = A2;      // Equals physical pin 16 on Port C (use PCMSK1)
const int ATTEN_SEL = A5;
//const int SPI_MOSI  = 11;   // Reserved by the SPI Library
//const int SPI_MISO  = 12;   // Reserved by the SPI Library
//const int SPI_CLOCK = 13;   // Reserved by the SPI Library

int adc_pin = ADC_SEL_315;  // Default sets this to either to the ADC for LO2 output


// Addresses for selecting the various hardware ICs
const int Attenuator = 0;
const int LO1_addr = 1;
const int LO2_addr = 2;
const int LO3_addr = 3;
const int RefClock = 4;
const int MISC_addr = 7;


// BitMask for programming the registers of the Attenuator IC
const uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data


/*********** HARDWARE DEFINITIONS END *******/

// Assign reference designators from the schematic to the LO ojbect of choice
ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO = new MAX2871_LO();  // Allows a single function to select and operate on LO2 or LO3


byte buf_index = 0;
uint32_t frac_div_F;
uint32_t frac_mod_M;
uint32_t counter_N;
uint32_t z;
uint8_t* byteZ = (byte*)&z;      // Tmp as a byte array
uint16_t* intZ = (uint16_t*)&z;  // Tmp as an int array

static String nameLO;
volatile uint16_t a2dAmplitude;
uint8_t* ampl_byte = (uint8_t*)&a2dAmplitude;

unsigned long start_PLL_Lock_time;

#define USE_BINARY  // Comment out for ASCII serial commuinication

bool DEBUG = false;

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
  digitalWrite(LED_BUILTIN, LOW);  // Make sure the LED is off

  Serial.setTimeout(200);
  Serial.begin(2000000);

  //  MUX pin interrupt
  // PCICR |= 0b00000010;   // turn on port C pin-change interrupt(s)
  // PCMSK1 |= 0b00000100;  // PCINT10

  // Presets for LO3
  LO3.Curr.Reg[0] = 0x002081C8;  // LO3 = 270 MHz with 66 MHz ref clock
  LO3.Curr.Reg[1] = 0x400103E9;
  LO3.Curr.Reg[2] = 0x98005F42;  // Digital Lock Detect ON
  LO3.Curr.Reg[3] = 0x00001F23;
  LO3.Curr.Reg[4] = 0x63CE803C;
  LO3.Curr.Reg[5] = 0x00400005;
  LO3.Curr.Reg[6] = 0x80005F42;  // Digital Lock Detect ON

  // Hardware Initialization for testing of early hardware builds.
  spiWriteAtten(0x0, ATTEN_SEL);   // Set 0dB on the digital attenuator
  digitalWrite(REF_LO_SEL, HIGH);  // Enable low frequency referenc clock
  digitalWrite(REF_HI_SEL, LOW);   // Disable high frequency referenc clock
  /* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent.  Why?
     TODO: Investigate LO1 locking anomaly
     Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's documentation
  */
  initialize_LO3(LO3_SEL, true);
  initialize_LO2(LO2_SEL, true);
  initialize_LO1(LO1_SEL);
  delay(20);
  initialize_LO3(LO3_SEL, false);
  initialize_LO2(LO2_SEL, false);
  initialize_LO1(LO1_SEL);
}

int hi_byte = 0;
int lo_byte = 0;
int LOCKED;
const int PLL_Lock_timeout = 1000; // usec. Use 195 for testing some failures to lock.


/******** MAIN LOOP ******************************************************************/
void loop() {
LoopTop:
  while (Serial.available()) {
#ifdef USE_BINARY
    // Binary Communication for normal usage:
    // Blocks until numBytesInSerialWord==4 has been received
    Serial.readBytes(serialWordAsBytes, numBytesInSerialWord);  // serialWord == serialWordAsBytes
#else
    // ASCII Communication for testing Mike's code:
    // Blocks until numBytesInSerialWord==4 has been received
    serialWord = Serial.parseInt();
    if (serialWord == 0) {
      goto LoopTop;
    }
#endif

    if (serialWordAsBytes[0] == CommandFlag) {
      COMMAND_FLAG = true;
    }

    /******** COMMAND FLAG **************************************************************/
    // If a Command Flag is found then parse the 32 bits into Data, Command and Address
    if (COMMAND_FLAG) {
      Data16 = serialWordAsInts[1];   // Copy 2 upper bytes to Data16
      Command = (serialWordAsBytes[1] & CommandBits) >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      COMMAND_FLAG = false;
    }

    // !CommandFlag - LO2/3 Instruction arrived...
    else if (!COMMAND_FLAG) {
      // M:  Clear Reg[1], bits [14:3], before accepting a new M word
      LO->Curr.Reg[1] = LO->Curr.Reg[1] & LO->M_clr;
      // M:  Mask and set bits[14:3] to program the new value for M
      LO->Curr.Reg[1] = LO->Curr.Reg[1] | ((serialWord >> 5) & LO->M_set);
      // N and F:  Clear Reg[0], bits [22:3], before accepting new N and F words
      LO->Curr.Reg[0] = LO->Curr.Reg[0] & LO->NF_clr;
      // N:  Mask and set bits [22:15] to program the new value for N
      LO->Curr.Reg[0] = LO->Curr.Reg[0] | ((serialWord << 15) & LO->N_set);
      // F:  Mask and set bits [14:3] to program the new value for F
      LO->Curr.Reg[0] = LO->Curr.Reg[0] | ((serialWord >> 17) & LO->F_set);

      // Program the selected LO starting with the higher numbered registers first
      spiWriteLO(LO->Curr.Reg[1], spi_select);
      spiWriteLO(LO->Curr.Reg[0], spi_select);

      // Wait for selected LO2 or LO3 to Lock
      start_PLL_Lock_time = micros();
      while (true) {
        LOCKED = digitalRead(PLL_MUX);  // Check the mux pin to see if we get a lock
        analogRead(adc_pin);  // HACK to prime the ADC. Fix the ADC input impedance?
        //  We either get a lock or we get a timeout.
        if (LOCKED) {
          a2dAmplitude = analogRead(adc_pin);
          hi_byte = ampl_byte[1];
          lo_byte = ampl_byte[0];
          break;
        }
        /* Trigger the timeout if we don't get a lock. We still want the amplitude data
         * but we can send a warning so the user knows that there is some inaccuracy.
         * The Arduino ADC is only 10 bits. That means there are 6 bits that can be used
         * for sending a variety of 'messages' embedded in the amplitude data.
        */
        if ((micros()-start_PLL_Lock_time) > PLL_Lock_timeout) {
          a2dAmplitude = analogRead(adc_pin);
          hi_byte = ampl_byte[1] | 0xF0;    // Set the 4 MSbits to indicate failure to lock
          lo_byte = ampl_byte[0];
          break;
        }
        // Bypass the lock detect when debugging the Arduino by itself.
        if (DEBUG) {
          delayMicroseconds(100);
          break;
        }
      }

      // Send the amplitude as individual bytes from the ADC to the PC for plotting
      // Big Endian
      Serial.write(hi_byte);
      Serial.write(lo_byte);
//      Serial.write((byte *)a2dAmplitude, sizeof(a2dAmplitude));   // This is little-endian
    }

    /******** SPECTRUM ANALYZER INSTRUCTIONS **********************************************/
    /* Hardware selection and operations. This is where the processing of
       Specific commands occurs. */
    // Start by selecting the Address of the device then the operation to be performed.
    switch (Address) {
      case Attenuator:
        Data16 &= ATTEN_Data_Mask;
        spiWriteAtten((uint8_t)Data16, ATTEN_SEL);
        break;

      case LO1_addr:
        nameLO = "LO1";
        spi_select = LO1_SEL;
        Data32 = ((uint32_t)Data16 << 4);   // Aligns INT_N bits <N16:N1> with R[0]<DB19:DB4>
        LO1.Curr.Reg[0] &= LO1.INT_N_Mask;  // Clear the INT_N bits from Register 0
        LO1.Curr.Reg[0] |= Data32;          // Set the new INT_N bits into Register 0
        switch (Command) {
          case RF_off:
            LO1.Curr.Reg[6] = LO1.Curr.Reg[6] & LO1.RFpower_off;
            spiWord = LO1.Curr.Reg[6];
            break;
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
            break;
        }
        // Now program LO1 with the new settings
        spiWriteLO(spiWord, spi_select);          // Write Reg[4] or Reg[6] depending on the given command
        spiWriteLO(LO1.Curr.Reg[0], spi_select);
        break;

      case LO2_addr:
        // Making LO2 active
        nameLO = "LO2";
        LO = &LO2;
        spi_select = LO2_SEL;
        adc_pin = ADC_SEL_315;
        // Now fall through
      case LO3_addr:
        // Making LO3 active
        if (Address == LO3_addr) {
          nameLO = "LO3";
          LO = &LO3;
          spi_select = LO3_SEL;
          adc_pin = ADC_SEL_045;
        }
        switch (Command) {
          case RF_off:
            LO->Curr.Reg[4] = LO->Curr.Reg[4] & LO->RFpower_off;
            spiWord = LO->Curr.Reg[4];
            break;
          case neg_4dBm:
            // OR'ing with LO->neg4dBm does nothing and is only added for documentation
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
          case DIV_MODE:
            LO->Curr.Reg[4] &= LO->RFOUT_DIV_MASK;
            LO->Curr.Reg[4] |= (serialWord & !LO->RFOUT_DIV_MASK);
            spiWord = LO->Curr.Reg[4];
            break;
          default:
            break;
        }
        // Now program the currently selected LO
        spiWriteLO(spiWord, spi_select);
        break;  // End case LO2 OR case LO3

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
            if (!DEBUG) {
              Serial.write(0xFF);
              Serial.write(0xFF);
            }
            break;
          case LED_on:
            digitalWrite(LED_BUILTIN, HIGH);
            break;
          case MSG_REQ:
            Serial.println("- WN2A Spectrum Analyzer CmdProcessor Oct. 2021");
            break;
          case SWEEP_START:
            break;
          case SWEEP_END:
            Serial.write(0xFF);
            Serial.write(0xFF);
            break;
          case RESET:
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

    }  // End switch(Address)
  }    // End While serial available
  COMMAND_FLAG = false;
} /* End loop() */





/* MUX Interrupt
   When the mux pin is configured for Digital Lock Detect output
   we can read the status of the pin from here.
*/
// ISR(PCINT1_vect) {
//   if (DEBUG) {
//     Serial.print(nameLO);
//     Serial.println(" Lock ");
//   }
// }


void initialize_LO1(uint8_t selectPin) {
  nameLO = "LO1";
  spiWriteLO(LO1.Curr.Reg[4], selectPin);  // Enable LO1 lock detect
  for (int x = 13; x >= 0; x--) {
    z = LO1.Curr.Reg[x];
    spiWriteLO(z, selectPin);  // Program LO1=3776.52 MHz with LD on Mux
  }
  spiWriteLO(LO1.Curr.Reg[14], selectPin);  // Tri-stating the mux output disables LO1 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void initialize_LO2(uint8_t selectPin, bool initialize) {
  nameLO = "LO2";
  spiWriteLO(LO2.Curr.Reg[5], selectPin);  // First we program LO2 Register 5
  if (initialize) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    z = LO2.Curr.Reg[x];       // Program remaining registers where LO2=3915 MHz
    spiWriteLO(z, selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  spiWriteLO(LO2.Curr.Reg[6], selectPin);  // Tri-stating the mux output disables LO2 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void initialize_LO3(uint8_t selectPin, bool initialize) {
  nameLO = "LO3";
  spiWriteLO(LO3.Curr.Reg[5], selectPin);  // First we program LO3 Register 5
  if (initialize) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    z = LO3.Curr.Reg[x];                     // Program remaining registers where LO3=270 MHz
    spiWriteLO(LO3.Curr.Reg[x], selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  spiWriteLO(LO3.Curr.Reg[6], selectPin);  // Tri-stating the mux output disables LO3 lock detect
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