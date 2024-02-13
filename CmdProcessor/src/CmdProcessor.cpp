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
uint16_t* serialWordAsInts = reinterpret_cast<uint16_t*>(&serialWord);  // Serial Word as a int array

// Command Flag 0xFF indicates that a new instruction was received by the Arduino
bool COMMAND_FLAG = false;

/* All the values required by the spi_write() command */
uint8_t spi_select;  // Currently selected LO (1, 2 or 3) that is to be programmed
uint32_t regWord;    // Holds the register contents to be written to the selected device

/*** Parsed values from the incoming 32 bit serial word ***/
uint16_t Data16;  // 16 bits
byte Command;
byte Address;
const byte AddressBits = 0x07;  // Mask out 3 bits of 'Register Address' from serialWord[1]
const byte CommandFlag = 0xFF;  // Byte pattern to identify a 'Control Word'

unsigned long start_PLL_Lock_time;

#define SER_TIMEOUT 200 // Serial port gives up trying to read after 200 mSec
bool useBinary = true;  // Set this to true for binary mode, false for ASCII mode

bool DEBUG = false;

volatile uint16_t a2dAmplitude;
uint8_t* ampl_byte = (uint8_t*)&a2dAmplitude;
uint8_t adc_pin;

// Spectrum Analyzer command-&-control
SpecAnn SA = SpecAnn();

/* Command-to-Function Mapping:
 * 
 * Each instruction found in the API ('Instruction List XXX.ods') has an
 * associated Address and Command. The Address identifies the component
 * and the Command identifies the action to be performed.
 * 
 * The 'Command Number' is found in the API document and is 5 bits long.
 * It is ideal as an index into one of the xxxYYYYCmnMap[] arrays. The
 * CmdMap arrays decouple the API Command Number from the layout of the
 * function-pointer arrays found in the max2871.h and adf4356.h files.
 * 
 * The enums provide descriptive command names for the function-pointer
 * arrays instead of using the Command Numbers.
 * 
 * For a list of available commands and their corresponding functions, refer
 * to the Execute() function in max2871.h and the Execute() function in adf4356.h.
 */
enum loCmdList{GERERAL, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, CHANGE_FREQ, TRI, DLD, DIV_MODE, NA=30, };
enum arduinoCmdList{LED_OFF, LED_ON, VERSION, BEGIN_SWEEP, };
enum ckCmdLIst{ALL_OFF, REF_LO_ON, REF_HI_ON, };

// Command map declarations
uint8_t max2871CmdMap[] {NA, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, NA, TRI, DLD, DIV_MODE };
uint8_t adf4356CmdMap[] {NA, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, NA, TRI, DLD };
uint8_t arduinoCmdMap[] {LED_OFF, LED_ON, VERSION, BEGIN_SWEEP };
uint8_t clkCmdMap[] {ALL_OFF, REF_LO_ON, REF_HI_ON};

uint8_t hi_byte;
uint8_t lo_byte;
int LOCKED;
const int PLL_Lock_timeout = 500; // usec. Use 195 for testing some failures to lock.

void init_specann();

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
  init_specann();
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
      // This is where we set the frequency of LO2 or LO3
      // M:  Set R[1], bits[14:3] to program the new value for M
      SA.LO->set_M_bits(serialWord);
      // N & F:  Set bits R[0], bits[22:15] for new N, and R[0], bits[14:3] for new F
      SA.LO->set_NF_bits(serialWord);
      // Program the selected LO starting with the higher numbered registers first
      SA.LO->update(SA.LO->Curr.Reg[1], spi_select);
      SA.LO->update(SA.LO->Curr.Reg[0], spi_select);
      // Usage example
      // MAX2871_LO LO1, LO2, LO3;
      // LO1.setFrequency(F, M, N, selectPinForLO1); // Specify the select pin for LO1
      // LO2.setFrequency(F, M, N, selectPinForLO2); // Specify the select pin for LO2
      // LO3.setFrequency(F, M, N, selectPinForLO3); // Specify the select pin for LO3

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

    /******** SPECTRUM ANALYZER INSTRUCTIONS **********************************************/
    /* Hardware selection and operations. This is where the processing of
       Specific commands occurs. */
    switch (Address) {
      case SA.Attenuator:
        SA.updateAtten(static_cast<uint8_t>(Data16), SA.ATTEN_SEL);
        break;
      case SA.LO1_addr:
        spi_select = SA.LO1_SEL;
        SA.LO1.set_N_bits(Data16);                            // Set the new INT_N bits into Register 0
        // LO1->set_N_bits(Data16);                           // Set the new INT_N bits into Register 0
        regWord = SA.LO1.Execute(adf4356CmdMap[Command], 0);  // This selects from 1 of 7 adf4356 commands
        SA.LO1.update(regWord, spi_select);                   // Write Reg[4] for set_TRI/set_DLD, ELSE Reg[6]
        SA.LO1.update(SA.LO1.Curr.Reg[0], spi_select);        // followed by Reg[0] (REQUIRED by specsheet)
        break;
      case SA.LO2_addr:
        SA.LO = SA.ptrLO2;
        // LO = SA.ptrLO2;
        spi_select = SA.LO2_SEL;
        adc_pin = SA.ADC_SEL_315;
        regWord = SA.LO->Execute(max2871CmdMap[Command], serialWord);
        SA.LO->update(regWord, spi_select);  // Update LO2 registers
        break;
      case SA.LO3_addr:
        SA.LO = SA.ptrLO3;
        spi_select = SA.LO3_SEL;
        adc_pin = SA.ADC_SEL_045;
        regWord = SA.LO->Execute(max2871CmdMap[Command], serialWord);
        SA.LO->update(regWord, spi_select);  // Update LO3 registers
        break;
      case SA.RefClock:
        SA.clkExecute(Command);
        break;
      case SA.MISC_addr:
        SA.miscExecute(arduinoCmdMap[Command]);
        break;
      default:
        Serial.print(F("Requested Address:"));
        Serial.print(Address);
        Serial.println(F(" not found"));
    }   /* End switch(Address) */

  }   /* End While serial available */
  COMMAND_FLAG = false;
} /* End loop() */


/* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent.  Why?
    Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's specsheet
*/
void init_specann() {
  SA.LO3.begin(SA.LO3_SEL);
  SA.LO2.begin(SA.LO2_SEL);
  SA.LO1.begin(SA.LO1_SEL);
  delay(20);
  SA.LO3.begin(SA.LO3_SEL);
  SA.LO2.begin(SA.LO2_SEL);
  SA.LO1.begin(SA.LO1_SEL);
}
