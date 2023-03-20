//************************************************
//  Name    : CmdProcessor_ASCII_2.ino
//  Author  : WN2A
//  Date    : 1 January 2021
//  Version : 1.0
//  Use with FreeBasic Program FB_CmdGen_ASCII_2.bas
//************************************************

#include "CmdProcessor_ASCII_InitRegs_2.h"
#include <SPI.h>
#include <avr/interrupt.h>

unsigned long z;
unsigned long z1;   // incoming unsigned word
unsigned int s1;    // outgoing unsigned int

//Signal to Pin assignments
const int LD1     = A3;   // Load LO1 ADF4356
const int LD2     =  3;   // Load LO2 MAX2871
const int LD3     = A4;   // Load LO3 MAX2871
const int REF_EN1 =  8;
const int REF_EN2 =  9;
const int PLL_MUX = A2;
const int LD_ATT  = A5;

const int pauseMillis = 20; // delay

int LD_PIN;
int Mux;
int ctr = 0;
int logAmp45;
int logAmp315;
int ncount = 0;
int x = 0;

unsigned int selectedLO;
unsigned int numFreqs;
unsigned long MOD;
unsigned long Reg0;
unsigned long Reg1;

const int LED_OFF   = 2047;
const int LED_ON    = 4095;
const int NO_REF    = 1279;
const int REF_1_060 = 3327;
const int REF_2_100 = 5375;

bool initialize;    // Used to indicate when you are initializing LO2 and LO3

char* nameLO = "aaa";


/*
 * 
 * 
 */
void setup() {
  // MUX pin interrupt
  PCICR |= 0b00000010;    // turn on port C pin-change interrupt(s)
  PCMSK1 |= 0b00000100;   // PCINT10

  Serial.begin(1000000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(REF_EN1, OUTPUT);
  pinMode(REF_EN2, OUTPUT);
  pinMode(LD1, OUTPUT);
  pinMode(LD2, OUTPUT);
  pinMode(LD3, OUTPUT);
  pinMode(PLL_MUX, INPUT);
  pinMode(LD_ATT, OUTPUT);

  digitalWrite(REF_EN1, HIGH); // Enable 60MHz
  digitalWrite(REF_EN2, LOW);  // Disable 100MHz

//  ShiftAtten(Atten_level, LD_ATT);  // Program Attenuator
//  delay(pauseMillis);

  /* Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's documentation */
  nameLO = "LO1";
  loadLO1(LD1);
  nameLO = "LO2";
  loadLO2(LD2, true);
  nameLO = "LO3";
  loadLO3(LD3, true);
  delay(pauseMillis);
  nameLO = "LO1";
  loadLO1(LD1);
  nameLO = "LO2";
  loadLO2(LD2, false);
  nameLO = "LO3";
  loadLO3(LD3, false);
}


void loop() {
TopLoop:
  if (Serial.available() > 0)
  {
    z1 = Serial.parseInt();
    if (z1 == 0 ) {
      goto TopLoop;
    }
    Serial.println(z1, HEX);
  }

  switch (z1)  {
    case (LED_OFF):
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case (LED_ON):
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case (NO_REF):
      digitalWrite(REF_EN1, LOW);
      digitalWrite(REF_EN2, LOW);
      break;
    case (REF_1_060):
      digitalWrite(REF_EN1, HIGH);
      digitalWrite(REF_EN2, LOW);
      break;
    case (REF_2_100):
      digitalWrite(REF_EN1, LOW);
      digitalWrite(REF_EN2, HIGH);
      break;
  }

  // Routine to parse LO2 and LO3 Data Words
  if (((z1 & 0x000000FF) != 0x000000FF) && (z1 > 0)) {
    Reg1 = ((z1 & 0x000FFF00) >> 5) + 0x20008001;                 // Mask & Shift M plus settings
    Reg0 = ((z1 & 0xFFF00000) >> 17) + ((z1 & 0x00000FF) << 15);  // Mask & Shift for F and N
    LD_PIN = LD2;       // Preset LD_PIN=LD2..
    if (selectedLO == 3) {   // unless..its LD3...
      LD_PIN = LD3;
    }
    z = Reg1;   // Time to Shift Out to MAX2871
    ShiftLO(z, LD_PIN); // LO2 or LO3 Reg1
    z = Reg0;
    ShiftLO(z, LD_PIN); // LO2 or LO3 Reg0
  }

  // Digital Attenuator Command
  if ((z1 & 0x000007FF) == (0x000000FF)) {
    Serial.println("Called attenuator");
    Atten_level = ((z1 & 0x007F0000) >> 16);      // End user provided a new attenuation level
    ShiftAtten(Atten_level, LD_ATT);              // Program Attenuator
    delay(pauseMillis);
  }

  // LO1 Command with power change to -4 dBm
  if ((z1 & 0x0000FFFF) == (0x000011FF)) {
    z = ((z1 & 0xFFFF0000) >> 12);
    z = z + pow(2, 21);;  // add AC1 bit Auto-Cal
    nameLO = "LO1";
    ShiftLO(z, LD1);    // Program LO1
    delay(pauseMillis);
  }

  // LO2 Command Setup, no power change
  if ((z1 & 0x0000FFFF) == (0x000032FF)) {
    selectedLO = 2;
    numFreqs = ((z1 & 0xFFFF0000) >> 16); // Number of Freq words to follow
    nameLO = "LO2";
  }

  // LO3 Command Setup, no power change
  if ((z1 & 0x0000FFFF) == (0x000033FF)) {
    selectedLO = 3;
    numFreqs = ((z1 & 0xFFFF0000) >> 16); // Number of Freq words to follow
    nameLO = "LO3";
  }
}

// MUX Interrupt for tracking the LO1, LO2, and LO3 lock pins
ISR(PCINT1_vect) {
  Serial.print(nameLO);
  Serial.println(" det");
}


void loadLO1(uint8_t selectPin) {
  for (int x = 13; x >= 0; x--) {
    z = RegLO1[x];
    ShiftLO(z, selectPin);          // Program LO1=37Byte33[x]76.52 MHz with LD on Mux
  }

  delay(2);                         // Short delay before reading Register 6
  MuxTest("LO1");                   // Now read the Mux Pin
  ShiftLO(RegLO1[14], selectPin);   // Tri-stating the mux output disables LO1 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void loadLO2(uint8_t selectPin, bool initialize) {
  ShiftLO(RegLO2[5], selectPin);    // First we program LO2 Register 5
  if (initialize) { delay(20); }            // And only if it's our first time must we wait 20 mSec
  for (int x = 4; x >= 0; x--) {
    z = RegLO2[x];                  // Program remaining registers where LO2=3915 MHz
    ShiftLO(z, selectPin);          // and Lock Detect is enabled on the Mux pin
  }
  delay(2);                         // Short delay before reading Register 6
  MuxTest("LO2");                   // Check if LO2 is locked by reading the Mux pin
  ShiftLO(RegLO2[6], selectPin);    // Tri-stating the mux output disables LO2 lock detect
}


/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void loadLO3(uint8_t selectPin, bool initialize) {
  ShiftLO(RegLO3[5], selectPin);    // First we program LO3 Register 5
  if (initialize) {delay(20); }
  for (int x = 4; x >= 0; x--) {
    z = RegLO3[x];                  // Program remaining registers where LO3=270 MHz
    ShiftLO(RegLO3[x], selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  delay(2);                         // Short delay before reading Register 6
  MuxTest("LO3");                   // Check if LO3 is locked by reading the Mux pin
  ShiftLO(RegLO3[6], selectPin);    // Tri-stating the mux output disables LO3 lock detect
}


// Program the Digital Attenuator by sending and latching a single byte
void ShiftAtten(uint8_t level, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(selectPin, LOW);
  SPI.transfer(level);
  digitalWrite(selectPin, HIGH);
  SPI.end();
}


// Program a single register of the selected LO by sending and latching 4 bytes
void ShiftLO(uint32_t reg, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(selectPin, LOW);
  SPI.transfer(z >> 24);
  SPI.transfer(z >> 16);
  SPI.transfer(z >> 8);
  SPI.transfer(z);
  digitalWrite(selectPin, HIGH);
  SPI.end();
}


// When the mux pin is configured for Digital Lock Detect output
// we can read the status of the pin from here.
void MuxTest(String chipName) {
  if (digitalRead(PLL_MUX)) {
    Serial.print(chipName);
    Serial.println(" LOCK   ");
  }
  else {
    Serial.print(chipName);
    Serial.println(" UNLOCK   ");
  }
}
