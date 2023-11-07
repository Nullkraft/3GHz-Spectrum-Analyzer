#ifndef _SYNTHESIZER_H_
#define _SYNTHESIZER_H_

#include <Arduino.h>

class LO {
  public:
    virtual void begin(uint8_t selectPin, bool first_init) = 0;

};


#endif