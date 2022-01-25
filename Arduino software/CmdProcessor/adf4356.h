#ifndef _ADF4356_
#define _ADF4356_

#include <Arduino.h>    /* Needed for uint32_t */


/* Default register values for MAX2871 LO: Sets RFOout = 3945 MHz */
typedef struct {
  static const byte numRegisters = 15;
  uint32_t Reg[numRegisters] = { 0x002007C0,
                                 0x00000001,
                                 0x00000012,
                                 0x00000003,
                                 0x32008984,
                                 0x00800025,
                                 0x95012046,
                                 0x060000F7,
                                 0x15596568,
                                 0x0F09FCC9,
                                 0x00C00EBA,
                                 0x0061200B,
                                 0x000015FC,
                                 0x0000000D,
                                 0x02008984
                               };
} adf4356registers;




class ADF4356_LO {
  public:
    void begin(float initial_frequency);

    const adf4356registers Default;   // Default read-only copy of the registers
    adf4356registers Curr;            // Current modifiable copy of the registers

    /* 16 bit Mask of Embedded Data from serial Specific Command */
    const uint32_t Data_Mask = 0x300000;

    /* Clear old INT_N bits from Regist 0 */
    const uint32_t INT_N_Mask = 0xFFF0000F;

    /* R6<DB9> and <DB6> disable RFoutB and RFoutA */
    const uint32_t RFpower_off = 0xFFFFFC0F;

    /* R6<DB5:DB4> Adjust the RFoutA power level. (RFoutB is off by default) */
    /* Also provides a differenct name to make the main sketch more readable */
    const uint32_t Power_Level_Mask = RFpower_off;

    /* R6<DB8:DB7> Adjust the RFoutA power level. (RFoutB is off by default) */
    /* The MSbit, R6<DB9> ensures that the RFout is enabled */
    const uint32_t neg4dBm = 0x40;
    const uint32_t neg1dBm = 0x50;
    const uint32_t pos2dBm = 0x60;
    const uint32_t pos5dBm = 0x70;

    /*****  ----------------------- NOTE: --------------------------  *****/
    /***** | Enabling Tristate, Mux_Set_TRI, automatically disables | *****/
    /***** | Digital Lock Detect, Mux_Set_DLD, and vice versa.      | *****/
    /*****  --------------------------------------------------------  *****/

    /* 'AND' Mux_Set_TRI with R4 to enable Tristate. Affects bits <DB29:DB27> */
    const uint32_t Mux_Set_TRI = 0xC7FFFFFF;

    /* 'OR' Mux_Set_DLD with R4 to enable Digital Lock Detect. Affects bits <DB29:DB27> */
    const uint32_t Mux_Set_DLD = 0x30000000;

    uint32_t spiMaxSpeed = 50000000;   // 50 MHz max SPI clock
};


#endif






/* REGISTER INITIALIZATION SEQUENCE
  At initial power-up, after the correct application of voltages to
  the supply pins, the ADF4356 registers must be programmed in
  sequence.

  For Fpfd ≤ 75 MHz, use the following sequence:
  1. Register 13
  2. Register 12
  3. Register 11
  4. Register 10
  5. Register  9
  6. Register  8
  7. Register  7
  8. Register  6
  9. Register  5
  10. Register  4
  11. Register  3
  12. Register  2
  13. Register  1
  14. Ensure that >16 ADC_CLK cycles have elapsed between writing to Reg10 & Reg0.
    For example, if ADC_CLK = 99.417 kHz, wait 16/99,417 sec = 161 μs.
    See the Register 10 section for more information.
  15. Register  0



*/
