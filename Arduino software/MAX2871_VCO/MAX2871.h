/*
 * Hardware description of the MAX2871 Fractional/Integer-N Synthesizer/VCO chip made
 * by Maxim Integrated.
 */

#ifndef __MAX2871.H__
#define __MAX2871.H__

/*
 * MUX Configuration bits for setting the MUX MODE.
 * Bit 3 ==> Reg 5 bit 18
 * Bit 2 ==> Reg 2 bit 28
 * Bit 1 ==> Reg 2 bit 27
 * Bit 0 ==> Reg 2 bit 26
 */
#define MUX_TRI_STATE   0000  // Put MUX pin in tri-state output mode
#define MUX_D_VDD       0001  // Set MUX pin to Vdd  (Use for troubleshooting comms)
#define MUX_D_GND       0010  // Clear MUX pin Gnd   (Use for troubleshooting comms)
#define MUX_R_DIV       0011  // R-divider output
#define MUX_N_DIV       0100  // N-divider output/2
#define MUX_ALOCK_DET   0101  // Analog lock detect
#define MUX_DLOCK_DET   0110  // Digital lock detect
#define MUX_SYNC        0111  // Sync Input
//              1000 to 1011  // Reserved
#define MUX_READ        1100  // Read SPI registers 06
//              1101 to 1111  // Reserved



#endif



class MAX2871_VCO {
public:

	MAX2871();
	MAX2871(float initial_frequency);

	char* begin();
  void  begin(float initial_frequency);
  void  setFrequency(float target_frequency);
  void  enableRFout(int pinNumber);
  void  disableRFout(int pinNumber);
  void  turnOnLED(int pinNumber);
  void  turnOffLED(int pinNumber);
  char* getHWStatus(int pinNumber);

private:

  //void setFrequency(float freq);
};




