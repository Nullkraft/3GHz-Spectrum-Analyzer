/* CmdProcessor_ASCII_InitRegs_2.h 
 *  
 *  RF =   10 MHz
 * IF1 = 3640 MHz 
 * IF2 =  315 MHz 
 * IF3 =   45 MHz
 * 
 * Avoid IF's with harmonics of Fpfd,esp IF1 
 * 
 * Attenuator Examples:  0x00 =     0dB
 *                       0x40 =    16dB
 *                       0x60 =    24dB
 *                       0x7F = 31.75dB
 */
uint8_t Atten_level = 0x7F;    // Full attenuation or 31.75 dB

// Fixed Frequency=MHz, 60 MHz Reference. Codes for 6 total registers
// Digital Lock Detect Enabled, 3.2mA Charge Pump,10 sec Code Refresh

// RF = 100 MHz   LO1 = 3720MHz
uint32_t RegLO1[] = { 0x002007C0,   // new value    // original value 0x002007D0
                      0x00000001,
                      0x00000012,
                      0x00000003,
                      0x32008984,
                      0x00800025,
                      0x95012046,   // new value 0x95012046   // original value 0x35012046
                      0x060000F7,   // new value 0x060000F7   // original value 0x060000E7
                      0x15596568,
                      0x0F09FCC9,
                      0x00C00EBA,
                      0x0061200B,
                      0x000015FC,
                      0x0000000D,
                      0x02008984
                    };
// R4=0x32008984 Dig Lockdet  or  R14=0x02008984 TriState

// 3955 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz and B at PowerMax
uint32_t RegLO2[] = { 0x00419550,   // for 3955 MHz was 0x00418028
                      0x2000FFE9,   // was 0x20008031
                      0x58008042,   // was 0x58009242 on LO3 also
                      0xF8008003,
                      0x638FF1C4,
                      0x01400005,   // was 0x00400005
                      0x40008042    // was 0x40009242 on LO3 also
                    };
// R2=0x58009242 Dig Lockdet  or R6=0x40009242 TriState

// 270 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz
uint32_t RegLO3[] = { 0x00480000,
                      0x20008011,
                      0x58008042,
                      0xF8008003,
                      0x63CFF104,
                      0x00400005,
                      0x40008042
                    };
// R2=0x58009242 Dig Lockdet  or R6=0x40009242 TriState
