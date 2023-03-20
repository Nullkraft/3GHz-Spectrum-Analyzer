#ifndef _SPECANN_H_
#define _SPECANN_H_

#include "Arduino.h"

void initialize_LO1(uint8_t selectPin);
void initialize_LO2(uint8_t selectPin, bool initialize);
void initialize_LO3(uint8_t selectPin, bool initialize);
void spiWriteAtten(uint8_t level, uint8_t selectPin);
void spiWriteLO(uint32_t reg, uint8_t selectPin);


#endif






