#ifndef _SPECANN_
#define _SPECANN_

#include <Arduino.h>
#include <adf4356.h>  // driver
#include <max2871.h>  // driver
#include <SPI.h>

/* Vref = 2.595  Volts/ADC_bit == 0.002536 Volts
 * Noise-floor ≈ 0.488 Volts
 * ADC_value of 193 ≈ 0.488/0.002536
*/

class CmdObj {
  private:
    uint16_t data;  // 16 bits
    byte cmd;
    byte addr;
    const byte AddressMask = 0x07;  // Mask out 3 bits of 'Register Address' from serialWord[1]
    const byte CommandFlag = 0xFF;  // Byte pattern to identify a 'Control Word'
    /*           Serial Word with Command Flag:
      +------------------------------------------------+
      |       Embedded      | Instr- |Addr.|  Command  |  NOTE: Command Flag
      |       Data          | uction |     |   Flag    |        always = 0xFF
      +---------------------+--------+-----+-----------+
      [ xxxx_xxxx_xxxx_xxxx | xxxx_x | xxx | 1111_1111 ]
    */
    // const uint8_t numBytesInSerialWord = 4;
    uint32_t serialWord;                                  // Serial Word as 32 bits
    // uint8_t* serialWordAsBytes = reinterpret_cast<uint8_t*>(&serialWord);   // Serial Word as array with 4 bytes
    // uint16_t* serialWordAsInts = reinterpret_cast<uint16_t*>(&serialWord);  // Serial Word as array with 2 ints

  public:
    // CmdObj() : data(0), cmd(0), addr(0) {}
    CmdObj() {}

    // serialWord is 32 bits
    void parseSpecificInstruction(uint32_t serialWord) {
      uint8_t newAddr = (serialWord >> 8) & AddressMask;   // Mask out the lsb 3-bit Address
      if ((newAddr <= 4) || (newAddr == 7)) { // Reserves addresses 5 and 6
        addr = newAddr;
        cmd = (serialWord >> 11) & 0x1F;        // Mask out the msb 5-bits to cmd
        data = serialWord >> 16;                  // Shift the upper 16-bits to Data
      } else {
        // Invalid Address
      }
    }

    uint16_t getData() const { return data; }
    uint16_t getCommand() const { return cmd; }
    uint16_t getAddress() const { return addr; }
};

class SpecAnn {
  private:
    #define NUM_MISC_FUNCTIONS 4
    #define NUM_CLK_FUNCTIONS 3

  public:
    uint8_t spi_select;   // Chip selected for SPI programming

    /* Command-to-Function Mapping:
    *
    * Each instruction found in the API ('Instruction List XXX.ods') has an
    * associated Address and Command. The Address identifies the component
    * and the Command identifies the action to be performed.
    *
    * The 'Command Number' is found in the API document and is 5 bits long.
    * It is ideal as an index into one of the xxxYYYYCmnMap[] arrays. The
    * CmdMap arrays decouple the API Command Number from the layout of the
    * function-pointer arrays found in the max2871.h and adf4356.h files.
    *
    * The enums provide descriptive command names for the function-pointer
    * arrays instead of using the Command Numbers.
    *
    * For a list of available commands and their corresponding functions, refer
    * to the Execute() function in max2871.h and the Execute() function in adf4356.h.
    */
    enum loCmdList{GERERAL, RFOFF, N4DBM, N1DBM, P2DBM, P5DBM, CHANGE_FREQ, TRI, DLD, DIV_MODE, NA=30, };
    enum arduinoCmdList{LED_OFF, LED_ON, VERSION, BEGIN_SWEEP, };
    enum ckCmdLIst{ALL_OFF, REF_LO_ON, REF_HI_ON, };

    // Command map declarations
    uint8_t max2871CmdMap[10] {
      SpecAnn::NA, SpecAnn::RFOFF, SpecAnn::N4DBM, SpecAnn::N1DBM, SpecAnn::P2DBM,
      SpecAnn::P5DBM, SpecAnn::NA, SpecAnn::TRI, SpecAnn::DLD, SpecAnn::DIV_MODE
      };
    uint8_t adf4356CmdMap[9] {
      SpecAnn::NA, SpecAnn::RFOFF, SpecAnn::N4DBM, SpecAnn::N1DBM, SpecAnn::P2DBM,
      SpecAnn::P5DBM, SpecAnn::NA, SpecAnn::TRI, SpecAnn::DLD
      };
    uint8_t arduinoCmdMap[4] {
      SpecAnn::LED_OFF, SpecAnn::LED_ON, SpecAnn::VERSION, SpecAnn::BEGIN_SWEEP
      };
    uint8_t clkCmdMap[3] {
      SpecAnn::ALL_OFF, SpecAnn::REF_LO_ON, SpecAnn::REF_HI_ON
      };

    ADF4356_LO LO1;
    ADF4356_LO* ptrLO1 = &LO1;
    MAX2871_LO LO2;
    MAX2871_LO* ptrLO2 = &LO2;
    MAX2871_LO LO3;
    MAX2871_LO* ptrLO3 = &LO3;
    MAX2871_LO* LO;

    // Status codes use the 4 msbits of the 16 bit ADC return values
    // const int start_noise_floor = 0xD0;  // First in the series of ADC noise-floor readings
    // const int end_noise_floor   = 0xE0;  // Last in the series of ADC noise-floor readings
    static constexpr int failed_to_lock = 0xF0;  // VCO Failed to lock

    // BitMask for programming the registers of the Attenuator IC
    static constexpr uint16_t ATTEN_Data_Mask = 0x7F;  // 7 bits of Embedded Data

    /*********** ARDUINO PIN DEFINITIONS ***********/
    static constexpr uint8_t LO1_SEL = A3;
    static constexpr uint8_t LO2_SEL = 3;
    static constexpr uint8_t LO3_SEL = A4;
    static constexpr uint8_t REF_LO_SEL = 8;
    static constexpr uint8_t REF_HI_SEL = 9;
    static constexpr uint8_t ADC_SEL_045 = A0;  // ADC for LO2
    static constexpr uint8_t ADC_SEL_315 = A1;  // ADC for LO3
    static constexpr uint8_t PLL_MUX = A2;      // Equals physical pin 16 on Port C (use PCMSK1)
    static constexpr uint8_t ATTEN_SEL = A5;
    //static constexpr uint8_t SPI_MOSI  = 11;   // Reserved by the SPI Library
    //static constexpr uint8_t SPI_MISO  = 12;   // Reserved by the SPI Library
    //static constexpr uint8_t SPI_CLOCK = 13;   // Reserved by the SPI Library

    // Command Word Addresses of all the hardware Integrated Circuit chips
    // TODO: Create the list in a config file that remains with the project files.
    static constexpr uint8_t Attenuator = 0;
    static constexpr uint8_t LO1_addr = 1;
    static constexpr uint8_t LO2_addr = 2;
    static constexpr uint8_t LO3_addr = 3;
    static constexpr uint8_t RefClock = 4;
    static constexpr uint8_t MISC_addr = 7;

    /*********** HARDWARE DEFINITIONS END *******/
    SpecAnn();  //Ctor

    void init_specann();
    void builtinLEDOff();
    void builtinLEDOn();
    void version();
    void end_sweep_ack();

    void all_ref_off();
    void ref_LO();
    void ref_HI();

    void updateLORegisters(MAX2871_LO* loPtr, uint8_t spiSelect, uint8_t command, uint32_t serialWord);
    void updateAtten(uint8_t reg, uint8_t selectPin);

    typedef void (SpecAnn::*MiscFuncs)();
    void miscExecute(uint8_t);

    // Array of function-pointers containing the
    // 'Spectrum Analyzer miscellenious functions'
    MiscFuncs miscCmds[NUM_MISC_FUNCTIONS] = {
      &SpecAnn::builtinLEDOff, // Command number 0
      &SpecAnn::builtinLEDOn,  // Command number 1
      &SpecAnn::version,       // Command number 2
      &SpecAnn::end_sweep_ack, // Command number 3
    };

    typedef void (SpecAnn::*RefClockFuncs)();
    void clkExecute(uint8_t);

    // function-pointer array for clock selection functions
    RefClockFuncs refClockCmds[NUM_CLK_FUNCTIONS] = {
      &SpecAnn::all_ref_off,  // Command number 0
      &SpecAnn::ref_LO,       // Command number 1
      &SpecAnn::ref_HI,       // Command number 2
    };


};

#endif  // _SPECANN_
