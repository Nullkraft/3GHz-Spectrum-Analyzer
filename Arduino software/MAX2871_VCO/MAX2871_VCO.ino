/*  Simulate an RF noise spectrum with a signal and harmonic at bins 100 and 200.
 * This is to create a serial stream for testing the Python software that will be 
 * used for running Project #3, the spectrum analyzer.
 * 
 * To reach max serial speeds it is necessary to write more than a single byte at
 * a time.  A chunk of 32 bytes is transferred as a block.  By experimention it
 * was found that a 32 byte chunk provides the greatest improvement in the data
 * transfer rate.
 */
#include "MAX2871.h"


#define EOS             0xFF  // 0xFF is reserved for End Of Serial transmission (EOS)

// Arduino pin selections
int latchPin = A3;  // MAX2871 LE
int dataPin  = A2;  // MAX2871 DATA
int clockPin = A1;  // MAX2871 SCLK
int muxPin   = A0;  // MAX2871 MUX (Multiplexed I/O)
int RF_En    =  5;  // MAX2871 RF_EN

const unsigned int numRegisters = 6;    // Num programming registers for MAX2871
const unsigned int numBytesInReg = 4;   // 32 bits in each register

/* Why are there 2 names for accessing the same 4 bytes?
* When comparing the source code to the 'Register and Bit Descriptions' in the 
* spec-sheet it is easier to compare a 32-bit mask with the associated tables. 
* It is also easier to apply that 32-bit mask to the 32-bit register value.
* 
* The Problem: 
*   Serial.read() and shiftOut() only transfer individual bytes so I would need 
* 2 separate for-loops and would be constantly switching between the 32-bit word 
* and the 4-byte representation of that same word.
* 
* The Solution: 
*   By creating a byte-pointer to the 32-bit data I can treat the 32-bit word as 
* an array of 4-bytes.
* 
* The Drawback:
*   With two different variables accessing the same data I (always?) need to take 
* care to identify the relationship of the two variables in the source code.
*/
uint32_t spiBuff;                           // 32 bit access to buffered serial data.
byte* bytePtrSpiBuff = (byte *) &spiBuff;   // Buffered serial data to write to SPI bus.

uint32_t currentRegisters[numRegisters];    // Current copies of programmed registers.


const int szChunk = 32;
byte numChunks = 0;
byte numBlocks;
byte chunk[szChunk];

char commandChar;

unsigned int chunkIndex = 0;  // Track which chunk is being used
byte frequencyInHz[3];


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(RF_En, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(RF_En, HIGH);      // Turn on the MAX2871 RF Output
  digitalWrite(latchPin, LOW);    // Latch must start LOW

  Serial.begin(2000000);
  Serial.setTimeout(5);
  randomSeed(analogRead(A0));
}



void loop() {
  int addr;
  if (Serial.available() > 0) {
    commandChar = Serial.read();
    switch(commandChar) {
      case '1': case '2': case '3': case '4': case '5':  case '6':
        numBlocks = commandChar - '0';       // Same as: atoi(commandChar);
        numChunks = numBlocks * 512/szChunk;
        chunkIndex = numChunks;
        break;
      case 'r':    // Program MAX2871 to a new output frequency
        Serial.readBytes(bytePtrSpiBuff, numBytesInReg); // Get and buffer 4 bytes from the PC.
        spiWrite(bytePtrSpiBuff, numBytesInReg);       // Write 4 bytes to a register in the MAX2871.
        addr = spiBuff & 0x7;              // Return the register address from the 32 bit word.
        currentRegisters[addr] = spiBuff;  // Always remember the state of the chip's registers.
        break;
      case 'l':                   // Turn off built-in LED
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case 'L':                   // Turn on built-in LED
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      case 'e':                   // Disable RF output
        digitalWrite(RF_En, LOW);
        break;
      case 'E':                   // Enable RF output
        digitalWrite(RF_En, HIGH);
        break;
      case 'F':
        // Get target frequency in Hz because it makes conversion to binary much easier.
        // IMPORTANT: 6-decimal places or get rounding errors.
        Serial.readBytes(frequencyInHz, 3);
        frequencyToRegisterValues(frequencyInHz);
        break;
      case 'S':
        hwState();
        muxPinMode(MUX_READ);
        break;
      case 'z':
        byte reg6cmd[] = {0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xE};
        spiWrite(reg6cmd, 4);       // Write 4 bytes to register 6. Now we can read reg6 back.
        byte reg6data[4];
        spiRead(reg6data);
        if(Serial.availableForWrite() > 0) {
          Serial.write(reg6data, 4);
        }
        break;
      default:
        break;
    }
  }

  /* chunkIndex gets the same index as the active chunk. As long
   * as it stays above 0 the next chunk will be created and sent.
   */
  if (chunkIndex) {
    simRF();          // Create some simulated RF noise and signals
    hostWrite();      // Send the simulated data to the PC for plotting
  }
}


/* Bits MUX[3:0] set the MUX pin.
 *  MUX[3] is bit 18 in reg5 and is named MUX MSB, 
 *  while MUX[2:0] are bits 28:26 in reg2 and are
 *  named MUX Configuration.
 *  
 *  From reg2 settings:
 *  Sets MUX pin mode (MSB bit located register 05).
 *  0000 = Three-state output
 *  0001 = D_VDD
 *  0010 = D_GND
 *  0011 = R-divider output    
 *  0100 = N-divider output/2    
 *  0101 = Analog lock detect    
 *  0110 = Digital lock detect    
 *  0111 = Sync Input    
 *  1000 to 1011 = Reserved    
 *  1100 = Read SPI registers 06
 *  1101 to 1111= Reserved
 *  
 *  To be able to read register 6 (aka. mode 1100):
 *  1) Set the MUX MODE by setting reg5 mux msb and reg2 MUX Configuration to 1100. 
 *  2) spiWrite(bytePtrSpiBuffReg5, numBytesInReg);   // Reg 5 takes 32 clocks
 *  3) spiWrite(bytePtrSpiBuffReg2, numBytesInReg);   // Reg 2 takes 32 clocks
 *  4) Set LE low
 *  5) spiWrite(0x00_00_00_06, numBytesInReg); // Reg 6 takes 32 clocks
 *  6) Set LE High  // Reg 6 gets programmed
      digitalWrite(latchPin, HIGH);
    
      for (int j=0; j<numBytesToWrite; j++) {
        inputRegister[j] = shiftIn(dataPin, clockPin, MSBFIRST);  // clock must be low b4 first call
      }
      digitalWrite(latchPin, LOW);
 */
void muxPinMode(int mode) {
  
  byte reg5[numBytesInReg];
  byte reg2[numBytesInReg];


  pinMode(muxPin, OUTPUT);  // Normally an output...
  if (mode == MUX_SYNC) {   // except in sync mode
    pinMode(muxPin, INPUT);
  }

  bitWrite(reg5[2], 2, bitRead(mode, 3));   // MUX MODE bit 3 is in byte2 of register 5
  spiWrite(reg5, numBytesInReg);

  bitWrite(reg2[3], 4, bitRead(mode, 2));   // MUX MODE bit 2 is in byte3 of register 2
  bitWrite(reg2[3], 3, bitRead(mode, 1));   // MUX MODE bit 1 is in byte3 of register 2
  bitWrite(reg2[3], 2, bitRead(mode, 0));   // MUX MODE bit 0 is in byte3 of register 2
  spiWrite(reg2, numBytesInReg);

  Serial.print("reg5 = ");
  Serial.print  (reg5[3], BIN);
  Serial.print  (reg5[2], BIN);
  Serial.print  (reg5[1], BIN);
  Serial.println(reg5[0], BIN);
  for (int i=0; i<numBytesInReg; i++) {
    bytePtrSpiBuff[i] = reg5[i];
  }
  Serial.print("bytePtrSpiBuff = ");
  Serial.print(bytePtrSpiBuff[2], BIN);
  Serial.print(bytePtrSpiBuff[1], BIN);
  Serial.println(bytePtrSpiBuff[0], BIN);
}


/* Time to program the MAX2871 chip to make it do what you want.
 *  This is basically a software SPI to the max chip.
*/
void spiWrite(byte *selectedRegister, unsigned int numBytesToWrite) {
  digitalWrite(latchPin, LOW);

  // Write buffered serial data to SPI bus (MAX2871 registers)
  for (int j=0; j<numBytesToWrite; j++) {
    shiftOut(dataPin, clockPin, MSBFIRST, selectedRegister[j]);
  }
  digitalWrite(latchPin, HIGH);
}


void spiRead(byte* reg6byte) {
  byte reg6cmd[] = {0xFF, 0xFF, 0xFF, 0xFE};  // register 6 'read me' command
  spiWrite(reg6cmd, 4);
  // Now one extra clock as per spec-sheet
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
  for (int i=0; i<4; i++) {
    reg6byte[i] = shiftIn(muxPin, clockPin, MSBFIRST);
  }
}



/* Given a frequency generate the register values for the MAX2871
 * based on the refClock, lockDetect and fracOpt settings.
 */
void frequencyToRegisterValues(int frequencyInHz) {
  double target_frequency = pow(frequencyInHz, 6);
  Serial.println(target_frequency, 6);
}


/* You must fill the chunk array with data before calling this function.
 *
 * Send the End-Of-Stream marker 0xFF/0xFF after all the other chunks. It's
 * a double byte of 0xFF because the A/D uses less than 16 bits. That means
 * the full-scale value of the A/D (0xFF/0xC0) is less than the EOS marker.
 */
void hostWrite() {
  if(Serial.availableForWrite() > 0) {
    Serial.write(chunk, szChunk);
  }
  if (chunkIndex == 0) {
    Serial.write(EOS);
    Serial.write(EOS);
  }
}



/*
 * Simulate RF test data for testing the Spectrum Analyzer plotting program 
 * written in python.
 */
void simRF() {
  unsigned int rfNoise;
  byte* noiseByte = (byte *) &rfNoise;

  unsigned int i = numChunks - chunkIndex;
  unsigned int index;
  unsigned int spur = (numChunks * szChunk) / 2;

  chunkIndex--;
  
  // Fill a buffer with szChunk number of bytes.
  for(int j=0; j<szChunk; j+=2) {
    rfNoise = random(15000, 25000);     // Return values that sort of look like a noise-floor
    
    index = (i*szChunk+j)>>1;           // Accessing every 2-bytes as a single 16-bits

    if(index == random(spur)) {         // Create a random spur
      rfNoise = random(2500, 10000);    // Randomly select an amplitude for this spur
    }

    if(index==100) { rfNoise = random(150, 250); }    // Create the fundamental
    if(index==200) { rfNoise = random(1250, 1750); }  // Create the harmonic

    chunk[j] = noiseByte[0];      // Buffer HIGH Byte for rfNoise
    chunk[j+1] = noiseByte[1];    // Buffer LOW Byte for rfNoise
  }
}


void hwState() {
  // Report back the state of RF_En.
  if(Serial.availableForWrite() > 0) {
    byte enable = digitalRead(RF_En);
    int mux_value = analogRead(muxPin);
    Serial.write(enable);
  }
}
