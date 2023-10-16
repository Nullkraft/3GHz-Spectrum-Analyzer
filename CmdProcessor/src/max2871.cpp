#include "max2871.h"


void MAX2871_LO::begin(float freq) {
  freq += 1.0;
/* Based on the settings of the default register values the LO will be intitialized with:
  - Frequency is set to 
  - The Mux is set to tristated output (Digital Lock Detect is off)
  - Clock divider mode - Fast-lock is enabled
  - Clock divider value is set to 1
  - Band select clock divider - Set to 1023
  - RFOUT output divider mode - Set to 4
  - RFOUTB is enabled and set to divided output
  - RFOUTB power is set to +5 dBm
  - RFOUTA is off
*/
}

uint32_t MAX2871_LO::unused() {
    return 0xFFFF;  // You tried to use an undefined command
}

void MAX2871_LO::clr_reg0() {
  // N and F:  Clear Reg[0], bits [22:3], before accepting new N and F words
  Curr.Reg[0] = Curr.Reg[0] & NF_clr;
}

void MAX2871_LO::set_reg0(uint32_t serialWord) {
  // N:  Mask and set bits [22:15] to program the new value for N
  Curr.Reg[0] = Curr.Reg[0] | ((serialWord << 15) & N_set);
  // F:  Mask and set bits [14:3] to program the new value for F
  Curr.Reg[0] = Curr.Reg[0] | ((serialWord >> 17) & F_set);
}

void MAX2871_LO::clr_reg1() {
  // M:  Clear Reg[1], bits [14:3], before accepting a new M word
  Curr.Reg[1] = Curr.Reg[1] & M_clr;
}

void MAX2871_LO::set_reg1(uint32_t serialWord) {
  // M:  Mask and set bits[14:3] to program the new value for M
  Curr.Reg[1] = Curr.Reg[1] | ((serialWord >> 5) & M_set);
}

uint32_t MAX2871_LO::set_DLD() {
  Curr.Reg[2] = Curr.Reg[2] | Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::set_TRI() {
  Curr.Reg[2] = Curr.Reg[2] & Mux_Set_TRI;  // Set MuxOut to Tristate
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::set_DIV_MODE(uint32_t serialWord) {
  Curr.Reg[4] &= RFOUT_DIV_MASK;
  Curr.Reg[4] |= (serialWord & !RFOUT_DIV_MASK);
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p5dBm() {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos5dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::turn_RF_off() {
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
