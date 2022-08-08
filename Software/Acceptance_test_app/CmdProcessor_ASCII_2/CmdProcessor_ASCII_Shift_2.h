// CmdProcessor_ASCII_Shift_2.h   Shift Out Routines


void ShiftAtten() {  //Send out 1 byte then Latch
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(LD_ATT, LOW);
  SPI.transfer(ByteA);
  digitalWrite(LD_ATT, HIGH);
  SPI.end();
}


// Program the selected LO (LO1, LO2 or LO3)
void ShiftLO(int x) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(LD_PIN, LOW);
  SPI.transfer(z>>24);
  SPI.transfer(z>>16);
  SPI.transfer(z>>8);
  SPI.transfer(z);
  digitalWrite(LD_PIN, HIGH);
  SPI.end();
 
  printMux = true;
}


void MuxTest(String chipName) {
  Mux = digitalRead(PLL_MUX);
  if (printMux) {
    Serial.print (chipName);
    if (Mux == HIGH) {
      Serial.println(" LOCK   ");
    }
    else {
      Serial.println(" UNLOCK   ");
    }
    printMux = false;
  }
}

/* Original ShiftOut functions

void ShiftAtten() {  //Send out 1 byte then Latch
  digitalWrite(LD_ATT, LOW);
  shiftOut(dataPin,clockPin, LSBFIRST, ByteA);
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

*/
