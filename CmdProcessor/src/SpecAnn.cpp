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


// dummyLevel and dummyPin are placeholders so we can
// create a single function-pointer array
void builtinLEDOn(uint8_t level, uint8_t selectPin) {
    digitalWrite(LED_BUILTIN, HIGH);
}

void builtinLEDOff(uint8_t level, uint8_t selectPin) {
    digitalWrite(selectPin, level);
}

void version(uint8_t dummyLevel, uint8_t dummyPin) {
  Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
}

// Send the end-of-sweep acknowledgement back to the controller
void end_sweep_ack(uint8_t dummyLevel, uint8_t dummyPin) {
  Serial.write(0xFF);
  Serial.write(0xFF);
}

void miscExecute(uint8_t commandIndex, uint8_t level, const uint8_t selectPin) {
  if (commandIndex >= 0 && commandIndex < NUM_FUNCTIONS) {
    miscCmds[commandIndex](level, selectPin);
  }
}

// Create the function pointer array
void (*miscCmds[NUM_FUNCTIONS])(uint8_t, uint8_t) = {
  builtinLEDOff, // 0
  builtinLEDOn,  // 1
  version,       // 2
  end_sweep_ack, // 3
  spiWriteAtten, // 4
};

/* VSCode does AutoSave. If by chance you accidentally hit your keyboard
 * and your cursor is parked inside your code then it will get auto-saved.
 * 
 * Save yourself the trouble and always park your cursor past the last
 * line of your code.
 */


