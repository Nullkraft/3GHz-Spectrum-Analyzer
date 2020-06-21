//************************************************//
//  Name    : ShiftOutCode_Test_6A.ino                                      //
//  Author  : WN2A                                                //
//  Date    : 2 Jan 2020                                          //
//  Version : 1.0                                                 //
//  Notes   : Code for using ADF4356 Synth (14 Registers!)        //
//************************************************//
// Fixed Frequency=250 MHz, 46.5 MHz Reference. Codes for 14 total registers//
//            R0   R1   R2   R3   R4   R5   R6   R7   R8   R9   R10  R11  R12  R13//
int Byte0[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff};
int Byte1[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff};
int Byte2[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff};
int Byte3[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80};
//Pin connected to LE of ADF4356
int latchPin = A0;
//Pin connected to SCLK of ADF4356
int clockPin = A2;
////Pin connected to DATA of ADF4356
int dataPin = A1;
// Pin for strobe at start of tranmission
int strobe = A3;

void setup() {
    //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(strobe, OUTPUT);  
  Serial.begin(9600);
  Serial.println("ShiftOutCode_Test_6C.ino for ADF5356 First LO. 46.5MHz Ref/250MHz RF");
}
// for loop counts backwards from Reg 13 to Reg 0 //
void loop() {
  int x;
  delayMicroseconds(1000);
     digitalWrite (strobe, LOW);
      digitalWrite (strobe, HIGH);
       delayMicroseconds(1);
         digitalWrite (strobe, LOW);
 for (int x=13;x>=0;x--)
 
Shifty(x);

     
}
  void Shifty(int x) {  //Send out 4 bytes then Latch 
    
    digitalWrite(latchPin, LOW);
     shiftOut(dataPin, clockPin, MSBFIRST,Byte3[x]);
     shiftOut(dataPin, clockPin, MSBFIRST,Byte2[x]);
     shiftOut(dataPin, clockPin, MSBFIRST,Byte1[x]);
     shiftOut(dataPin, clockPin, MSBFIRST,Byte0[x]);

    delayMicroseconds(1); 
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(10);
        digitalWrite(latchPin, LOW);
  }
