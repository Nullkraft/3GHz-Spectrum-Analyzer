//************************************************
//  Name    : CmdProcessor_ASCII_2.ino
//  Author  : WN2A
//  Date    : 1 January 2021
//  Version : 1.0
//  Use with FreeBasic Program FB_CmdGen_ASCII_2.bas
//************************************************

#include <SPI.h>
#include <avr/interrupt.h>

unsigned long z;
unsigned long z1; //incoming unsigned word
unsigned int s1;   //outgoing unsigned int
//Signal to Pin assignments
int LD3 = A4;       // Load LO3 of MAX2871
int LD2 = 3;       // Load LO2 of MAX2871
int REF_EN1 = 8;
int REF_EN2 = 9;
int LD1     = A3;  // Load LO1 of ADF4356
int PLL_MUX = A2;
int LD_ATT  = A5;
int ctr     =  0;
int Mux;
int pauseMillis = 30; //delay
int logAmp45;
int logAmp315;
int ncount = 0;
bool printMux = true;
int dataPin = 11;
int clockPin = 13;
int LD_PIN;
int x = 0;
unsigned int numLO;
unsigned int numFreqs;
unsigned long MOD;
unsigned long Reg0;
unsigned long Reg1;

#include "CmdProcessor_ASCII_InitRegs_2.h"
#include "CmdProcessor_ASCII_Shift_2.h"

char* nameLO = "aaa";



void setup() {
  // MUX pin interrupt
  PCICR |= 0b00000010;    // turn on port C pin-change interrupt(s)
  PCMSK1 |= 0b00000100;   // PCINT10

  Serial.begin(1000000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(REF_EN1, OUTPUT);
  pinMode(REF_EN2, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(LD1, OUTPUT);
  pinMode(LD2, OUTPUT);
  pinMode(LD3, OUTPUT);
  pinMode(PLL_MUX, INPUT);
  pinMode(LD_ATT, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(REF_EN1, HIGH); //Enable 60MHz
  digitalWrite(REF_EN2, LOW); //Disable 100MHz
#include "CmdProcessor_ASCII_InitProg_2.h"
}




void loop() {
TopLoop:
  if (Serial.available() > 0)
  {
    z1 = Serial.parseInt();
    if (z1 == 0 ) {
      goto TopLoop;
    }

    // Serial.println (z1,HEX);
  }
  // Routine to deal with LO2 and LO3 Data Words
  if (((z1 & 0x000000FF) != (0x000000FF)) && (z1 > 0 )) {
    Reg1 = ((z1 & 0x000FFF00) >> 5) + (0x20008001); //Mask & Shift M plus settings
    Reg0 = ((z1 & 0xFFF00000) >> 17) + ((z1 & 0x00000FF) << 15)  ; //Mask & Shift for F and N
    LD_PIN = LD2; //Preset LD_PIN=LD2..unless..its LD3...
    if (numLO == 3) { //in which case, its LD3
      LD_PIN = LD3;
    }
    z = Reg1;  // Time to Shift Out to MAX2871
    ShiftLO(1); // LO2 or LO3 Reg1
    z = Reg0;
    ShiftLO(0); // LO2 or LO3 Reg1
  }

  switch (z1) {

    case (2047): //   LED OFF
      digitalWrite(13, LOW);
      break;

    case (4095): //LED ON
      digitalWrite(13, HIGH);
      break;

    case (1279): //NO REF
      digitalWrite(REF_EN1, LOW);
      digitalWrite(REF_EN2, LOW);
      break;

    case (3327): // REF 1 U500
      digitalWrite(REF_EN1, HIGH);
      digitalWrite(REF_EN2, LOW);
      break;

    case (5375): // REF 2 U501
      digitalWrite(REF_EN1, LOW);
      digitalWrite(REF_EN2, HIGH);
      break;
  }
  if ((z1 & 0x000007FF) == (0x000000FF)) { //Digital Atten
    ByteA = ((z1 & 0x007F0000) >> 16);
    ShiftAtten();  // Program Attenuator
    delay(pauseMillis);
  }
  if ((z1 & 0x0000FFFF) == (0x000011FF)) { //LO1
//    z = 0x32008984;
//    ShiftLO(x);  // Program LO1
    z = ((z1 & 0xFFFF0000) >> 12);
    z = z + pow(2, 21);; //add AC1  bit Auto-Cal
    LD_PIN = LD1;
    ShiftLO(x);  // Program LO1
    delay(pauseMillis);
    nameLO = "LO1";
  }
  if ((z1 & 0x0000FFFF) == (0x000032FF)) { //LO2 Command Setup,no power change
    numLO = 2; // Tag this as LO2, so that when a data word is found
    // the Arduino will know that is for LO2 or LO3
    numFreqs = ((z1 & 0xFFFF0000) >> 16); //Number of Freqs to follow
    // Serial.println (numLO);
    //  Serial.println (numFreqs);
    nameLO = "LO2";
  }

  if ((z1 & 0x0000FFFF) == (0x000033FF)) { //LO3 Command Setup,no power change
    numLO = 3; // Tag this as LO3, so that when a data word is found
    // the Arduino will know that is for LO2 or LO3
    numFreqs = ((z1 & 0xFFFF0000) >> 16); //Number of Freqs to follow
    //  Serial.println (numLO);
    //  Serial.println (numFreqs);
    nameLO = "LO3";
  }
}


// MUX Interrupt for tracking the LO1, LO2, and LO3 lock pins
ISR(PCINT1_vect) {
  Serial.print(nameLO);
  Serial.println(" det");
}
