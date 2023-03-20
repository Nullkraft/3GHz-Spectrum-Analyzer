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
