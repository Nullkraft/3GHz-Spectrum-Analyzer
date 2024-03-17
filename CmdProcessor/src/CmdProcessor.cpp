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

#include <Arduino.h>
#include <SPI.h>
#include "SpecAnn.h"

/////////// SERIAL COMMUNICATION PROTOCOL ///////////
#define SER_TIMEOUT 200 // Serial port gives up trying to read after 200 mSec
bool useBinary = true;  // Set this to true for binary mode, false for ASCII mode

/*           Serial Word with Command Flag:
    ,_________________________________________________,
   /_____________________/________/_____/____________/|
   |       Embedded      | Instr- |Addr.|  Command  | | NOTE: Command Flag
   |       Data          | uction |     |   Flag    | |        always = 0xFF
   |---------------------+--------+-----+-----------| |
   | xxxx_xxxx_xxxx_xxxx | xxxx_x | xxx | 1111_1111 | |
   |________________________________________________|/
*/
const uint8_t numBytesInSerialWord = 4;
uint32_t serialWord;                                  // Serial Word as 32 bits
uint8_t* serialWordAsBytes = reinterpret_cast<uint8_t*>(&serialWord);   // Serial Word as a byte array

const byte CommandFlag = 0xFF;  // Byte pattern to identify a 'Control Word'
uint8_t hi_byte;
uint8_t lo_byte;
/////////// END SERIAL COMMUNICATION PROTOCOL ///////////

///////// Move to SpecAnn.h /////////
unsigned long start_PLL_Lock_time;
const int PLL_Lock_timeout = 500; // usec. Use 195 for testing some failures to lock.
int LOCKED;
volatile uint16_t a2dAmplitude;
uint8_t* ampl_byte = (uint8_t*)&a2dAmplitude;
// uint8_t adc_pin;
/////////////////////////////////////

bool DEBUG = false;

// A new Spectrum Analyzer
SpecAnn SA = SpecAnn();
// A new SpecificInstruction
SpecificInstruction SpecificInstr = SpecificInstruction();
uint16_t Data16;
byte cmdIdx;
byte Address;


/******** SETUP *********************************************************************/
void setup() {
  #ifndef ARDUINO_SAMD_ZERO
    analogReference(EXTERNAL);
  #endif
  Serial.setTimeout(SER_TIMEOUT);
  Serial.begin(2000000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SA.REF_LO_SEL, OUTPUT);
  pinMode(SA.REF_HI_SEL, OUTPUT);
  pinMode(SA.LO1_SEL, OUTPUT);
  pinMode(SA.LO2_SEL, OUTPUT);
  pinMode(SA.LO3_SEL, OUTPUT);
  pinMode(SA.PLL_MUX, INPUT);
  pinMode(SA.ATTEN_SEL, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // Make sure the LED is off

  SA.updateAtten(0x0, SA.ATTEN_SEL);   // Set 0dB on the digital attenuator
  SA.init_specann();
}


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
        continue;
      }
    }

    // Specific Instructions: Pg 2 of "Interface Standard 5 - Spectrum Analyzer.odt"
    if (serialWordAsBytes[0] == CommandFlag) {
      SpecificInstr.parseSpecificInstruction(serialWord);
      Data16 = SpecificInstr.getData();
      cmdIdx = SpecificInstr.getCommand();
      Address = SpecificInstr.getAddress();
      SA.programHW(Data16, cmdIdx, Address, serialWord);
    }
    // General LO Instructions: Pg 9 of "Interface Standard 5 - Spectrum Analyzer.odt"
    // Programs the selected MAX2871 for LO2 or LO3 to the requested LO frequency.
    else {
      // M:  Set R[1], bits[14:3] to program the new value for M
      SA.LO->set_M_bits(serialWord);
      // N & F:  Set bits R[0], bits[22:15] for new N, and R[0], bits[14:3] for new F
      SA.LO->set_NF_bits(serialWord);
      // Program the selected LO starting with the higher numbered registers first
      SA.LO->update(SA.LO->Curr.Reg[1], SA.select_pin);
      SA.LO->update(SA.LO->Curr.Reg[0], SA.select_pin);

      // Wait for selected LO2 or LO3 to Lock
      start_PLL_Lock_time = micros();
      while (true) {
        LOCKED = digitalRead(SA.PLL_MUX);  // Check the mux pin to see if we get a lock
        analogRead(SA.adc_pin);  // HACK to prime the ADC. Fix the ADC input impedance?
        //  We either get a lock or we check for a timeout.
        if (LOCKED) {
          a2dAmplitude = analogRead(SA.adc_pin);
          hi_byte = ampl_byte[1];
          lo_byte = ampl_byte[0];
          break;
        }
        /* Trigger the timeout if we don't get a lock. We still want the amplitude
         * data so a 'failure to lock' warning is appended to the data so that the
         * user is notified that the amplitude may not be exact.
         * The ADC will eventually be a 12 bit device. The remaining 4 bits can be
         * used for sending a variety of messages embedded with the amplitude data.
        */
        if ((micros()-start_PLL_Lock_time) > PLL_Lock_timeout) {
          a2dAmplitude = analogRead(SA.adc_pin);
          hi_byte = ampl_byte[1] | SA.failed_to_lock;  // Send failure report to PC
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
    }

  }   /* End While serial available */
} /* End loop() */

