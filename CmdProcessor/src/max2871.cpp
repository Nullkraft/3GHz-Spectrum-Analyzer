#include "max2871.h"

/* IAW Manufacturer's PDF document "MAX2871 - 23.5MHz to 6000MHz Fractional/Integer-N Synthesizer/VCO"
   pg. 13 4-Wire Serial Interface during initialization there should be a 20mS delay after programming
   register 5.                                                  Document Version: 19-7106; Rev 4; 6/20
*/
void MAX2871_LO::begin(uint8_t selectPin) {
  update(Curr.Reg[5], selectPin);   // First we program Register 5
  if (first_init) {
    delay(20);  // Only if it's our first time must we wait 20 mSec
    first_init = false; // Ensure delay(20) is only called once
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
  Curr.Reg[4] = (Curr.Reg[4] | (reg & !RFOUT_DIV_MASK));
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::turn_off_RF(uint32_t dummy) {
  Curr.Reg[4] = Curr.Reg[4] & RFpower_off;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_n4dBm(uint32_t) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | neg4dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_n1dBm(uint32_t) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | neg1dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p2dBm(uint32_t) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos2dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_p5dBm(uint32_t) {
  Curr.Reg[4] = (Curr.Reg[4] & Power_Level_Mask) | pos5dBm;
  return Curr.Reg[4];
}

uint32_t MAX2871_LO::set_TRI(uint32_t) {
  Curr.Reg[2] = Curr.Reg[2] & Mux_Set_TRI;  // Set MuxOut to Tristate
  return Curr.Reg[2];
}

uint32_t MAX2871_LO::set_DLD(uint32_t) {
  Curr.Reg[2] = Curr.Reg[2] | Mux_Set_DLD;  // Set MuxOut to Dig. Lock Det.
  return Curr.Reg[2];
}

// uint32_t MAX2871_LO::Execute(byte commandIndex, uint32_t controlWord) {
uint32_t MAX2871_LO::Execute(byte commandIndex, uint32_t controlWord) {
  if (commandIndex >= 0 && commandIndex < MAX2871_LO::NUMBER_OF_FUNCTIONS) {
    return (this->*maxCmds[commandIndex])(controlWord);
  }
  return 0xFFFF;    // You tried to use an undefined command
}

void MAX2871_LO::setFrequency(float frequency, float ref_clock, uint8_t selectPin) {
    uint16_t F;
    uint16_t M;
    uint8_t N;

    FMN_from_freq(frequency, ref_clock, F, M, N);
    setFrequency(F, M, N, selectPin);
}

void MAX2871_LO::setFrequency(uint16_t F, uint16_t M, uint8_t N, uint8_t selectPin) {
    // Masking and shifting F and N for R[0]
    uint32_t F_masked_shifted = (uint32_t)(F & 0xFFF) << 3;  // Mask and shift F
    uint32_t N_masked_shifted = (uint32_t)(N & 0xFF) << 15;  // Mask and shift N

    // Masking and shifting M for R[1]
    uint32_t M_masked_shifted = (uint32_t)(M & 0xFFF) << 3;  // Mask and shift M

    // Clearing the bits where F, M, and N will be placed, while keeping the other bits unchanged
    Curr.Reg[0] &= ~(0xFFF << 3);  // Clear bits for F
    Curr.Reg[0] &= ~(uint32_t(0xFF) << 15);  // Clear bits for N
    Curr.Reg[1] &= ~(0xFFF << 3);  // Clear bits for M

    // Setting the new values for F, M, and N
    Curr.Reg[0] |= F_masked_shifted;  // Set new value for F
    Curr.Reg[0] |= N_masked_shifted;  // Set new value for N
    Curr.Reg[1] |= M_masked_shifted;  // Set new value for M

    // Update the registers
    update(Curr.Reg[0], selectPin); // Update R0
    update(Curr.Reg[1], selectPin); // Update R1
}

void MAX2871_LO::FMN_from_freq(float target_freq_MHz, float ref_clock, uint16_t &F, uint16_t &M, uint8_t &N) {
    float R = 2;
    float Fpfd = ref_clock / R;
    float max_error = pow(2, 32);
    float Fvco = target_freq_MHz;

    while (Fvco < 3000.0) {
        Fvco *= 2;
        if (Fvco >= 3000.0) {
            break;
        }
    }

    N = static_cast<uint8_t>(Fvco / Fpfd);
    float Fract = Fvco / Fpfd - N;
    float best_F = 0;
    uint16_t best_M = 0;

    for (int M_candidate = 4095; M_candidate > 1; --M_candidate) {
        float F_candidate = Fract * M_candidate;
        float err = abs(Fvco - (Fpfd * (N + F_candidate / M_candidate)));
        if (err == 0) {
            best_F = F_candidate;
            best_M = M_candidate;
            break;
        }
        if (err <= max_error) {
            max_error = err;
            best_F = F_candidate;
            best_M = M_candidate;
        }
    }

    F = static_cast<uint16_t>(round(best_F));
    M = best_M;
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
