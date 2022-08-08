// CmdProcessor_ASCII_InitRegs_2.h RF=100MHz; IF1=3620MHz IF2=315MHz IF3=45MHz. Avoid IF's with harmonics of Fpfd,esp IF1   10Jan2022
//Attenuation Examples:  0x00=0dB   0x40=16dB     0x60=24dB  0x7F=31.75dB
byte ByteA = 0x7F ; // 31.75dB
// Fixed Frequency=MHz, 60 MHz Reference. Codes for 6 total registers
//Digital Lock Detect Enabled, 3.2mA Charge Pump(unless Fast-Lock)

// RF = 100 MHz    LO1 = 3720MHz
unsigned long WordLO1[]={ 0x002007C0,
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
// R4=0x32008984 Dig Lockdet Initially, then switch to  R14=0x02008984 for TriState  Power Min @-4dBm, Bleed Bits changed, 

// 3935 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz
unsigned long WordLO2[]={ 0x00419550,
                          0x2000FFE9,
                          0x58008042,
                          0xF8008003,
                          0x638FF1C4,
                          0x01400005,
                          0x40008042
                        };//3935MHz PFD=30MHz PowerMax B A-Off Fast-Lock En
// R2=0x58008042 Dig Lockdet Initally then switch to R6=0x40008042 for TriState// Reg % <31>='0' per spec sheet. Inconsequential
//R5 <24>='1'. Allows Auto Int-N mode when F=0

// 270 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz
unsigned long WordLO3[]={ 0x00480000,
                          0x20008011,
                          0x58008042,
                          0xF8008003,
                          0x63CFF104,
                          0x01400005,
                          0x40008042
                        };//270 MHz PFD=30MHz  PowerMin B A-Off Fast-Lock En
// R2=0x58008042 Dig Lockdet Initally then switch to R6=0x40008042 for TriState// Reg % <31>='0' per spec sheet. Inconsequential
//R5 <24>='1'. Allows Auto Int-N mode when F=0
