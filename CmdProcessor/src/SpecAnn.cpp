#include "SpecAnn.h"

// Program the Digital Attenuator by sending and latching a single byte
void spiWriteAtten(uint8_t level, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  SPI.transfer(level);
  digitalWrite(selectPin, HIGH);
  digitalWrite(selectPin, LOW);
  SPI.end();
}

void builtinLEDOn() {
    digitalWrite(LED_BUILTIN, HIGH);
}

void builtinLEDOff() {
    digitalWrite(LED_BUILTIN, LOW);
}

void version() {
  Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
}

