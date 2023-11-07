#ifndef _SYNTHESIZER_H_
#define _SYNTHESIZER_H_

#include <Arduino.h>

class LO {
  public:
    // virtual void begin(uint32_t reg, uint8_t selectPin) = 0;
    // virtual void update(uint32_t reg, uint8_t selectPin) = 0;
    // Default value for controlWord to account for ADF4356
    virtual uint32_t Execute(byte commandIndex, uint32_t controlWord = 0) = 0;
    virtual ~LO() {}

};


#endif