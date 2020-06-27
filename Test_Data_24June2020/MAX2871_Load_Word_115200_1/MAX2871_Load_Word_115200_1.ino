//************************************************
//  Name    : MAX2871_Load_Word_1.ino                 
//  Author  : WN2A                                
//  Date    : 19 June 2020                         
//  Version : 1.0                                
//  Notes   : Code for using MAX2871 Synth (1 Register at a time)
//  Use with FreeBasic Program MAX2871_TestCom_1.bas 
//************************************************

#define HIGH ON
#define LOW  0FF

unsigned long z;
unsigned long q;
int (x);
String readString;
int latchPin = 17;  // LE
int clockPin = 15;  // SCLK
int dataPin  = 16;  // DATA
int strobe   = 10;  // STROBE
int RF_En    =  5;  // Pin 5 controls MAX2871 RF output

// Wait without using delay()
unsigned long timer_start;

void setup() {
  Serial.begin(115200);

  // These 5 pins are used to control the MAX2871.
  pinMode(RF_En, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(strobe, OUTPUT);  

//  Serial.println("MAX2871_Load_Word_1.ino  19 June 2020 Takes U32 word from USB to SPI Bus ShiftOut");
//  Serial.println("For use with the PROJECT#3 Synthesizer @ 115200 Baud");
  digitalWrite(RF_En, ON);  // Turn on the MAX2871 RF Output
  delay(1);

  timer_start = micros();
}

void loop() {
  while (Serial.available()) {
    if ( (micros()-timer_start) > 1000) {
      char c = Serial.read();
      readString += c;
      timer_start = micros();
    }
  }
  
  if (readString.length() >0) {
    z=readString.toInt();
    readString=""; 
    q=z/65536;
    delayMicroseconds(10);
    digitalWrite (strobe, LOW);
    digitalWrite (strobe, HIGH);
    delayMicroseconds(10);
    digitalWrite (strobe, LOW);
    Shifty(x);
  }
}

//Send out (4) 8-Bit Bytes then Latch 
void Shifty(int x) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST,(q>>8)); 
  shiftOut(dataPin, clockPin, MSBFIRST,q); 
  shiftOut(dataPin, clockPin, MSBFIRST,(z>>8));
  shiftOut(dataPin, clockPin, MSBFIRST,z);
  delayMicroseconds(1); 
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(latchPin, LOW);
}
