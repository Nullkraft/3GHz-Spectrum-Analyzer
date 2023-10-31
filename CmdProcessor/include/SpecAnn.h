#ifndef _SPECANN_
#define _SPECANN_

#include "Arduino.h"
#include <SPI.h>

/* Vref = 2.595  Volts/ADC_bit == 0.002536 Volts
 * Noise-floor ≈ 0.488 Volts
 * ADC_value of 193 ≈ 0.488/0.002536
*/

// Status codes use the 4 msbits of the 16 bit ADC return values
// const int start_noise_floor = 0xD0;  // First in the series of ADC noise-floor readings
// const int end_noise_floor   = 0xE0;  // Last in the series of ADC noise-floor readings
const int failed_to_lock = 0xF0;  // VCO Failed to lock

// BitMask for programming the registers of the Attenuator IC
const uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data

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

// Addresses for selecting the various hardware ICs
// TODO: Create the list in a config file that remains with the project files.
const uint8_t Attenuator = 0;
const uint8_t LO1_addr = 1;
const uint8_t LO2_addr = 2;
const uint8_t LO3_addr = 3;
const uint8_t RefClock = 4;
const uint8_t MISC_addr = 7;

/*********** HARDWARE DEFINITIONS END *******/
#define NUM_MISC_FUNCTIONS 4
#define NUM_CLK_FUNCTIONS 3

// Create a function pointer to use in the function pointer array
// typedef void (*CmdFunc)(uint8_t, uint8_t);
void builtinLEDOff();
void builtinLEDOn();
void version();
void end_sweep_ack();

void all_ref_off();
void ref_LO();
void ref_HI();

void setAtten(uint8_t level, uint8_t selectPin);

extern void (*miscCmds[NUM_MISC_FUNCTIONS])();
void miscExecute(uint8_t);

extern void (*refClockCmds[NUM_CLK_FUNCTIONS])();
void clkExecute(uint8_t);

#endif  // _SPECANN_
