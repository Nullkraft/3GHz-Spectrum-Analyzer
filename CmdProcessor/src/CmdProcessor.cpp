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
#endif

/////////// SERIAL COMMUNICATION PROTOCOL ///////////
#define SER_TIMEOUT 200 // Serial port gives up trying to read after 200 mSec
bool useBinary = true;  // Set this to true for binary mode, false for ASCII mode

/*           Serial Word with Command Flag:
    ________________________________________________
   [       Embedded      | Instr- |Addr.|  Command  ]  NOTE: Command Flag
   [       Data          | uction |     |   Flag    ]        always = 0xFF
   [---------------------+--------+-----+-----------]
   [ xxxx_xxxx_xxxx_xxxx | xxxx_x | xxx | 1111_1111 ]
*/
const uint8_t numBytesInSerialWord = 4;
uint32_t serialWord;                                  // Serial Word as 32 bits
uint8_t* serialWordAsBytes = reinterpret_cast<uint8_t*>(&serialWord);   // Serial Word as a byte array

/* All the values required by the spi_write() command */
uint32_t regWord;    // Holds the register contents to be written to the selected device

//*** Parsed values from the incoming 32 bit serial word ***
uint16_t Data16;  // 16 bits
byte Command;
byte Address;
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
uint8_t adc_pin;
/////////////////////////////////////

bool DEBUG = false;

// A new Spectrum Analyzer
SpecAnn SA = SpecAnn();
// A new SpecificInstruction
SpecificInstruction SpInstr = SpecificInstruction();

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

    /* Specific Instructions:
        Page 2 of "Interface Standard 5 - Spectrum Analyzer.odt"
    */
    if (serialWordAsBytes[0] == CommandFlag) {
      SpInstr.parseSpecificInstruction(serialWord);
      Data16 = SpInstr.getData();
      Command = SpInstr.getCommand();
      Address = SpInstr.getAddress();
      /* Hardware selection and operation */
      switch (Address) {
        case SA.Attenuator:
          SA.updateAtten(static_cast<uint8_t>(Data16), SA.ATTEN_SEL);
          break;
        case SA.LO1_addr:
          SA.spi_select = SA.LO1_SEL;
          SA.LO1.set_N_bits(Data16);                              // Set the new INT_N bits into Register 0
          regWord = SA.LO1.Execute(SA.adf4356CmdMap[Command], 0); // This selects from 1 of 7 adf4356 commands
          SA.LO1.update(regWord, SA.spi_select);                     // Write Reg[4] for set_TRI/set_DLD, ELSE Reg[6]
          SA.LO1.update(SA.LO1.Curr.Reg[0], SA.spi_select);          // followed by Reg[0] (REQUIRED by specsheet)
          break;
        case SA.LO2_addr:
          adc_pin = SA.ADC_SEL_315; // Select the ADC that reads the output of the LO2 RF path
          SA.spi_select = SA.LO2_SEL;  // Select the pin for the MAX2871 used for LO2
          SA.updateLORegisters(SA.ptrLO2, SA.LO2_SEL, Command, serialWord);
          break;
        case SA.LO3_addr:
          adc_pin = SA.ADC_SEL_045;
          /***** TODO: Why can't I feed SA.LO3_SEL directly into updateLORegisters() *****/
          SA.spi_select = SA.LO3_SEL;
          SA.updateLORegisters(SA.ptrLO3, SA.LO3_SEL, Command, serialWord);
          break;
        case SA.RefClock:
          SA.clkExecute(Command);
          break;
        case SA.MISC_addr:
          SA.miscExecute(SA.arduinoCmdMap[Command]);
          break;
        default:
          Serial.print(F("Requested Address:"));
          Serial.print(Address);
          Serial.println(F(" not found"));
      }   /* End switch(Address) */
    }
    /* General LO Instructions:
        Page 9 of "Interface Standard 5 - Spectrum Analyzer.odt" */
      // Programs LO2 or LO3 to the requested LO frequency.
    else {
      // M:  Set R[1], bits[14:3] to program the new value for M
      SA.LO->set_M_bits(serialWord);
      // N & F:  Set bits R[0], bits[22:15] for new N, and R[0], bits[14:3] for new F
      SA.LO->set_NF_bits(serialWord);
      // Program the selected LO starting with the higher numbered registers first
      SA.LO->update(SA.LO->Curr.Reg[1], SA.spi_select);
      SA.LO->update(SA.LO->Curr.Reg[0], SA.spi_select);

      // Wait for selected LO2 or LO3 to Lock
      start_PLL_Lock_time = micros();
      while (true) {
        LOCKED = digitalRead(SA.PLL_MUX);  // Check the mux pin to see if we get a lock
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

