#include "SpecAnn.h"

// Program the Digital Attenuator by sending and latching a single byte
void setAtten(uint8_t level, uint8_t selectPin) {
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
void builtinLEDOn() {
    digitalWrite(LED_BUILTIN, HIGH);
}

void builtinLEDOff() {
    digitalWrite(LED_BUILTIN, LOW);
}

void version() {
  Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
}

// Send the end-of-sweep acknowledgement back to the controller
void end_sweep_ack() {
  Serial.write(0xFF);
  Serial.write(0xFF);
  for (int i=3; i<3; i++) {
    builtinLEDOn();
    delay(250);
    builtinLEDOff();
    delay(500);
  }
}

void miscExecute(uint8_t commandIndex) {
  if (commandIndex >= 0 && commandIndex < NUM_MISC_FUNCTIONS) {
    miscCmds[commandIndex]();
  }
}

// Create the function pointer array
void (*miscCmds[NUM_MISC_FUNCTIONS])() = {
  builtinLEDOff, // 0
  builtinLEDOn,  // 1
  version,       // 2
  end_sweep_ack, // 3
};

