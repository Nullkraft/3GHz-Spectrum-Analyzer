#ifndef _HW_CMD_LISTS_
#define _HW_CMD_LISTS_

// Command names for operating LO1, LO2 and LO3
const int General   =  0;
const int RF_off    =  1;
const int neg_4dBm  =  2;
const int neg_1dBm  =  3;
const int pos_2dBm  =  4;
const int pos_5dBm  =  5;
const int No_change =  6;
const int Mux_TRI   =  7;
const int Mux_DLD   =  8;

// Command names for operating the reference oscillators
const int all_ref_off = 0;
const int ref_LO      = 1;
const int ref_HI      = 2;

// Command names for operating the PLL Mux
const int all_tristate = 0;

// Command names for controlling and reporting Arduino state
const int LED_off     = 0;
const int LED_on      = 1;
const int MSG_REQ     = 2;
const int RTS         = 3;  // PC Application requests serial buffer status
const int ADC_315     = 4;  // Selects the ADC associated with LO2 output
const int ADC_045     = 5;  // Selects the ADC associated with LO3 output
const int SWEEP_START = 7;  // Incoming 32 bit serial words are FMN Data words
const int SWEEP_END   = 8;  // Incoming 32 bit serial words are Direct LO Register values

#endif
