#ifndef _HW_CMD_LISTS_
#define _HW_CMD_LISTS_

// Command names for operating LO1, LO2 and LO3
enum devices { General,
               RF_off,
               neg_4dBm,
               neg_1dBm, 
               pos_2dBm, 
               pos_5dBm, 
               No_change, 
               Mux_TRI, 
               Mux_DLD, 
               DIV_MODE };

// Command names for operating the reference oscillators
enum LOCmds { all_ref_off, 
              ref_LO, 
              ref_HI} ;

// Command names for operating the PLL Mux
enum mux { all_tristate };

// Command names for controlling and reporting Arduino state
enum misc { LED_off, 
            LED_on, 
            MSG_REQ, 
            SWEEP_START, 
            SWEEP_END, 
            RESET };
#endif
