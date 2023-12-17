#ifndef _MAX2871_REGISTERS_H_
#define _MAX2871_REGISTERS_H_

#include <Arduino.h>

class adfRegisters {
    static const uint8_t num_registers = 14;
    uint32_t reg[num_registers];

    adfRegisters () {
         reg[0] = 0x00200370;  // R[0] N = 0x37 (dec 55)
         reg[1] = 0x00000001;
         reg[2] = 0x00000012;
         reg[3] = 0x00000003;
         reg[4] = 0x30008984;  // R[4] Muxout = Digital Lock Detect DB[29:27]
         reg[5] = 0x00800025;
         reg[6] = 0x35034046;
         reg[7] = 0x060000F7;  // R[7] Int-N mode DB[4]
         reg[8] = 0x15596568;
         reg[9] = 0x2A1BBCC9;
        reg[10] = 0x00C0297A;
        reg[11] = 0x0061200B;
        reg[12] = 0x000015FC;
        reg[13] = 0x0000000D;
    };

    /* 16 bit Mask of Embedded Data from serial Specific Command */
    static constexpr uint32_t Data_Mask = 0x300000;

    /* Clear old INT_N bits from Regist 0 */
    #define INT_N_Mask 0xFFF0000F // const uint32_t INT_N_Mask = 0xFFF0000F;

    /* R6<DB9> and <DB6> disable RFoutB and RFoutA */
    static constexpr uint32_t RFpower_off = 0xFFFFFC0F;

    /* R6<DB5:DB4> Adjust the RFoutA power level. (RFoutB is off by default) */
    /* Also provides a differenct name to make the main sketch more readable */
    static constexpr uint32_t Power_Level_Mask = RFpower_off;

    /* R6<DB8:DB7> Adjust the RFoutA power level. (RFoutB is off by default) */
    /* The MSbit, R6<DB9> ensures that the RFout is enabled */
    static constexpr uint32_t neg4dBm = 0x40;
    static constexpr uint32_t neg1dBm = 0x50;
    static constexpr uint32_t pos2dBm = 0x60;
    static constexpr uint32_t pos5dBm = 0x70;

    /* R4<24:15> Set the RFOut Divider Mode from 1 to 1023 */
    #define RFOUT_DIV_MASK 0xFF8FFFFF // const uint32_t RFOUT_DIV_MASK = 0xFF8FFFFF;  // 00700000;

    /*****  ----------------------- NOTE: --------------------------  *****/
    /***** | Enabling Tristate, Mux_Set_TRI, automatically disables | *****/
    /***** | Digital Lock Detect, Mux_Set_DLD, and vice versa.      | *****/
    /*****  --------------------------------------------------------  *****/

    /* 'AND' Mux_Set_TRI with R4 to enable Tristate. Affects bits <DB29:DB27> */
    static constexpr uint32_t Mux_Set_TRI = 0xC7FFFFFF;

    /* 'OR' Mux_Set_DLD with R4 to enable Digital Lock Detect. Affects bits <DB29:DB27> */
    static constexpr uint32_t Mux_Set_DLD = 0x30000000;

    uint32_t spiMaxSpeed = 50000000;   // 50 MHz max SPI clock

};


#endif

/*  // max2871 junk that adf4356 junk doesn't have

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
