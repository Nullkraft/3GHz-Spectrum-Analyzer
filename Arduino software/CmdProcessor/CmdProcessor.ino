
#define szSerialWord 4    // Size in number of bytes
size_t numBytesRead;

uint32_t serialWord;                            // Serial word as a single 32 bits
byte* serialWordArray = (byte *) &serialWord;   // Serial word as an array of bytes

bool newCommandWord = false;


// 32 bit Control Word slices
unsigned short Data;        // 16 bits
byte Command;
byte CommandBits = 0xF8;    // Mask to select 5 bits of Command from serialWord[1]
byte Address;
byte AddressBits = 0x07;    // Mask to select 3 bits of Address from serialWord[1]
byte CommandFlag = 0xFF;

// Addresses for selecting the hardware (chip).
const int Attenuator = 0;
short AttenuatorBits = 0x007F;    // Mask to select 7 bits of Attenuator Embedded Data
const int LO1 = 1;
const int LO2 = 2;
const int LO3 = 3;
const int RefClock = 4;
const int PLL_Mux = 5;

// List of command names as found in the API Spreadsheet
const int RF_off = 1;
const int neg_4dBm = 2;
const int neg_1dBm = 3;
const int pos_2dBm = 4;
const int pos_5dBm = 5;
const int No_change = 6;



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.setTimeout(200);
  Serial.begin(2000000);
}




void loop() {
  while (Serial.available()) {
    numBytesRead = Serial.readBytes(serialWordArray, szSerialWord);

    /*
     * Here we parse the 32 bit control word into its constituent components
     * and set a flag (newCommandWord) to allow Hardware selection to begin.
     */
    if (numBytesRead == szSerialWord) {
      if (serialWordArray[0] == CommandFlag) {
        Data = serialWord >> 16;
        Command = (serialWordArray[1] & CommandBits) >> 3;
        Address = serialWordArray[1] & AddressBits;
      }
      newCommandWord = true;
    }
  } // End of While Serial available


  /*
   * Hardware selection and operations.
   */
  if (newCommandWord) {
    printCommandWord();


   /*
    * Start by selecting the device that you want to control. Then under
    * each device you can select the operation that you want to perform.
    */
    switch(Address) {
      case Attenuator:
        // Data bitmask for Attenuator - TODO: Create a constant for the mask
        Data &= AttenuatorBits;
        Serial.print("Digital Attenuator selected and its command is set to ");
        Serial.println(Data, HEX);
        break;

      case LO1:
        Serial.println("LO1 address selected");
        switch(Command) {
          case RF_off:
            Serial.println("Turn off LO1 RFout");
            break;
          case neg_4dBm:
            Serial.println("Setting LO1 to -4dBm absolute");
            break;
          case neg_1dBm:
            Serial.println("Setting LO1 to -1dBm absolute");
            break;
          case pos_2dBm:
            Serial.println("Setting LO1 to +2dBm absolute");
            break;
          case pos_5dBm:
            Serial.println("Setting LO1 to +5dBm absolute");
            break;
          case No_change:
            Serial.println("Setting LO1 to NO POWER CHANGE");
            break;
          default:
            // turn off LO1 RFout
            break;
        }
        break;
      
      case LO2:
        Serial.println("LO2 address selected");
        // Data bitmask for LO2 - TODO: Create a constant for the mask
        Data &= 0x0F;
        Serial.print("LO2 Data = ");
        Serial.println(Data, HEX);
        switch(Command) {
          case RF_off:
            Serial.println("Turn off LO2 RFout");
            break;
          case neg_4dBm:
            Serial.println("Setting LO2 to -4dBm absolute");
            break;
          case neg_1dBm:
            Serial.println("Setting LO2 to -1dBm absolute");
            break;
          case pos_2dBm:
            Serial.println("Setting LO2 to +2dBm absolute");
            break;
          case pos_5dBm:
            Serial.println("Setting LO2 to +5dBm absolute");
            break;
          case No_change:
            Serial.println("Setting LO2 to NO POWER CHANGE");
            break;
          default:
            // turn off LO2 RFout
            break;
        }
        break;
      
      case LO3:
        Serial.println("LO3 address selected");
        // Data bitmask for LO3 - TODO: Create a constant for the mask
        Data &= 0x0F;
        Serial.print("LO3 Data = ");
        Serial.println(Data, HEX);
        switch(Command) {
          case RF_off:
            Serial.println("Turn off LO3 RFout");
            break;
          case neg_4dBm:
            Serial.println("Setting LO3 to -4dBm absolute");
            break;
          case neg_1dBm:
            Serial.println("Setting LO3 to -1dBm absolute");
            break;
          case pos_2dBm:
            Serial.println("Setting LO3 to +2dBm absolute");
            break;
          case pos_5dBm:
            Serial.println("Setting LO3 to +5dBm absolute");
            break;
          case No_change:
            Serial.println("Setting LO3 to NO POWER CHANGE");
            break;
          default:
            // turn off LO3 RFout
            break;
        }
        break;
      
      case RefClock:
        Serial.println("Reference clock address selected");
        switch(Command) {
          case 1:
            Serial.println("Using 60 MHz Ref Clock");
            break;
          case 2:
            Serial.println("Using 100 MHz Ref Clock");
            break;
          default:
            Serial.println("Not using a Ref Clock");
            break;
        }
        break;
      
      case PLL_Mux:
        Serial.println("PLL Mux address selected");
        break;
      default:
        break;
    }
    
    newCommandWord = false;
  }
  
}


/*
 * Operate the digital attenuator
 */
int digitalAttenuator(byte command, byte address) {
  
}





/*
    I wanted to get this out of the the loop so the loop would be
    more concise and only demonstrate receiving a new command.
*/
void printCommandWord() {
  Serial.println();
  Serial.print("Data = ");
  Serial.println(Data, HEX);
  Serial.print("Command = ");
  Serial.println(Command, HEX);
  Serial.print("Address = ");
  Serial.println(Address, HEX);
}
