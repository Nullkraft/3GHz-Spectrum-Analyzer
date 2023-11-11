#include "max2871.h"

/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void MAX2871_LO::begin(uint8_t selectPin) {
  update(Curr.Reg[5], selectPin);   // First we program Register 5
  if (first_init) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
    first_init = false; // Next time delay() won't be called
  }
  for (int x = 4; x >= 0; x--) {
    update(Curr.Reg[x], selectPin); // and Lock Detect is enabled on the Mux pin
  }
  update(Curr.Reg[6], selectPin);   // Set mux tri-state to disable LO1 lock detect
}

void MAX2871_LO::set_NF_bits(uint32_t reg) {
  // N and F:  Clear Reg[0], bits [22:3], before accepting new N and F words
  Curr.Reg[0] = Curr.Reg[0] & NF_clr;
  // N:  Mask and set bits [22:15] to program the new value for N
  Curr.Reg[0] = Curr.Reg[0] | ((reg << 15) & N_set);
  // F:  Mask and set bits [14:3] to program the new value for F
  Curr.Reg[0] = Curr.Reg[0] | ((reg >> 17) & F_set);
}

void MAX2871_LO::set_M_bits(uint32_t reg) {
  // M:  Clear Reg[1], bits [14:3], before accepting a new M word
  Curr.Reg[1] = Curr.Reg[1] & M_clr;
  // M:  Mask and set bits[14:3] to program the new value for M
  Curr.Reg[1] = Curr.Reg[1] | ((reg >> 5) & M_set);
}

uint32_t MAX2871_LO::set_DIV_MODE(uint32_t reg) {
  Curr.Reg[4] = Curr.Reg[4] & RFOUT_DIV_MASK;
  // Curr.Reg[4] &= RFOUT_DIV_MASK;
  Curr.Reg[4] = (Curr.Reg[4] | (reg & !RFOUT_DIV_MASK));
  // Curr.Reg[4] |= (controlWord & !RFOUT_DIV_MASK);
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::turn_off_RF(uint32_t dummyReg) {
  Curr.Reg[4] = Curr.Reg[4] & RFpower_off;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_n4dBm(uint32_t dummyReg) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | neg4dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_n1dBm(uint32_t dummyReg) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | neg1dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p2dBm(uint32_t dummyReg) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos2dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p5dBm(uint32_t dummyReg) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos5dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_TRI(uint32_t dummyReg) {
  Curr.Reg[2] = Curr.Reg[2] & Mux_Set_TRI;  // Set MuxOut to Tristate
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::set_DLD(uint32_t dummyReg) {
  Curr.Reg[2] = Curr.Reg[2] | Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::Execute(byte commandIndex, uint32_t controlWord) {
  if (commandIndex >= 0 && commandIndex < MAX2871_LO::NUMBER_OF_FUNCTIONS) {
    return (this->*maxCmds[commandIndex])(controlWord);
  }
  return 0xFFFF;    // You tried to use an undefined command
}

// Program a single register of the selected LO by sending and latching 4 bytes
void MAX2871_LO::update(uint32_t reg, uint8_t selectPin) {
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
