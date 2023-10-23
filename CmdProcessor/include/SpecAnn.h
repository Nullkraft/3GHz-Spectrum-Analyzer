#ifndef _SPECANN_H_
#define _SPECANN_H_

#include "Arduino.h"
#include "max2871.h"
#include "adf4356.h"

/* Vref = 2.595  Volts/ADC_bit == 0.002536 Volts
 * Noise-floor ≈ 0.488 Volts
 * ADC_value of 193 ≈ 0.488/0.002536
*/
// bool LAST_STATE;
// bool ABOVE_NOISE_FLOOR;
// bool BELOW_NOISE_FLOOR;

// Status codes: Uses the 4 msbits of the 16 bit ADC return values
// const int start_noise_floor = 0xD0;  // First in the series of ADC noise-floor readings
// const int end_noise_floor   = 0xE0;  // Last in the series of ADC noise-floor readings
const int failed_to_lock = 0xF0;  // VCO Failed to lock

// Command names for operating LO1, LO2 and LO3
enum class devices {
    // General,
    // RF_off,
    // neg_4dBm,
    // neg_1dBm, 
    // pos_2dBm, 
    // pos_5dBm, 
    // No_change, 
    // Mux_TRI, 
    // Mux_DLD,
    DIV_MODE = 9,
    COUNT       // Sentinel value must always be at the end
};

// Command names for operating the reference oscillators
enum LOCmds { all_ref_off, 
              ref_LO, 
              ref_HI, };

// Command names for operating the PLL Mux
enum mux { all_tristate, };

// Command names for controlling and reporting Arduino state
enum misc { LED_off, 
            LED_on, 
            MSG_REQ, 
            SWEEP_START, 
            SWEEP_END, 
            RESET, };

/*********** ARDUINO PIN DEFINITIONS ***********/
const uint8_t LO1_SEL = A3;
const uint8_t LO2_SEL = 3;
const uint8_t LO3_SEL = A4;
const uint8_t REF_LO_SEL = 8;
const uint8_t REF_HI_SEL = 9;
const uint8_t ADC_SEL_045 = A0;  // ADC for LO2
const uint8_t ADC_SEL_315 = A1;  // ADC for LO3
const uint8_t PLL_MUX = A2;      // Equals physical pin 16 on Port C (use PCMSK1)
const uint8_t ATTEN_SEL = A5;
//const uint8_t SPI_MOSI  = 11;   // Reserved by the SPI Library
//const uint8_t SPI_MISO  = 12;   // Reserved by the SPI Library
//const uint8_t SPI_CLOCK = 13;   // Reserved by the SPI Library
uint8_t adc_pin = ADC_SEL_315;  // Default sets this to either to the ADC for LO2 output

// Addresses for selecting the various hardware ICs
// TODO: Create the list in a config file that remains with the project files.
const uint8_t Attenuator = 0;
const uint8_t LO1_addr = 1;
const uint8_t LO2_addr = 2;
const uint8_t LO3_addr = 3;
const uint8_t RefClock = 4;
const uint8_t MISC_addr = 7;

// BitMask for programming the registers of the Attenuator IC
const uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data

/*********** HARDWARE DEFINITIONS END *******/

// Assign reference designators from the schematic to the LO ojbect of choice
ADF4356_LO LO1 = ADF4356_LO();
MAX2871_LO LO2 = MAX2871_LO();
MAX2871_LO LO3 = MAX2871_LO();
MAX2871_LO* LO;  // Allows a single function to select and operate on LO2 or LO3

volatile uint16_t a2dAmplitude;
uint8_t* ampl_byte = (uint8_t*)&a2dAmplitude;

void spiWriteAtten(uint8_t level, uint8_t selectPin);
void init_specann();
void version();

#endif


/* Starting with one of the MAX2871 chips makes initializing LO1 much more consistent.  Why?
    TODO: Investigate LO1 locking anomaly
    Initialize IC's LO1, LO2 and LO3 by programming them twice IAW manufacturer's specsheet
*/
void init_specann() {
  // Presets for LO3
  LO3.Curr.Reg[0] = 0x002081C8;  // LO3 = 270 MHz with 66 MHz ref clock
  LO3.Curr.Reg[1] = 0x400103E9;
  LO3.Curr.Reg[2] = 0x98005F42;  // Digital Lock Detect ON
  LO3.Curr.Reg[3] = 0x00001F23;
  LO3.Curr.Reg[4] = 0x63CE803C;
  LO3.Curr.Reg[5] = 0x00400005;
  LO3.Curr.Reg[6] = 0x80005F42;  // Digital Lock Detect ON

  LO3.begin(LO3_SEL, true);
  LO2.begin(LO2_SEL, true);
  LO1.begin(LO1_SEL);
  delay(20);
  LO3.begin(LO3_SEL, false);
  LO2.begin(LO2_SEL, false);
  LO1.begin(LO1_SEL);
}

// Program the Digital Attenuator by sending and latching a single byte
void spiWriteAtten(uint8_t level, uint8_t selectPin) {
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  SPI.transfer(level);
  digitalWrite(selectPin, HIGH);
  digitalWrite(selectPin, LOW);
  SPI.end();
}

void version() {
  Serial.print(F("- WN2A Spectrum Analyzer CmdProcessor Oct. 2023"));
}
