#include "max2871.h"


uint32_t MAX2871_LO::unused() {
    return 0xFFFF;  // You tried to use an undefined command
}

uint32_t MAX2871_LO::unused(uint32_t) {
    return 0xFFFF;  // You tried to use an undefined command
}

/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void MAX2871_LO::begin(uint8_t selectPin, bool initialize) {
  spiWrite(Curr.Reg[5], selectPin);  // First we program LO2 Register 5
  if (initialize) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
  }
  for (int x = 4; x >= 0; x--) {
    spiWrite(Curr.Reg[x], selectPin);  // and Lock Detect is enabled on the Mux pin
  }
  spiWrite(Curr.Reg[6], selectPin);  // Tri-stating the mux output disables LO2 lock detect
}

void MAX2871_LO::set_NF_bits(uint32_t controlWord) {
  // N and F:  Clear Reg[0], bits [22:3], before accepting new N and F words
  Curr.Reg[0] = Curr.Reg[0] & NF_clr;
  // N:  Mask and set bits [22:15] to program the new value for N
  Curr.Reg[0] = Curr.Reg[0] | ((controlWord << 15) & N_set);
  // F:  Mask and set bits [14:3] to program the new value for F
  Curr.Reg[0] = Curr.Reg[0] | ((controlWord >> 17) & F_set);
}

void MAX2871_LO::set_M_bits(uint32_t controlWord) {
  // M:  Clear Reg[1], bits [14:3], before accepting a new M word
  Curr.Reg[1] = Curr.Reg[1] & M_clr;
  // M:  Mask and set bits[14:3] to program the new value for M
  Curr.Reg[1] = Curr.Reg[1] | ((controlWord >> 5) & M_set);
}

uint32_t MAX2871_LO::set_DLD() {
  Curr.Reg[2] = Curr.Reg[2] | Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::set_TRI() {
  Curr.Reg[2] = Curr.Reg[2] & Mux_Set_TRI;  // Set MuxOut to Tristate
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::set_DIV_MODE(uint32_t controlWord) {
  Curr.Reg[4] &= RFOUT_DIV_MASK;
  Curr.Reg[4] |= (controlWord & !RFOUT_DIV_MASK);
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p5dBm() {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos5dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::turn_off_RF() {
  Curr.Reg[4] = Curr.Reg[4] & RFpower_off;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_n4dBm() {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | neg4dBm;
  return Curr.Reg[4];
}
uint32_t MAX2871_LO::set_n1dBm() {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | neg1dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p2dBm() {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos2dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::MAX2871Execute(int commandIndex) {
  if (commandIndex >= 0 && commandIndex < MAX2871_LO::NUMBER_OF_FUNCTIONS) {
    return (this->*maxCmds[commandIndex])();
  }
  return 0xFFFF;    // You tried to use an undefined command
}

// Program the register for setting div mode
uint32_t MAX2871_LO::MAX2871ExecuteWithArg(int commandIndex, uint32_t controlWord) {
  if (commandIndex >= 0 && commandIndex < MAX2871_LO::NUMBER_OF_COMMANDS) {
    return (this->*maxCmdsWithArg[commandIndex])(controlWord);
  }
  return 0xFFFF;    // You tried to use an undefined command
}

// Program a single register of the selected LO by sending and latching 4 bytes
void MAX2871_LO::spiWrite(uint32_t reg, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(spiMaxSpeed, MSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(selectPin, LOW);
  SPI.transfer(reg >> 24);
  SPI.transfer(reg >> 16);
  SPI.transfer(reg >> 8);
  SPI.transfer(reg);
  digitalWrite(selectPin, HIGH);
  SPI.end();
}
