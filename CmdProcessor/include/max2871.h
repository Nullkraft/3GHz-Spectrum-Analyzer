#ifndef _MAX2871_
#define _MAX2871_

#include <Arduino.h>    // Includes typedef for uint32_t
#include <SPI.h>


/* Default register values for MAX2871 LO: Sets default RFOout = 3865.0 MHz */
/* This correlates to an 80 MHz RFin value
    LO2 = 3865.0 MHz
    Fractional Mode
    MUX = TriState
    DIVA = 1
    N = 58
    F = 2294
    M = 4092
    RFOut_A = OFF / 0dBm
    RFOut_B =  ON / 5dBm
*/
/* Frequency 3551.0
 * 001AE6B0 = R[0]
 * 40017FE1 = R[1]
 * 80005F42 = R[2]
 * 04009F23 = R[3]
 * 638E83C4 = R[4]
 * 00400005 = R[5]
*/

typedef struct maxRegisters {
  static const byte numRegisters = 7;
  static const byte numProgrammableRegs = 5;
  uint32_t Reg[numRegisters] = { 0x001D47B0,  // 001D47B0 = R[0] N = Bits[30:15], F = Bits[14:3]
                                 0x40017FE1,  // 40017FE1 = R[1] M = Bits[14:3]
                                 0x80005F42,  // 80005F42 = R[2] Digital Lock detect OFF = MUX=Bits[28:26] = 110
                                 0x04009F23,  // 04009F23 = R[3] Fast Lock enabled = Bits[16:15]
                                 0x638E83C4,  // 638E83C4 = R[4] RFout_B enabled @ +5dBm / RFout_A disabled
                                 0x00400005,  // 00400005 = R[5] 
                                 0x98005F42   // 98005F42 = R[2] Digital Lock detect ON --> MUX=Bits[28:26]
                               };
//  uint16_t* RLO2_as_int = (uint16_t*)RLO2;
} max2871Registers;


class MAX2871_LO {
  private:
    static const int NUMBER_OF_FUNCTIONS = 8;

  public:
    const max2871Registers Default;   // Default read-only copy of the registers
    max2871Registers Curr;            // Modifiable copy of the registers for LO3

    /* 6 bit mask of Embedded Data from serial Specific Command */
    const short Data_Mask = 0x3F;

    /* 12 bit mask, R[1] bits [14:3], for Fractional Modulus Value, M */
    #define M_set 0x7FF8      // const uint32_t M_set = 0x7FF8;
    #define M_clr 0xFFFF8007  // const uint32_t M_clr = 0xFFFF8007;

    /* 12 bit mask, R[0] bits [14:3], for Frequency Division Value, F */
    #define F_set 0x7FF8      // const uint32_t F_set = 0x7FF8;
    #define F_clr 0xFFFF8007  // const uint32_t F_clr = 0xFFFF8007;

    /* 8 bit mask, R[0] bits [22:15], for Integer Counter, N */
    #define N_set 0x7F8000    // const uint32_t N_set = 0x7F8000;
    #define N_clr 0xFFFF8007  // const uint32_t N_clr = 0xFFFF8007;

    /* 20 bit mask, R[0] bits [22:3], for N and F */
    #define NF_set 0x7FFFF8   // const uint32_t NF_set = 0x7FFFF8;
    #define NF_clr 0xFF800007 // const uint32_t NF_clr = 0xFF800007;

    /* R4<8> and R4<5> disable RFoutB and RFoutA */
    const uint32_t RFpower_off = 0xFFFFFE07;

    /* R4<7:6> Adjust the RFoutB power level. (RFoutA is off by default) */
    /* Also provides a differenct name to make the main sketch more readable */
    const uint32_t Power_Level_Mask = RFpower_off;

    /* R4<7:6> Adjust the RFoutB power level. (RFoutA is off by default) */
    /* The MSbit, R4<8>, ensures that the RFout is enabled */
    const uint32_t neg4dBm = 0x100;
    const uint32_t neg1dBm = 0x140;
    const uint32_t pos2dBm = 0x180;
    const uint32_t pos5dBm = 0x1C0;

    /* R4<22:20> Set the RFOut Divider Mode to 1, 2, 4, 8, 16, 32, 64, or 128 */
    #define RFOUT_DIV_MASK 0xFF8FFFFF // const uint32_t RFOUT_DIV_MASK = 0xFF8FFFFF;  // 00700000;

    /*****  ----------------------- NOTE: --------------------------  *****/
    /***** | Enabling Tristate, Mux_Set_TRI, automatically disables | *****/
    /***** | Digital Lock Detect, Mux_Set_DLD, and vice versa.      | *****/
    /*****  --------------------------------------------------------  *****/

    /* 'AND' Mux_Set_TRI with R2 to enable Tristate. Affects bits <28:26> */
    const uint32_t Mux_Set_TRI = 0xE3FFFFFF;

    /* 'OR' Mux_Set_DLD with R2 to enable Digital Lock Detect. Affects bits <28:26> */
    const uint32_t Mux_Set_DLD = 0x18000000;

    uint32_t spiMaxSpeed = 20000000;   // 20 MHz max SPI clock

    // Create a function pointer to use in the function pointer array
    typedef uint32_t (MAX2871_LO::*CmdFunc)(uint32_t);

    // Create the function pointer array
    CmdFunc maxCmds[NUMBER_OF_FUNCTIONS] = {
      &MAX2871_LO::turn_off_RF,   // 0
      &MAX2871_LO::set_n4dBm,     // 1
      &MAX2871_LO::set_n1dBm,     // 2
      &MAX2871_LO::set_p2dBm,     // 3
      &MAX2871_LO::set_p5dBm,     // 4
      &MAX2871_LO::set_TRI,       // 5
      &MAX2871_LO::set_DLD,       // 6
      &MAX2871_LO::set_DIV_MODE,  // 7
    };

    void begin(uint8_t, bool);
    void set_NF_bits(uint32_t);
    void set_M_bits(uint32_t);
    /* dummyControlWord sets the function signatures so they all
     * match. If not, then more than one function-pointer-array
     * would be needed, negating the benefit of 'indexing' into
     * the array.
     */
    uint32_t set_DLD(uint32_t dummyControlWord);
    uint32_t set_TRI(uint32_t dummyControlWord);
    uint32_t set_DIV_MODE(uint32_t controlWord);
    uint32_t turn_off_RF(uint32_t dummyControlWord);
    uint32_t set_n4dBm(uint32_t dummyControlWord);
    uint32_t set_n1dBm(uint32_t dummyControlWord);
    uint32_t set_p2dBm(uint32_t dummyControlWord);
    uint32_t set_p5dBm(uint32_t dummyControlWord);

    void spiWrite(uint32_t reg, uint8_t selectPin); // Write 4 bytes to chip register
    uint32_t MAX2871Execute(byte commandIndex, uint32_t controlWord);
};


#endif



// Do LO power level commands next
