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
byte ByteA = 0x7F;    // Full attenuation or 31.75 dB

// Fixed Frequency=MHz, 60 MHz Reference. Codes for 6 total registers
// Digital Lock Detect Enabled, 3.2mA Charge Pump, 10 sec Code Refresh

// RF = 10 MHz    LO1 = 3630MHz
unsigned long WordLO1[] = {0x00200790,
                           0x00000001,
                           0x00000002,
                           0x00000003,
                           0x32008984,
                           0x00800025,
                           0x35012046,
                           0x060000E7,
                           0x15596568,
                           0x0F09FCC9,
                           0x00C00EBA,
                           0x0061200B,
                           0x000015FC,
                           0x0000000D,
                           0x02008984
                          };
// R4=0x32008984 Dig Lockdet  or  R14=0x02008984 TriState

// 3955 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz, RFoutB at max power
unsigned long WordLO2[] = {0x00418028,
                           0x20008031,
                           0x58009242,
                           0xF8008003,
                           0x638FF1C4,
                           0x00400005,
                           0x40009242
                          }; //395MHz PFD=30MHz
// R2=0x58009242 Dig Lockdet  or R6=0x40009242 TriState

// 270 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz
unsigned long WordLO3[] = {0x00480000,
                           0x20008011,
                           0x58009242,
                           0xF8008003,
                           0x63CFF104,
                           0x00400005,
                           0x40009242
                          };
// R2=0x58009242 Dig Lockdet  or R6=0x40009242 TriState
