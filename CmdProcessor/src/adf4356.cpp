
#include "adf4356.h"
#include "synthesizer.h"

void ADF4356_LO::begin(uint8_t selectPin) {
  // Program all 13 registers of the chip
   update(Curr.Reg[4], selectPin);    // Enable LO1 lock detect
   for (int x = 13; x >= 0; x--) {
     update(Curr.Reg[x], selectPin);  // Program LO1=3776.52 MHz with LD on Mux
   }
   update(Curr.Reg[14], selectPin);   // Set mux tri-state to disable LO1 lock detect
}

void ADF4356_LO::set_N_bits(uint16_t Data16)
{
  uint32_t Data32 = ((uint32_t)Data16 << 4);  // Aligns INT_N bits <N16:N1> with R[0]<DB19:DB4>
  Curr.Reg[0] = Curr.Reg[0] & INT_N_Mask;     // Clear the INT_N bits from Register 0
  Curr.Reg[0] = Curr.Reg[0] | Data32;         // Set the new INT_N bits into Register 0
}

uint32_t ADF4356_LO::set_DIV_MODE(uint32_t reg) {
  Curr.Reg[4] = Curr.Reg[4] & RFOUT_DIV_MASK;
  Curr.Reg[4] = (Curr.Reg[4] | (reg & ~RFOUT_DIV_MASK));
  return Curr.Reg[4];
}

uint32_t ADF4356_LO::turn_off_RF() {
  Curr.Reg[6] = Curr.Reg[6] & RFpower_off;
  return Curr.Reg[6];
}

uint32_t ADF4356_LO::set_n4dBm() {
  Curr.Reg[6] = (Curr.Reg[6] & Power_Level_Mask) | neg4dBm;
  return Curr.Reg[6];
}

uint32_t ADF4356_LO::set_n1dBm() {
  Curr.Reg[6] = (Curr.Reg[6] & Power_Level_Mask) | neg1dBm;
  return Curr.Reg[6];
}

uint32_t ADF4356_LO::set_p2dBm() {
  Curr.Reg[6] = (Curr.Reg[6] & Power_Level_Mask) | pos2dBm;
  return Curr.Reg[6];
}

uint32_t ADF4356_LO::set_p5dBm() {
  Curr.Reg[6] = (Curr.Reg[6] & Power_Level_Mask) | pos5dBm;
  return Curr.Reg[6];
}

uint32_t ADF4356_LO::set_TRI() {
  Curr.Reg[4] = (Curr.Reg[4] & Mux_Set_TRI);  // Turns on Tristate
  return Curr.Reg[4];
}

uint32_t ADF4356_LO::set_DLD() {
  Curr.Reg[4] = Curr.Reg[4] | Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
  return Curr.Reg[4];
}

uint32_t ADF4356_LO::Execute(byte commandIndex, uint32_t reg) {
  return ADF4356Execute(commandIndex);
}

// Program a single register of the ADF4356 by sending and latching 4 bytes
void ADF4356_LO::update(uint32_t reg, uint8_t selectPin) {
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

uint32_t ADF4356_LO::ADF4356Execute(byte commandIndex) {
  if (commandIndex < ADF4356_LO::NUMBER_OF_FUNCTIONS) {
    return (this->*adfCmds[commandIndex])();
  }
  return 0xFFFF;    // You tried to use an undefined command
}

