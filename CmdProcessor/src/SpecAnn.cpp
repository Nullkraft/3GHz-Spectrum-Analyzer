#include "SpecAnn.h"

// Program the Digital Attenuator by sending and latching a single byte
void SPECANN::updateAtten(uint8_t level, uint8_t selectPin) {
  level = level & ATTEN_Data_Mask;
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  SPI.transfer(level);
  digitalWrite(selectPin, HIGH);
  digitalWrite(selectPin, LOW);
  SPI.end();
}

// dummyLevel and dummyPin are placeholders so we can
// create a single function-pointer array
void SPECANN::builtinLEDOn() {
    digitalWrite(LED_BUILTIN, HIGH);
}

void SPECANN::builtinLEDOff() {
    digitalWrite(LED_BUILTIN, LOW);
}

void SPECANN::version() {
  Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
}

// Send the end-of-sweep acknowledgement back to the controller
void SPECANN::end_sweep_ack() {
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
void SPECANN::all_ref_off() {
  digitalWrite(REF_LO_SEL, LOW);
  digitalWrite(REF_HI_SEL, LOW);
}
// Turn on low freq reference clock, Ref1, and turn off high freq reference clock, Ref2
void SPECANN::ref_LO() {
  digitalWrite(REF_HI_SEL, LOW);
  digitalWrite(REF_LO_SEL, HIGH);
}
// Turn on high freq reference clock, Ref2, and turn off low freq reference clock, Ref1
void SPECANN::ref_HI() {
  digitalWrite(REF_LO_SEL, LOW);
  digitalWrite(REF_HI_SEL, HIGH);
}

void SPECANN::clkExecute(uint8_t commandIndex) {
  if (commandIndex >= 0 && commandIndex < NUM_CLK_FUNCTIONS) {
    (this->*refClockCmds[commandIndex])();
  }
}

// Select the miscCmds function to be executed
void SPECANN::miscExecute(uint8_t commandIndex) {
  if (commandIndex >= 0 && commandIndex < NUM_MISC_FUNCTIONS) {
    // Call the function 
    (this->*miscCmds[commandIndex])();
  }
}


