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

#ifdef PLATFORMIO
#include <Arduino.h>
#include "SpecAnn.h"
#include "max2871.h"
#include "adf4356.h"
#endif


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
uint32_t regWord;    // Holds the register contents to be written to the selected device

/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data16;  // 16 bits
uint32_t Data32;  // Needed for bit shifting and masking with LO registers
byte Command;
byte Address;
const byte AddressBits = 0x07;  // Mask to select 3 bits of 'Address' from serialWord[1]
const byte CommandFlag = 0xFF;  // Byte pattern to identify a 'Control Word'

unsigned long start_PLL_Lock_time;

#define SER_TIMEOUT 200 // Serial port gives up trying to read after 200 mSec
bool useBinary = true;  // Set this to true for binary mode, false for ASCII mode

bool DEBUG = false;

volatile uint16_t a2dAmplitude;
uint8_t* ampl_byte = (uint8_t*)&a2dAmplitude;
uint8_t adc_pin;

// Assign reference designators from the schematic to the circuit components
ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows a single function to select and operate on LO2 or LO3

/* Maps the Command to the index of the max2871CmdMap or adf4356CmdMap function:
 * This map decouples the Command value from the function-pointer-array index in
 * max2871.h and adf4356.h driver files.
 * See MAX2871Execute(), in max2871.h, or ADF4356Execute(), in adf4356.h, for the
 * list of available commands
 */
enum loCmdList{GERERAL, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, CHANGE_FREQ, TRI, DLD, DIV_MODE, NA=30, };
enum arduinoCmdList{LED_OFF, LED_ON, VERSION, BEGIN_SWEEP, };
enum ckCmdLIst{ALL_OFF, REF_LO_ON, REF_HI_ON, };

uint8_t max2871CmdMap[] {NA, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, NA, TRI, DLD, DIV_MODE };
uint8_t adf4356CmdMap[] {NA, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, NA, TRI, DLD };
uint8_t arduinoCmdMap[] {LED_OFF, LED_ON, VERSION, BEGIN_SWEEP };
uint8_t clkCmdMap[] {ALL_OFF, REF_LO_ON, REF_HI_ON};

/******** SETUP *********************************************************************/
void setup() {
  #ifndef ARDUINO_SAMD_ZERO
    analogReference(EXTERNAL);
  #endif

  Serial.setTimeout(SER_TIMEOUT);
  Serial.begin(2000000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(REF_LO_SEL, OUTPUT);
  pinMode(REF_HI_SEL, OUTPUT);
  pinMode(LO1_SEL, OUTPUT);
  pinMode(LO2_SEL, OUTPUT);
  pinMode(LO3_SEL, OUTPUT);
  pinMode(PLL_MUX, INPUT);
  pinMode(ATTEN_SEL, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // Make sure the LED is off

  // Hardware Initialization for testing of early hardware builds.
  setAtten(0x0, ATTEN_SEL);   // Set 0dB on the digital attenuator
  digitalWrite(REF_LO_SEL, HIGH);  // Enable low frequency referenc clock
  digitalWrite(REF_HI_SEL, LOW);   // Disable high frequency referenc clock

  init_specann();
}

uint8_t hi_byte;
uint8_t lo_byte;
int LOCKED;
const int PLL_Lock_timeout = 500; // usec. Use 195 for testing some failures to lock.


/******** MAIN LOOP ******************************************************************/
void loop() {
  while (Serial.available()) {
    if (useBinary) {
      // Binary Communication for normal usage:
      Serial.readBytes(serialWordAsBytes, numBytesInSerialWord);  // serialWord == serialWordAsBytes
    } else {
      // ASCII Communication for testing Mike's code:
      serialWord = Serial.parseInt();
      if (serialWord == 0) {    // Serial timed out on SER_TIMEOUT
        continue;  // This will just skip to the next iteration of the loop
      }
    }

    if (serialWordAsBytes[0] == CommandFlag) {
      COMMAND_FLAG = true;
    }

    /******** COMMAND FLAG **************************************************************
     * If a Command Flag is found then parse the 3 upper bytes into Data, Command and Address
     * There are 4 bytes in an Instruction Word:
     * Bits serialWord[31:16] are Data
     * Bits serialWord[15:11] are the Command to be performed
     * Bits serialWord[10:8] are the address of the chip to be programmed
     * The serial word can be accessed as a single 32 bit value called serialWord, or as
     * a 2 word array of 16 bit values accessed by using serialWordAsInts[1 or 0], or as
     * a 4 word array of 8 bit values accessed by using serialWordAsBytes[3, 2, 1, or 0].
     * Since serialWordAsBytes[0] is already used to set the COMMAND_FLAG we just ignore it.
     * So that just leaves the upper 3 bytes to process.
     * Data16 is copied from the upper 16 bits of serialWordAsInts[1],
     * Command is copied, masked, and shifted from serialWord Byte[1] bits[15:11], and
     * Address is also copied, masked, and shifted from serialWord Byte[1] bits[10:8].
    */
    if (COMMAND_FLAG) {
      Data16 = serialWordAsInts[1];   // Copy 2 upper bytes to Data16
      Command = serialWordAsBytes[1] >> 3;
      Address = serialWordAsBytes[1] & AddressBits;
      COMMAND_FLAG = false;
    }
    else if (!COMMAND_FLAG) {    // An LO2 or LO3 Instruction has arrived...
      // M:  Set R[1], bits[14:3] to program the new value for M
      LO->set_M_bits(serialWord);
      // N & F:  Set bits R[0], bits[22:15] for new N, and R[0], bits[14:3] for new F
      LO->set_NF_bits(serialWord);
      // Program the selected LO starting with the higher numbered registers first
      LO->update(LO->Curr.Reg[1], spi_select);
      LO->update(LO->Curr.Reg[0], spi_select);

      // Wait for selected LO2 or LO3 to Lock
      start_PLL_Lock_time = micros();
      while (true) {
        LOCKED = digitalRead(PLL_MUX);  // Check the mux pin to see if we get a lock
        analogRead(adc_pin);  // HACK to prime the ADC. Fix the ADC input impedance?
        //  We either get a lock or we check for a timeout.
        if (LOCKED) {
          a2dAmplitude = analogRead(adc_pin);
          // hi_byte = (uint8_t)(a2dAmplitude >> 8);
          // lo_byte = (uint8_t)a2dAmplitude;
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
          // hi_byte = (uint8_t)(a2dAmplitude >> 8) | failed_to_lock;  // Report failure to PC
          // lo_byte = (uint8_t)a2dAmplitude;
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
      Serial.write(hi_byte);  // Big Endian
      Serial.write(lo_byte);
//      Serial.write((byte *)a2dAmplitude, sizeof(a2dAmplitude));   // This is little-endian
    }

    /******** SPECTRUM ANALYZER INSTRUCTIONS **********************************************/
    /* Hardware selection and operations. This is where the processing of
       Specific commands occurs. */
    // Start by selecting the Address of the device then the 'Command' to be performed.
    switch (Address) {
      case Attenuator:
        setAtten((uint8_t)Data16, ATTEN_SEL);
        break;

      case LO1_addr:
        spi_select = LO1_SEL;
        LO1.set_N_bits(Data16);                   // Set the new INT_N bits into Register 0
        regWord = LO1.ADF4356Execute(adf4356CmdMap[Command]); // This selects from 1 of 7 adf4356 commands
        LO1.update(regWord, spi_select);          // Write Reg[4] when doing set_TRI/set_DLD, ELSE Reg[6]
        LO1.update(LO1.Curr.Reg[0], spi_select);  // followed by Reg[0] as required by the spec-sheet.
        break;

      case LO2_addr:
        // Making LO2 active
        LO = &LO2;
        spi_select = LO2_SEL;
        adc_pin = ADC_SEL_315;
        // Now fall through
      case LO3_addr:
        // Making LO3 active
        if (Address == LO3_addr) {
          LO = &LO3;
          spi_select = LO3_SEL;
          adc_pin = ADC_SEL_045;
        }
        regWord = LO->MAX2871Execute(max2871CmdMap[Command], serialWord);
        LO->update(regWord, spi_select);  // Now program the currently selected LO
        break;

      case RefClock:
        clkExecute(Command);
        break;

      case MISC_addr:
        miscExecute(arduinoCmdMap[Command]);
        break;

      default:
        Serial.print(F("Requested Address:"));
        Serial.print(Address);
        Serial.println(F(" not found"));
        break;

    }   /* End switch(Address) */
  }   /* End While serial available */
  COMMAND_FLAG = false;
} /* End loop() */


/* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent.  Why?
    Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's specsheet
*/
void init_specann() {
  // Presets for LO3
  LO3.Curr.Reg[0] = 0x002081C8;  // LO3 = 270 MHz with 66 MHz ref clock
  LO3.Curr.Reg[1] = 0x400103E9;
  LO3.Curr.Reg[2] = 0x98005F42;  // Digital Lock Detect ON
  LO3.Curr.Reg[3] = 0x00001F23;
  LO3.Curr.Reg[4] = 0x63CE803C;
  LO3.Curr.Reg[5] = 0x00400005;
  LO3.Curr.Reg[6] = 0x80005F42;  // Digital Lock Detect ON

  LO3.begin(LO3_SEL, true);
  LO2.begin(LO2_SEL, true);
  LO1.begin(LO1_SEL);
  delay(20);
  LO3.begin(LO3_SEL, false);
  LO2.begin(LO2_SEL, false);
  LO1.begin(LO1_SEL);
}
