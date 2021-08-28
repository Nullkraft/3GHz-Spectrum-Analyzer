
/************************************************
 *  Name    : Project1_Load_146p52_13.ino
 *  Author  : WN2A with contributions by M.Stanley
 *  Date    : 22 August 2021
 *  Version : 3.0
 *  Notes   : Code for Attenuator, (1)ADF4356 (2)MAX2871 LO2=3945MHz LO3=270MHz REF=60MHz PFD=5 MHz
 *            LO2 frequency=3945MHz Atten=16 dB  Serial=115200
 *            IF1=3630MHz IF2=315MHz IF3=45MHz
 ************************************************/

//Attenuation Examples:  0x00=0dB   0x40=16dB   0x7F=31.75dB
int ByteA = {0x40}; //16 dB
// Fixed Frequency=MHz, 60 MHz Reference. Codes for 6 total registers
//Digital Lock Detect Enabled, 3.2mA Charge Pump,10 sec Code Refresh

// RF = 146.52 MHz    LO1 = 3776.52MHz
unsigned long WordLO1[]={0x002007D0,0x0E24DD21,0x00000002,0x00000003,0x32008984,0x00800025,0x350120F6,0x060000E7,0x15596568,0x0F09FCC9,0x00C00EBA,0x0061200B,0x000015FC,0x0000000D,0x02008984};
// R4=0x32008984 Dig Lockdet  or  R14=0x02008984 TriState

// 3945 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz
unsigned long WordLO2[]={0x00418008,0x20008011,0x58009242,0xF8008003,0x638FF1DC,0x80400005,0x40009242};//3945MHz PFD=30MHz PowerMax B
// R2=0x58009242 Dig Lockdet  or R6=0x40009242 TriState

// 270 MHz Opt Fract Opt; Dig Lockdet MuxOut Fpfd=30 MHz
unsigned long WordLO3[]={0x00480000,0x20008011,0x58009242,0xF8008003,0x63CFF104,0x00400005,0x40009242};
// R2=0x58009242 Dig Lockdet  or R6=0x40009242 TriState

//Signal to Pin assignments
int LD3 = 2;       // Load LO3 of MAX2871
int DA3 = 3;       // Data LO3 of MAX2871
int DA2 = 5;       // Data LO2 of MAX2871
int LD2 = 4;       // Load LO2 of MAX2871
int CLKPLL  = 10;  // common CLK_PLL
int REF_EN1 = 11;
int REF_EN2 = 12;
int Strobe  = 13;  // temp for sync scope
int DA1     = A0;  // Data LO1 of ADF4356
int LD1     = A1;  // Load LO1 of ADF4356
int PLL_MUX = A2;
int LD_ATT  = A3;
int CLK_ATT = A4;
int DA_ATT  = A5;
int ctr     =  0;
int Mux;
int pauseMillis = 30; //delay
int logAmp45;
int logAmp315;
int ncount=0;
bool printMux = true;
int dataPin;
int clockPin=10;
int LD_PIN;
unsigned long z;


void setup() {
  //set pin Directions Input or Output
  pinMode(LD3, OUTPUT);
  pinMode(DA3, OUTPUT);
  pinMode(DA2, OUTPUT);
  pinMode(LD2, OUTPUT);
  pinMode(CLKPLL, OUTPUT);
  pinMode(REF_EN1, OUTPUT);
  pinMode(REF_EN2, OUTPUT);
  pinMode(Strobe, OUTPUT);
  pinMode(DA1, OUTPUT);
  pinMode(LD1, OUTPUT);
  pinMode(PLL_MUX, INPUT);
  pinMode(LD_ATT, OUTPUT);
  pinMode(CLK_ATT, OUTPUT);
  pinMode(DA_ATT, OUTPUT);

  digitalWrite(REF_EN1, HIGH); //Enable 60MHz
  digitalWrite(REF_EN2, LOW); //Disable 100MHz

  Serial.begin(115200);
  Serial.println ("  ");
  Serial.println("Project1_Load_146p52_13.ino for Project#1 Spectrum Analyzer RF=146.52MHz   22 August 2021   WN2A with M.Stanley contributions");
  Serial.println("Programs PE Digital Attenuator for 16 dB   IF1=3630MHz IF2=315MHz IF3=45MHz");
  Serial.println("Program L01 ADF4356 Synthesizer Vco=3776.52 MHz Ref=60 MHz PFD=5MHz?? (14 Registers)");
  Serial.println("Program L02 MAX2871 Synthesizer Vco=3945 MHz Ref=60 MHz PFD=30MHz (6 Registers)");
  Serial.println("Program L03 MAX2871 Synthesizer Vco=270.000 MHz Ref=60 MHz PFD=30MHz (6 Registers)");
  Serial.println("This version supports Synth Lock Detect signal via Mux, Register Values are in the byte datatype, not int, less RAM waste");
  Serial.println("Only Tables for RF=146.52 MHz are included here. This is intended as a cleaned up version of previous versions");

  // for loop counts backwards from Reg 5 to Reg 0 2X Per Spec Sheet //
  //int y; //to count 0 to 1
  //int x; //For LO1 Register 13>>0.  For LO2&LO3 Register 5>>0. LO1 uses R14, LO2&LO3 uses R6 to Tri-State PLL_MUX

  ctr = ctr + 1;
  Serial.print (" ctr = ");
  Serial.println (ctr);
  ShiftAtten();  // Program Attenuator
  delay(pauseMillis);

  for (int y = 2; y >= 0; y--)
  {
    Serial.print (" Y counter= ");
    Serial.println (y);

    LD_PIN=LD1;
    dataPin=DA1;
    for (int x = 13; x >= 0; x--) {
      z=WordLO1[x];
      ShiftLO(x);                    // Program LO1=37Byte33[x]76.52 MHz with LD on Mux
      delay (pauseMillis);
    }
    MuxTest("   LO1");                // Now read the Mux Pin
    int x = 14;
    z=WordLO1[x];
    ShiftLO(x); //LO1 Mux Off
    delay(pauseMillis);


    LD_PIN=LD2;
    dataPin=DA2;
    for (int x = 5; x >= 0; x--) {
      z=WordLO2[x];
      ShiftLO(x);                    // Program LO2=3915 MHz with LD on Mux
      delay (pauseMillis);
    }
    MuxTest("LO2");                   // Now read the Mux Pin
    x = 6;
    z=WordLO2[x];
    ShiftLO(x);                      // LO2 Mux Off
    delay(pauseMillis);


    LD_PIN=LD3;
    dataPin=DA3;
    for (int x = 5; x >= 0; x--) {    // Program LO3=270MHz with LD on Mux      
      z=WordLO3[x]; 
      ShiftLO(x);                  
      delay (pauseMillis);
    }
    MuxTest("LO3");                 // Read the Mux Pin
    x = 6;
    z=WordLO3[x]; 
    ShiftLO(x);                    // LO3 Mux Off
    delay(pauseMillis);

  } // end for y
     pinMode(DA_ATT, INPUT);
     pinMode(CLK_ATT, INPUT);
}


void loop() {
  logAmp45=analogRead(A5);
  logAmp315=analogRead(A4);
  ncount=ncount+1;
  Serial.print("Hello, world   logAmp45= ");
  Serial.print(logAmp45);
  Serial.print("      logAmp315= ");
  Serial.println(logAmp315);
  delay(50);
}



void writeChip(int LD_PIN, int dataPin, int clockPin, unsigned long DATA)   {
  Serial.println();

  digitalWrite(LD_PIN, LOW);
 // shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, DATA);
  shiftOut(dataPin, clockPin, MSBFIRST, DATA >> 24);
  shiftOut(dataPin, clockPin, MSBFIRST, DATA >> 16);
  shiftOut(dataPin, clockPin, MSBFIRST, DATA >> 8);
  shiftOut(dataPin, clockPin, MSBFIRST, DATA);
        Serial.println (DATA);
  delayMicroseconds(1);
  digitalWrite(LD_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(LD_PIN, LOW);
  delayMicroseconds(50);
}



void ShiftAtten() {  //Send out 4 bytes then Latch
  digitalWrite(LD_ATT, LOW);
  shiftOut(DA_ATT, CLK_ATT, LSBFIRST, ByteA);
  delayMicroseconds(1);
  digitalWrite(LD_ATT, HIGH);
  delayMicroseconds(10);
  digitalWrite(LD_ATT, LOW);
  delayMicroseconds(50);
}



void ShiftLO(int x) {  //LO3 Send out 32 bits (4 x 8-Bit Bytes) then Latch//
  digitalWrite(LD_PIN, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, z>>24);
  shiftOut(dataPin, clockPin, MSBFIRST, z>>16);
  shiftOut(dataPin, clockPin, MSBFIRST, z>>8);
  shiftOut(dataPin, clockPin, MSBFIRST, z);
  delayMicroseconds(20);
  digitalWrite(LD_PIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(LD_PIN, LOW);
  delayMicroseconds(20);
  printMux = true;
}



void MuxTest(String chipName) {
  Mux = digitalRead(PLL_MUX);
  if (printMux) {
    Serial.print (chipName);
    if (Mux == HIGH) {
      Serial.print (" LOCK   ");
    }
    else {
      Serial.print (" UNLOCK   ");
    }
    printMux = false;
  }
}
