
#define szSerialWord 4    // Size in number of bytes
size_t numBytesRead;

uint32_t serialWord;                            // Serial word as a single 32 bits
byte* serialWordArray = (byte *) &serialWord;   // Serial word as an array of bytes

unsigned short Data;        // 16 bits
byte Command;
byte CommandBits = 0xF8;
byte Address;
byte AddressBits = 0x07;
byte CommandFlag = 0xFF;

bool newCommandWord = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.setTimeout(200);
  Serial.begin(2000000);
}

void loop() {
  while (Serial.available()) {
    numBytesRead = Serial.readBytes(serialWordArray, szSerialWord);

    if (numBytesRead == szSerialWord) {
      if (serialWordArray[3] == CommandFlag) {
        Data = serialWord >> 16;
        Command = (serialWordArray[2] & CommandBits) >> 3;
        Address = serialWordArray[2] & AddressBits;
      }
      newCommandWord = true;
    }
  } // End of While Serial available


  // Only when a new Command Word is received does this do anything.
  if (newCommandWord) {
  /*    _________________
   ****|                 |****
   ****|  Your Code for  |****
   ****|  Data, Command  |****
   ****|   and Address   |****
   ****|    goes here    |****
   ****|_________________|****/
    printCommandWord();
    newCommandWord = false;
  }
  
}







/*
    I wanted to get this out of the the loop so the loop would be
    more concise and only demonstrate receiving a new command.
*/
void printCommandWord() {
  Serial.print("Data = ");
  Serial.println(Data, HEX);
  Serial.print("Command = ");
  Serial.println(Command, HEX);
  Serial.print("Address = ");
  Serial.println(Address, HEX);
}
