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
#include "SpecAnn.h"

#include "max2871.h"
#include "adf4356.h"


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
// uint32_t z;
// uint8_t* byteZ = (byte*)&z;      // Tmp as a byte array
// uint16_t* intZ = (uint16_t*)&z;  // Tmp as an int array

static String nameLO;
volatile uint16_t a2dAmplitude;
uint8_t* ampl_byte = (uint8_t*)&a2dAmplitude;

unsigned long start_PLL_Lock_time;

#define USE_BINARY  // Comment out for ASCII serial commuinication

bool DEBUG = false;

/******** SETUP *********************************************************************/
void setup() {
  #ifndef ARDUINO_SAMD_ZERO
    analogReference(EXTERNAL);
  #endif

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

  LAST_STATE = ABOVE_NOISE_FLOOR;
}

int hi_byte = 0;
int lo_byte = 0;
int LOCKED;
const int PLL_Lock_timeout = 500; // usec. Use 195 for testing some failures to lock.


/******** MAIN LOOP ******************************************************************/
void loop() {
[[maybe_unused]] LoopTop:
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

    /******** COMMAND FLAG **************************************************************
     * If a Command Flag is found then parse the 3 upper bytes into Data, Command and Address
     * There are 4 bytes in an Instruction Word:
     * Bits serialWord[31:16] are Data
     * Bits serialWord[15:11] contain the Command to be performed
     * Bits serialWord[10:8] contain the address of the chip to be programmed
     * The serial word can be accessed as a single 32 bit value called serialWord, or as
     * a 2 word array of 16 bit values accessed by using serialWordAsInts[1 or 0], or as
     * a 4 word array of 8 bit values accessed by using serialWordAsBytes[3, 2, 1, or 0].
     * Since serialWordAsBytes[0] is already used to set the COMMAND_FLAG we just ignore it.
     * So that just leaves the upper 3 bytes to process.
     * Data16 is copied from the upper 16 bits of serialWord Byte[3] and Byte[2],
     * Command is copied, masked, and shifted from serialWord Byte[1] bits[15:11], and
     * Address is also copied, masked, and shifted from serialWord Byte[1] bits[10:8].
    */
    if (COMMAND_FLAG) {
      Data16 = serialWordAsInts[1];   // Copy 2 upper bytes to Data16
      Command = serialWordAsBytes[1] >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      COMMAND_FLAG = false;
    }

    // If !CommandFlag - means that an LO2/3 Instruction arrived...
    else if (!COMMAND_FLAG) {
      // M:  Set R[1], bits[14:3] to program the new value for M
      LO->set_reg1(serialWord);
      // N & F:  Set bits R[0], bits[22:15] for new N, and R[0], bits[14:3] for new F
      LO->set_reg0(serialWord);
      // Program the selected LO starting with the higher numbered registers first
      LO->spiWrite(LO->Curr.Reg[1], spi_select);
      LO->spiWrite(LO->Curr.Reg[0], spi_select);

      // Wait for selected LO2 or LO3 to Lock
      start_PLL_Lock_time = micros();
      while (true) {
        LOCKED = digitalRead(PLL_MUX);  // Check the mux pin to see if we get a lock
        analogRead(adc_pin);  // HACK to prime the ADC. Fix the ADC input impedance?
        //  We either get a lock or we check for a timeout.
        if (LOCKED) {
          a2dAmplitude = analogRead(adc_pin);
          hi_byte = ampl_byte[1];
          lo_byte = ampl_byte[0];
          break;
        }
        /* Trigger the timeout if we don't get a lock. We still want the amplitude
         * data so a 'failure to lock' warning is appended to the data so that the
         * user is notified that the amplitude may not be exact.
         * The ADC will eventually be a 12 bit device. That means there are 4 bits
         * for sending a variety of 'messages' embedded in the amplitude data.
        */
        if ((micros()-start_PLL_Lock_time) > PLL_Lock_timeout) {
          a2dAmplitude = analogRead(adc_pin);
          hi_byte = ampl_byte[1] | failed_to_lock;  // Report failure to PC
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
        Data32 = ((uint32_t)Data16 << 4);          // Aligns INT_N bits <N16:N1> with R[0]<DB19:DB4>
        LO1.Curr.Reg[0] &= LO1.INT_N_Mask;         // Clear the INT_N bits from Register 0
        LO1.Curr.Reg[0] |= Data32;                 // Set the new INT_N bits into Register 0
        spiWord = LO1.ADF4356Execute(Command);  // This selects from 1 of 7 commands to run
        LO1.spiWrite(spiWord, spi_select);         // Write Reg[4] when doing set_TRI/set_DLD, ELSE Reg[6]
        LO1.spiWrite(LO1.Curr.Reg[0], spi_select); // followed by Reg[0] as required by the spec-sheet.
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
        if (Command == static_cast<int>(devices::DIV_MODE)) {
          spiWord = LO->MAX2871Execute(Command);
        } else {
          spiWord = LO->MAX2871ExecuteWithArg(Command, serialWord);
        }
        // Now program the currently selected LO
        LO->spiWrite(spiWord, spi_select);
        break;  // End case LO2 OR case LO3

      case RefClock:
        switch (Command) {
          // Turn both ref_clocks off
          case all_ref_off:
            digitalWrite(REF_LO_SEL, LOW);
            digitalWrite(REF_HI_SEL, LOW);
            break;
          // Turn off 66.666 MHz ref_clock AND turn on 66.000 MHz ref_clock
          case ref_LO:
            digitalWrite(REF_HI_SEL, LOW);
            digitalWrite(REF_LO_SEL, HIGH);
            break;
          // Turn off 66.000 MHz ref_clock AND turn on 66.666 MHz ref_clock
          case ref_HI:
            digitalWrite(REF_LO_SEL, LOW);
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
            Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
            break;
          case SWEEP_START:
            break;
          case SWEEP_END:
            LAST_STATE = ABOVE_NOISE_FLOOR;   // Reset for start of next sweep
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
        Serial.print(F("Requested Address:"));
        Serial.print(Address);
        Serial.println(F(" not found"));
        break;

    }  // End switch(Address)
  }    // End While serial available
  COMMAND_FLAG = false;
} /* End loop() */





void initialize_LO1(uint8_t selectPin) {
  nameLO = "LO1";
  LO1.spiWrite(LO1.Curr.Reg[4], selectPin);  // Enable LO1 lock detect
  // spiWriteLO(LO1.Curr.Reg[4], selectPin);  // Enable LO1 lock detect
  for (int x = 13; x >= 0; x--) {
    LO1.spiWrite(LO1.Curr.Reg[x], selectPin);  // Program LO1=3776.52 MHz with LD on Mux
    // spiWriteLO(LO1.Curr.Reg[x], selectPin);  // Program LO1=3776.52 MHz with LD on Mux
  }
  LO1.spiWrite(LO1.Curr.Reg[14], selectPin);  // Tri-stating the mux output disables LO1 lock detect
  // spiWriteLO(LO1.Curr.Reg[14], selectPin);  // Tri-stating the mux output disables LO1 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void initialize_LO2(uint8_t selectPin, bool initialize) {
  nameLO = "LO2";
  LO2.spiWrite(LO2.Curr.Reg[5], selectPin);  // First we program LO2 Register 5
  if (initialize) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    LO2.spiWrite(LO2.Curr.Reg[x], selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  LO2.spiWrite(LO2.Curr.Reg[6], selectPin);  // Tri-stating the mux output disables LO2 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void initialize_LO3(uint8_t selectPin, bool initialize) {
  nameLO = "LO3";
  LO3.spiWrite(LO3.Curr.Reg[5], selectPin);  // First we program LO3 Register 5
  if (initialize) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    LO3.spiWrite(LO3.Curr.Reg[x], selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  LO3.spiWrite(LO3.Curr.Reg[6], selectPin);  // Tri-stating the mux output disables LO3 lock detect
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

