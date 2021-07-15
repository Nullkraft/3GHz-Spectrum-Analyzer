
//************************************************//
//  Name    : Project1_Load_1000p0_9.ino        //
//  Author  : WN2A                                //
//  Date    : 26 June 2021                          //
//  Version : 3.0              //
//  Notes   : Code for Attenuator, (1)ADF4356 (2)MAX2871 LO2=3945MHz LO3=270MHz REF=60MHz PFD=5 MHz //
//            LO2 frequency=3945MHz Atten=16 dB  Serial=115200//
//            IF1=3630MHz IF2=315MHz IF3=45MHz //
//************************************************//

// 0x00=0dB   0x40=16dB   0x7F=31.75dB 
int ByteA={0x40}; //16 dB
// Fixed Frequency= MHz, 60 MHz Reference. Codes for 6 total registers//
//Digital Lock Detect Enabled, 3.2mA Charge Pump,10 sec Code Refresh//



//      LO1    R0   R1   R2   R3   R4   R5   R6   R7   R8   R9   R10  R11  R12  R13  R14  // RF=146.52 MHz LO1=3776.52MHz
// int Byte10[]={0xD0,0x21,0x02,0x03,0x84,0x25,0xF6,0xE7,0x68,0xC9,0xBA,0x0B,0xFC,0x0D,0x84};//
// int Byte11[]={0x07,0xDD,0x00,0x00,0x89,0x00,0x20,0x00,0x65,0xFC,0x0E,0x20,0x15,0x00,0x89};//CHG R6 TO 0x354120F6 for /8
// int Byte12[]={0x20,0x24,0x00,0x00,0x00,0x80,0x01,0x00,0x59,0x09,0xC0,0x61,0x00,0x00,0x00};
// int Byte13[]={0x00,0x0E,0x00,0x00,0x32,0x00,0x35,0x06,0x15,0x0F,0x00,0x00,0x00,0x00,0x02};
 // R4= 0x32008984 Dig Lockdet  or  R14=0x02008984 TriState
 
 
 
//      LO1    R0   R1   R2   R3   R4   R5   R6   R7   R8   R9   R10  R11  R12  R13  R14  // RF=1000.0 MHz LO1=4630MHz
int Byte10[]={0xA0,0x51,0x02,0x03,0x84,0x25,0xF6,0xE7,0x68,0xC9,0xBA,0x0B,0xFC,0x0D,0x84};//
int Byte11[]={0x09,0x55,0x00,0x00,0x89,0x00,0x20,0x00,0x65,0xFC,0x0E,0x20,0x15,0x00,0x89};//CHG R6 TO 0x354120F6 for /8
int Byte12[]={0x20,0x55,0x00,0x00,0x00,0x80,0x01,0x00,0x59,0x09,0xC0,0x61,0x00,0x00,0x00};
int Byte13[]={0x00,0x05,0x00,0x00,0x32,0x00,0x35,0x06,0x15,0x0F,0x00,0x00,0x00,0x00,0x02};
// R4= 0x32008984 Dig Lockdet  or  R14=0x02008984 TriState
 
 
 
 //      LO1    R0   R1   R2   R3   R4   R5   R6   R7   R8   R9   R10  R11  R12  R13  R14  // RF=3000.0 MHz LO1=6630MHz
// int Byte10[]={0xD0,0x01,0x02,0x03,0x84,0x25,0x56,0xE7,0x68,0xC9,0xBA,0x0B,0xFC,0x0D,0x84};//
// int Byte11[]={0x0D,0x00,0x00,0x00,0x89,0x00,0x20,0x00,0x65,0xFC,0x0E,0x20,0x15,0x00,0x89};//CHG R6 TO 0x354120F6 for /8
// int Byte12[]={0x20,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x59,0x09,0xC0,0x61,0x00,0x00,0x00};
// int Byte13[]={0x00,0x00,0x00,0x00,0x32,0x00,0x35,0x06,0x15,0x0F,0x00,0x00,0x00,0x00,0x02};

 
//      LO2    R0   R1   R2   R3   R4   R5   R6  //
int Byte20[]={0x00,0x11,0x42,0x03,0xFC,0x05,0x42}; // Tristate Mux
int Byte21[]={0x80,0x80,0x12,0x80,0xF1,0x00,0x12}; //now 3945 MHz optimal Fract;Tristate Mux Fpfd=5 MHz 
int Byte22[]={0x8A,0x00,0x03,0x00,0x8F,0x40,0x03};
int Byte23[]={0x01,0x20,0x58,0xF8,0x63,0x80,0x40};
// R2= 0x58011242 Dig Lockdet  or R6=0x40011242 TriState
 
 //      LO3    R0   R1   R2   R3   R4   R5   R6  //
// int Byte30[]={0x00,0x11,0x42,0x03,0x1C,0x05,0x42}; //270 MHz Opt Fract Opt;Dig Lockdet MuxOut Fpfd=5 MHz 
// int Byte31[]={0x00,0x80,0x12,0x80,0xF1,0x00,0x12};
// int Byte32[]={0xB0,0x00,0x03,0x00,0xCF,0x40,0x03};
// int Byte33[]={0x01,0x20,0x58,0xF8,0x63,0x80,0x40};
// R2= 0x58011242 Dig Lockdet  or R6=0x40011242 TriState


//      LO3    R0   R1   R2   R3   R4   R5   R6  //
 int Byte30[]={0x00,0x11,0x42,0x03,0x1C,0x05,0x42}; //270 MHz Opt Fract Opt;Dig Lockdet MuxOut Fpfd=15 MHz 
 int Byte31[]={0x00,0x80,0x12,0x80,0xF1,0x00,0x12};
 int Byte32[]={0x90,0x00,0x01,0x00,0xCF,0x40,0x01};
 int Byte33[]={0x00,0x20,0x58,0xF8,0x63,0x80,0x40};
// R2= 0x58011242 Dig Lockdet  or R6=0x40011242 TriState

//Signal to Pin assignments
int LD3 = 2;// Load LO3 of MAX2871
int DA3 = 3;// Data LO3 of MAX2871
int DA2 = 5;// Data LO2 of MAX2871
int LD2 = 4;// Load LO2 of MAX2871
int CLKPLL = 10; //common CLK_PLL
int REF_EN1=11;
int REF_EN2=12;
int Strobe = 13;  //temp for sync scope 
int DA1 = A0;//Data LO1 of ADF4356
int LD1 = A1;//Load LO1 of ADF4356
int PLL_MUX = A2;
int LD_ATT = A3;
int CLK_ATT = A4;
int DA_ATT = A5;
int ctr = 0;
int Mux;
int d=30; //delay

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
  Serial.println ("Project1_Load_1000p0_9.ino for Project#1 Spectrum Analyzer RF=1000MHz");
  Serial.println("Programs PE Digital Attenuator for 16 dB   IF1=3630MHz IF2=315MHz IF3=45MHz");
  Serial.println("Program L01 ADF4356 Synthesizer Vco=4630 MHz Ref=60 MHz PFD=5MHz?? (14 Registers)");  
  Serial.println("Program L02 MAX2871 Synthesizer Vco=3945 MHz Ref=60 MHz PFD=5MHz (6 Registers)");
  Serial.println("Program L03 MAX2871 Synthesizer Vco=270.000 MHz Ref=60 MHz PFD=5MHz (6 Registers)");
  Serial.println("This version supports Synth Lock Detect signal via Mux");
  Serial.println("Only LO3 is in loop, everything else in setup"); 

  // for loop counts backwards from Reg 5 to Reg 0 2X Per Spec Sheet //
  //int y; //to count 0 to 1
  //int x; //to count 5-4-3-2-1-0 or 14>>0

  ctr=ctr+1;
  Serial.print (" ctr = ");
  Serial.print (ctr);
  ShiftAtten();  // Program Attenuator
  
  delay(d);
  for (int y=2;y>=0;y--) {
    Serial.print (" Y counter= ");
     Serial.println (y);
  for (int x=13;x>=0;x--) {
  ShiftLO1(x);//Program LO1=4630 MHz with LD on Mux
  delay (d); 
  } 
  Serial.print ("   LO1");
  MuxTest();       //Now read the Mux Pin
  int x=14;
  ShiftLO1(x); //LO1 Mux Off
  delay(d); 
  
  for (int x=5;x>=0;x--) {
  ShiftLO2(x);//Program LO2=3915 MHz with LD on Mux
  delay (d);
  }  
  Serial.print ("LO2");
  MuxTest();       //Now read the Mux Pin
  x=6;
  ShiftLO2(x); //LO2 Mux Off
  delay(d);
  
  for (int x=5;x>=0;x--) {
  ShiftLO3(x);//Program LO3=270MHz with LD on Mux
  delay (100);  
  Serial.print ("LO3");
  MuxTest();       //Read the Mux Pin
  int x=6;
  ShiftLO3(x); //LO3 Mux Off
  }
  }
  }

  void loop() {

//for (int x=1;x>=0;x--) {
 // Serial.println (x);
//  ShiftLO3(x);//Program LO3=270MHz with LD on Mux
//  delay (d);  
//}
 // Serial.println ("LO3");
 // MuxTest();       //Read the Mux Pin
//  int x=6;
 // ShiftLO3(x); //LO3 Mux Off
    delay(2000);
  
  }

  void ShiftAtten() {  //Send out 4 bytes then Latch     
    digitalWrite(LD_ATT, LOW);               
    shiftOut(DA_ATT, CLK_ATT, LSBFIRST,ByteA);
    delayMicroseconds(1); 
        digitalWrite(LD_ATT, HIGH);
        delayMicroseconds(10);
        digitalWrite(LD_ATT, LOW);
        delayMicroseconds(50);  
  }
   
 void ShiftLO1(int x) {  //Send out 4 bytes then Latch 
     digitalWrite(LD1, LOW);
     shiftOut(DA1, CLKPLL, MSBFIRST,Byte13[x]);
     shiftOut(DA1, CLKPLL, MSBFIRST,Byte12[x]);
     shiftOut(DA1, CLKPLL, MSBFIRST,Byte11[x]);
     shiftOut(DA1, CLKPLL, MSBFIRST,Byte10[x]);
    delayMicroseconds(1); 
    digitalWrite(LD1, HIGH);
    delayMicroseconds(10);
        digitalWrite(LD1, LOW);
        delayMicroseconds(50);
  }
  
  void ShiftLO2(int x) {  //LO2 Send out 32 bits (4 x 8-Bit Bytes) then Latch//
     digitalWrite(LD2, LOW);
     shiftOut(DA2, CLKPLL, MSBFIRST,Byte23[x]);
     shiftOut(DA2, CLKPLL, MSBFIRST,Byte22[x]);
     shiftOut(DA2, CLKPLL, MSBFIRST,Byte21[x]);
     shiftOut(DA2, CLKPLL, MSBFIRST,Byte20[x]);
    delayMicroseconds(10); 
    digitalWrite(LD2, HIGH);
    delayMicroseconds(10);
        digitalWrite(LD2, LOW);
        delayMicroseconds(10);
  }

  void ShiftLO3(int x) {  //LO3 Send out 32 bits (4 x 8-Bit Bytes) then Latch//
     digitalWrite(LD3, LOW);
     shiftOut(DA3, CLKPLL, MSBFIRST,Byte33[x]);
     shiftOut(DA3, CLKPLL, MSBFIRST,Byte32[x]);
     shiftOut(DA3, CLKPLL, MSBFIRST,Byte31[x]);
     shiftOut(DA3, CLKPLL, MSBFIRST,Byte30[x]);
    delayMicroseconds(20); 
    digitalWrite(LD3, HIGH);
    delayMicroseconds(20);
        digitalWrite(LD3, LOW);
        delayMicroseconds(20);
  }
   
  void MuxTest() {
  Mux=digitalRead(PLL_MUX);
 if (Mux==HIGH) {
   Serial.print (" LOCK   ");
 }
 else {
   Serial.print (" UNLOCK   ");
 }
  }
  
