#include "SpecAnn.h"

uint8_t SpecAnn::select_pin = 255;

SpecAnn::SpecAnn() {
  // lo.begin(SpecAnn::LO2_SEL);
  ref_LO();
  // Presets for LO3
  LO3.Curr.Reg[0] = 0x002081C8;  // LO3 = 270 MHz with 66 MHz ref clock
  LO3.Curr.Reg[1] = 0x400103E9;
  LO3.Curr.Reg[2] = 0x98005F42;  // Digital Lock Detect ON
  LO3.Curr.Reg[3] = 0x00001F23;
  LO3.Curr.Reg[4] = 0x63CE803C;
  LO3.Curr.Reg[5] = 0x00400005;
  LO3.Curr.Reg[6] = 0x80005F42;  // Digital Lock Detect ON
}

/* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent. Why?
   Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's specsheet
*/
void SpecAnn::init_specann() {
  LO1.begin(LO1_SEL);
  LO2.begin(LO2_SEL);
  LO3.begin(LO3_SEL);
  delay(20);
  LO1.begin(LO1_SEL);
  LO2.begin(LO2_SEL);
  LO3.begin(LO3_SEL);
}

void SpecAnn::updateLORegisters(MAX2871_LO* LoPtr, uint8_t selectPin, uint8_t command, uint32_t serialWord) {
  LO = LoPtr;
  SpecAnn::select_pin = selectPin;  // Update global static select_pin to selected chip
  uint32_t regWord = LO->Execute(max2871CmdMap[command], serialWord);
  LO->update(regWord, SpecAnn::select_pin);
}

// Program the Digital Attenuator by sending and latching a single byte
void SpecAnn::updateAtten(uint8_t level, uint8_t selectPin) {
  SpecAnn::select_pin = selectPin;  // Update global static select_pin to selected chip
  level = level & ATTEN_Data_Mask;
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  SPI.transfer(level);
  digitalWrite(SpecAnn::select_pin, HIGH);
  digitalWrite(SpecAnn::select_pin, LOW);
  SPI.end();
}

// dummyLevel and dummyPin are placeholders so we can
// create a single function-pointer array
void SpecAnn::builtinLEDOn() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print(F("Called builtin_LED_On"));
}

void SpecAnn::builtinLEDOff() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print(F("Called builtin_LED_Off"));
}

void SpecAnn::version() {
  Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
}

// Send the end-of-sweep acknowledgement back to the controller
void SpecAnn::end_sweep_ack() {
  Serial.write(0xFF);
  Serial.write(0xFF);
  for (int i=3; i<3; i++) {
    builtinLEDOn();
    delay(250);
    builtinLEDOff();
    delay(500);
  }
}

// Turn both ref_clocks off
void SpecAnn::all_ref_off() {
  digitalWrite(REF_LO_SEL, LOW);
  digitalWrite(REF_HI_SEL, LOW);
}
// Turn on low freq reference clock, Ref1, and turn off high freq reference clock, Ref2
void SpecAnn::ref_LO() {
  digitalWrite(REF_LO_SEL, HIGH);
  digitalWrite(REF_HI_SEL, LOW);
}
// Turn on high freq reference clock, Ref2, and turn off low freq reference clock, Ref1
void SpecAnn::ref_HI() {
  digitalWrite(REF_LO_SEL, LOW);
  digitalWrite(REF_HI_SEL, HIGH);
}

void SpecAnn::clkExecute(uint8_t commandIndex) {
  if (commandIndex < NUM_CLK_FUNCTIONS) {
    (this->*refClockCmds[commandIndex])();
  }
}

// Select the miscCmds function to be executed
void SpecAnn::miscExecute(uint8_t commandIndex) {
  if (commandIndex < NUM_MISC_FUNCTIONS) {
    // Call the function 
    (this->*miscCmds[commandIndex])();
  }
}


