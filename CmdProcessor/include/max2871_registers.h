#ifndef _MAX2871_REGISTERS_H_
#define _MAX2871_REGISTERS_H_

#include <Arduino.h>

class maxRegisters {
    static const uint8_t num_registers = 6;
    uint32_t reg[num_registers];

    maxRegisters() {
        reg[0] = 0x001D47B0;  // 001D47B0 = R[0] N = Bits[30:15], F = Bits[14:3]
        reg[1] = 0x40017FE1;  // 40017FE1 = R[1] M = Bits[14:3]
        reg[2] = 0x80005F42;  // 80005F42 = R[2] Digital Lock detect OFF = MUX=Bits[28:26] = 110 : (0x98005F42 = DLD ON)
        reg[3] = 0x04009F23;  // 04009F23 = R[3] Fast Lock enabled = Bits[16:15]
        reg[4] = 0x638E83C4;  // 638E83C4 = R[4] RFout_B enabled @ +5dBm / RFout_A disabled
        reg[5] = 0x00400005;  // 00400005 = R[5]
    };
};

class MAX2871 {
        /* 6 bit mask of Embedded Data from serial Specific Command */
    static constexpr short Data_Mask = 0x3F;

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
    static constexpr uint32_t RFpower_off = 0xFFFFFE07;

    /* R4<7:6> Adjust the RFoutB power level. (RFoutA is off by default) */
    /* Also provides a differenct name to make the main sketch more readable */
    static constexpr uint32_t Power_Level_Mask = RFpower_off;

    /* R4<7:6> Adjust the RFoutB power level. (RFoutA is off by default) */
    /* The MSbit, R4<8>, ensures that the RFout is enabled */
    static constexpr uint32_t neg4dBm = 0x100;
    static constexpr uint32_t neg1dBm = 0x140;
    static constexpr uint32_t pos2dBm = 0x180;
    static constexpr uint32_t pos5dBm = 0x1C0;

    /* R4<22:20> Set the RFOut Divider Mode to 1, 2, 4, 8, 16, 32, 64, or 128 */
    #define RFOUT_DIV_MASK 0xFF8FFFFF // const uint32_t RFOUT_DIV_MASK = 0xFF8FFFFF;  // 00700000;

    /*****  ----------------------- NOTE: --------------------------  *****/
    /***** | Enabling Tristate, Mux_Set_TRI, automatically disables | *****/
    /***** | Digital Lock Detect, Mux_Set_DLD, and vice versa.      | *****/
    /*****  --------------------------------------------------------  *****/

    /* 'AND' Mux_Set_TRI with R2 to enable Tristate. Affects bits <28:26> */
    static constexpr uint32_t Mux_Set_TRI = 0xE3FFFFFF;

    /* 'OR' Mux_Set_DLD with R2 to enable Digital Lock Detect. Affects bits <28:26> */
    static constexpr uint32_t Mux_Set_DLD = 0x18000000;

    uint32_t spiMaxSpeed = 20000000;   // 20 MHz max SPI clock

};

#endif

/*  The stuff below are the differences between the defines in MAX2871
    and ADF4356 header files. The rest of the names are the same.
*/

/*  // ADF junk

    /* Clear old INT_N bits from Regist 0
    #define INT_N_Mask 0xFFF0000F // const uint32_t INT_N_Mask = 0xFFF0000F;
*/

/*  // MAX junk

    /* 12 bit mask, R[1] bits [14:3], for Fractional Modulus Value, M *
    #define M_set 0x7FF8      // const uint32_t M_set = 0x7FF8;
    #define M_clr 0xFFFF8007  // const uint32_t M_clr = 0xFFFF8007;

    /* 12 bit mask, R[0] bits [14:3], for Frequency Division Value, F *
    #define F_set 0x7FF8      // const uint32_t F_set = 0x7FF8;
    #define F_clr 0xFFFF8007  // const uint32_t F_clr = 0xFFFF8007;

    /* 8 bit mask, R[0] bits [22:15], for Integer Counter, N *
    #define N_set 0x7F8000    // const uint32_t N_set = 0x7F8000;
    #define N_clr 0xFFFF8007  // const uint32_t N_clr = 0xFFFF8007;

    /* 20 bit mask, R[0] bits [22:3], for N and F 
    #define NF_set 0x7FFFF8   // const uint32_t NF_set = 0x7FFFF8;
    #define NF_clr 0xFF800007 // const uint32_t NF_clr = 0xFF800007;
*/

/*  Here's all the items that are named the same in both headers

    //* 6 bit mask of Embedded Data from serial Specific Command *
    static constexpr short Data_Mask = 0x3F;

    //* R4<8> and R4<5> disable RFoutB and RFoutA *
    static constexpr uint32_t RFpower_off = 0xFFFFFE07;

    //* R4<7:6> Adjust the RFoutB power level. (RFoutA is off by default) *
    //* Also provides a differenct name to make the main sketch more readable *
    static constexpr uint32_t Power_Level_Mask = RFpower_off;

    //* R4<7:6> Adjust the RFoutB power level. (RFoutA is off by default) *
    //* The MSbit, R4<8>, ensures that the RFout is enabled *
    static constexpr uint32_t neg4dBm = 0x100;
    static constexpr uint32_t neg1dBm = 0x140;
    static constexpr uint32_t pos2dBm = 0x180;
    static constexpr uint32_t pos5dBm = 0x1C0;

    //* R4<22:20> Set the RFOut Divider Mode to 1, 2, 4, 8, 16, 32, 64, or 128 *
    #define RFOUT_DIV_MASK 0xFF8FFFFF // const uint32_t RFOUT_DIV_MASK = 0xFF8FFFFF;  // 00700000;

    //* 'AND' Mux_Set_TRI with R2 to enable Tristate. Affects bits <28:26> *
    static constexpr uint32_t Mux_Set_TRI = 0xE3FFFFFF;

    //* 'OR' Mux_Set_DLD with R2 to enable Digital Lock Detect. Affects bits <28:26> *
    static constexpr uint32_t Mux_Set_DLD = 0x18000000;

    uint32_t spiMaxSpeed = 20000000;   // 20 MHz max SPI clock

*/