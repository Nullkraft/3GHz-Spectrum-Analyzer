#ifndef _ADF4356_
#define _ADF4356_

#include <Arduino.h>    /* Needed for uint32_t */
#include <SPI.h>


/* Default register values for MAX2871 LO: Sets RFOout = 3630.0 MHz */
typedef struct adfRegisters {
  static const byte numRegisters = 15;
  uint32_t Reg[numRegisters] = { 0x00200370,  // R[0] N = 0x37 (dec 55)
                                 0x00000001,
                                 0x00000012,
                                 0x00000003,
                                 0x30008984,  // R[4] Muxout = Digital Lock Detect DB[29:27]
                                 0x00800025,
                                 0x35034046,
                                 0x060000F7,  // R[7] Int-N mode DB[4]
                                 0x15596568,
                                 0x2A1BBCC9,
                                 0x00C0297A,
                                 0x0061200B,
                                 0x000015FC,
                                 0x0000000D,
                                 0x30008984   // R[4] Digital Lock detect DB[29:27]
                               };
} adf4356registers;

class ADF4356_LO {
  private:
    static const int NUMBER_OF_FUNCTIONS = 10;

  public:
    // ADF4356_LO(int num_funcs);

    typedef uint32_t (ADF4356_LO::*CmdFunc)();  // Create a funcPtr type

    /* Create an array of function pointers to replace the CmdProcessor.cpp
     * giant switch-case statements. Intialize adfCmds with all the
     * available commands
     */
    CmdFunc adfCmds[NUMBER_OF_FUNCTIONS] = {
      &ADF4356_LO::unused,
      &ADF4356_LO::turn_RF_off,
      &ADF4356_LO::set_n4dBm,
      &ADF4356_LO::set_n1dBm,
      &ADF4356_LO::set_p2dBm,
      &ADF4356_LO::set_p5dBm,
      &ADF4356_LO::unused,
      &ADF4356_LO::set_TRI,
      &ADF4356_LO::set_DLD,
      &ADF4356_LO::unused,
    };
    
    const adfRegisters Default;   // Default read-only copy of the registers
    adfRegisters Curr;            // Current modifiable copy of the registers

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

    // ADF4356 methods
    void begin(uint8_t);
    uint32_t unused();
    uint32_t turn_RF_off();
    uint32_t set_n4dBm();
    uint32_t set_n1dBm();
    uint32_t set_p2dBm();
    uint32_t set_p5dBm();
    uint32_t set_TRI();
    uint32_t set_DLD();

    uint32_t ADF4356Execute(int commandIndex) {
      if (commandIndex >= 0 && commandIndex < NUMBER_OF_FUNCTIONS) {
        return (this->*adfCmds[commandIndex])();
      }
      return 0xFFFF;    // You tried to use an undefined command
    }

    void spiWrite(uint32_t reg, uint8_t selectPin);
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
