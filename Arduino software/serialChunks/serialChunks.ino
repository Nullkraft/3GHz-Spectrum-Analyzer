/*  To reach max serial speeds it is necessary to write more than a single byte
 * at a time.  A chunk of 32 bytes is transferred as a block.  By experimention
 * it was found that a 32 byte chunk provides the greatest improvement in the
 * data transfer rate.
 */



uint32_t dataBuff;                            // 32 bit access to buffered serial data.
byte* bytePtrDataBuff = (byte *) &dataBuff;   // Buffered serial data to write to SPI bus.


const int szChunk = 32;
byte numChunks = 0;
byte numBlocks;
byte chunk[szChunk];

byte commandByte;             // Don't use char as Serial.read() will treat it as 7 bits only

unsigned int chunkIndex = 0;  // Track which chunk is being used
byte frequencyInHz[3];

#define EOS          0xFF  // 0xFF is reserved for End Of Serial transmission (EOS)

unsigned int i;
unsigned int garbage;
#define sz_garbage 1124
byte reallyBigArray[sz_garbage];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(2000000);
  Serial.setTimeout(5);
}



void loop() {
  int hwAddr;

  for (i=0; i<sz_garbage; i++) {
    reallyBigArray[i] = 42;
  }
  reallyBigArray[sz_garbage - 1] += 1;
//  for (i=0; i<sz_garbage; i++) {
//    garbage = reallyBigArray[i] + 1;
//  }
  
  if (Serial.available() > 0) {
    
    commandByte = Serial.read();

    switch(commandByte) {
      case '1': case '2': case '3': case '4': case '5':  case '6':
        numBlocks = commandByte - '0';       // Convert commandByte from char to byte
        numChunks = numBlocks * 512/szChunk;
        chunkIndex = numChunks;
        break;
      case 'l':
        digitalWrite(LED_BUILTIN, LOW);    // Turn off built-in LED
        break;
      case 'L':
        digitalWrite(LED_BUILTIN, HIGH);   // Turn on built-in LED
        break;
      default:
        break;
    }
  }

  while (chunkIndex > 0) {
    hostWrite();
    chunkIndex--;
  }
}


/* You must fill the chunk array with data before calling this function.
 *
 * Send the End-Of-Stream marker 0xFF/0xFF after all the other chunks. It's
 * a double byte of 0xFF because the A/D uses less than 16 bits. That means
 * the full-scale value of the A/D (0xFF/0xC0) is less than the EOS marker.
 */
void hostWrite() {
  if(Serial.availableForWrite() > 0) {
    Serial.write(chunk, szChunk);
  }
  if (chunkIndex == 0) {
    Serial.write(EOS);
    Serial.write(EOS);
  }
}
