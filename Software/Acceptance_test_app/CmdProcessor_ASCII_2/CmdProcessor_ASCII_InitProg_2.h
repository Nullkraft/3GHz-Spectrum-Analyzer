/*  CmdProcessor_ASCII_InitProg_2.h
 *   for loop counts backwards from Reg 5 to Reg 0 2X Per Spec Sheet
 *   int y; // counts from 1 downto 0
 *   int x; // For LO1 Register 13 downto 0 and For LO2 & LO3 counts Register 5 downto 0
 *   LO1 uses R14 and LO2&LO3 uses R6 to Tri-State PLL_MUX
 */
 
ShiftAtten();  // Program Attenuator
delay(pauseMillis);

for (int y = 1; y >= 0; y--)
{
  // Serial.print (" Y counter= ");
  //  Serial.println (y);

  LD_PIN = LD1;
  nameLO = "LO1";
  for (int x = 13; x >= 0; x--) {
    z = WordLO1[x];
    ShiftLO(x);                     // Program LO1=3776.52 MHz with LD on Mux
    delay (pauseMillis);
  }

  MuxTest("LO1");                // Now read the Mux Pin
  int x = 14;
  z = WordLO1[x];
  ShiftLO(x);                       //LO1 Mux Off
  delay(pauseMillis);


  LD_PIN = LD2;
  nameLO = "LO2";
  for (int x = 5; x >= 0; x--) {
    z = WordLO2[x];
    ShiftLO(x);                     // Program LO2=3915 MHz with LD on Mux
    delay (pauseMillis);
  }

  MuxTest("LO2");                   // Now read the Mux Pin
  x = 6;
  z = WordLO2[x];
  ShiftLO(x);                       // LO2 Mux Off
  delay(pauseMillis);


  LD_PIN = LD3;
  nameLO = "LO3";
  for (int x = 5; x >= 0; x--) {    // Program LO3=270MHz with LD on Mux
    z = WordLO3[x];
    ShiftLO(x);
    delay (pauseMillis);
  }

  MuxTest("LO3");                   // Read the Mux Pin
  x = 6;
  z = WordLO3[x];
  ShiftLO(x);                       // LO3 Mux Off
  delay(pauseMillis);
} // end for y
