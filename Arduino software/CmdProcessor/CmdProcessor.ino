
size_t numBytesRead;

#define szSerialWord 4    // Size in number of bytes
uint32_t serialWord;                            // Serial word as a single 32 bits
byte* serialWordArray = (byte *) &serialWord;   // Serial word as an array of bytes

uint32_t spiWord;   // This holds the bit masked & shifted value from serialWord

bool newCommandWord = false;

int miso_pin;
int mosi_pin;
int spi_clock;
int spi_select;


/*** 32 bit CONTROL WORD and MASKING BITS ***/
unsigned short Data;        // 16 bits
byte Command;
byte CommandBits = 0xF8;    // Mask to select 5 bits of Command from serialWord[1]
byte Address;
byte AddressBits = 0x07;    // Mask to select 3 bits of Address from serialWord[1]
byte CommandFlag = 0xFF;    // Byte pattern to identify a Control Word


/*********** HARDWARE DEFINITIONS ***********/
// Pin assignments for Version 2 of the circuit board
int REF1_SEL = 11;
int REF2_SEL = 12;

int ATTEN_SEL = A3;
int ATTEN_MOSI = A5;
int ATTEN_CLOCK = A4;

int LO1_SEL = A1;
int LO1_MOSI = A0;

int LO2_SEL = 5;
int LO2_MOSI = 4;

int LO3_SEL = 2;
int LO3_MOSI = 3;

int PLL_MUX_MISO = A2;

// Common SPI clock for the LO's and Attenuator
int SPI_CLOCK = 10;

// Addresses for selecting the various hardware ICs.
const int Attenuator  = 0;
short ATTEN_Bits = 0x007F;      // 7 bits of Embedded Data
const int LO1         = 1;
const int LO2         = 2;
short LO2_Bits     = 0x0F;      // 4 bits of Embedded Data
const int LO3         = 3;
short LO3_Bits     = 0x0F;      // 4 bits of Embedded Data
const int RefClock    = 4;
const int PLL_Mux     = 5;
const int Reserved    = 6;
const int LED         = 7;

// Command names for operating LO1, LO2 and LO3
const int General   = 0;
const int RF_off    = 1;
const int neg_4dBm  = 2;
const int neg_1dBm  = 3;
const int pos_2dBm  = 4;
const int pos_5dBm  = 5;
const int No_change = 6;

// Command names for operating the reference oscillators
const int all_off  = 0;
const int sel_ref1 = 1;
const int sel_ref2 = 2;

// Command names for operating the PLL Mux
const int TRI_STATE    =  0;
const int D_VDD        =  1;
const int D_GND        =  2;
const int R_DIV_OUT    =  3;
const int N_DIV_OUT    =  4;
const int ANA_LOCK_DET =  5;
const int DIG_LOCK_DET =  6;
const int SYNC_INPUT   =  7;
const int READ_REG_6   = 12;  // 0xC

// Command names for operating the Arduino builtin LED
const int LED_off = 0;
const int LED_on  = 1;

/*********** HARDWARE DEFINITIONS END *******/






void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(miso_pin, OUTPUT);
  pinMode(mosi_pin, INPUT);
  pinMode(spi_clock, OUTPUT);
  pinMode(spi_select, OUTPUT);

  Serial.setTimeout(200);
  Serial.begin(2000000);
}




void loop()
{
  while (Serial.available())
  {
    numBytesRead = Serial.readBytes(serialWordArray, szSerialWord);
    /* Check for a Command Word and parse into Data, Command and Address */
    if (serialWordArray[0] == CommandFlag)
    {
      Data = serialWord >> 16;
      Command = (serialWordArray[1] & CommandBits) >> 3;
      Address = serialWordArray[1] & AddressBits;
      newCommandWord = true;
    }
    else if (numBytesRead > 0)
    {
      /* TODO: Buffer Directdata Words */
      spi_write();    // Write Directdata Word to the selected device
    }
  } // End While


  // Hardware selection and operations.
  if (newCommandWord) {

    // This line is for testing only - Remove at your earliest convenience.
    printCommandWord();

    // Start by selecting the device that you want to control. Then under
    // each device you can select the operation that you want to perform.
    switch (Address) {
      case Attenuator:
        mosi_pin = ATTEN_MOSI;
        spi_clock = ATTEN_CLOCK;
        spi_select = ATTEN_SEL;
        Data &= ATTEN_Bits;
        
        Serial.print("Digital Attenuator selected and its command is set to ");
        Serial.println(Data, HEX);
        break;

      case LO1:
        Serial.println("LO1 address selected");
        mosi_pin = LO1_MOSI;
        spi_clock = SPI_CLOCK;
        spi_select = LO1_SEL;
        switch (Command) {
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
          default:
            Serial.println("Setting LO1 to NO POWER CHANGE");
            break;
        }
        break;

      case LO2:
        Serial.println("LO2 address selected");
        mosi_pin = LO2_MOSI;
        spi_clock = SPI_CLOCK;
        spi_select = LO2_SEL;
        Data &= LO2_Bits;
        Serial.print("LO2 Data = ");
        Serial.println(Data, HEX);
        switch (Command) {
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
          default:
            Serial.println("Setting LO2 to NO POWER CHANGE");
            break;
        }
        break;

      case LO3:
        Serial.println("LO3 address selected");
        mosi_pin = LO3_MOSI;
        spi_clock = SPI_CLOCK;
        spi_select = LO3_SEL;
        Data &= LO3_Bits;
        Serial.print("LO3 Data = ");
        Serial.println(Data, HEX);
        switch (Command) {
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
          default:
            Serial.println("Setting LO3 to NO POWER CHANGE");
            break;
        }
        break;

      case RefClock:
        Serial.println("Reference clock address selected");
        switch (Command) {
          case sel_ref1:
            digitalWrite(REF1_SEL, HIGH);
            digitalWrite(REF2_SEL, LOW);
            Serial.println("Using 60 MHz Ref Clock");
            break;
          case sel_ref2:
            digitalWrite(REF1_SEL, LOW);
            digitalWrite(REF2_SEL, HIGH);
            Serial.println("Using 100 MHz Ref Clock");
            break;
          default:
            digitalWrite(REF1_SEL, LOW);
            digitalWrite(REF2_SEL, LOW);
            Serial.println("Ref Clocks disabled");
            break;
        }
        break;

      // To use the PLL Mux you must first set the selection pin for one of the LO's.
      // Once that's done you send the Command for READ_REG_6, do an spi_read() to
      // get the register's contents, then send the Command TRI_STATE before moving
      // on to the next LO.
      case PLL_Mux:
        Serial.println("PLL Mux address selected");
        switch (Command) {
          case READ_REG_6:
            Serial.println("Set Mux pin to Read Register 6");
            break;
          default:
            Serial.println("Set Mux pin to Tri-state output");
            break;
        }
        // TODO: Add spi_read()
        break;


      // Not part of the Interface Standard - For in-house testing only.
      case LED:
        Serial.println("LED address selected");
        switch (Command) {
          case LED_off:
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("Builtin LED Off");
            break;
          case LED_on:
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("Builtin LED On");
            break;
          default:
            Serial.println("Invalid LED command");
            break;
        }
        break;
        
      default:
        Serial.println("Invalid or unused hardware device address");
        break;
    }   /* End switch(Address) */

    newCommandWord = false;
  }  /* End newCommandWord */
}  /* End loop() */


// Program the selected device using shiftOut()
void spi_write() {
  for (int i = 0; i < szSerialWord; i++) {
    shiftOut(mosi_pin, spi_clock, MSBFIRST, serialWordArray[i]);
  }
  digitalWrite(spi_select, HIGH);
  digitalWrite(spi_select, LOW);
}


// I wanted to get this out of the the loop so the loop would be
// more concise and only demonstrate receiving a new command.
void printCommandWord() {
  Serial.println();
  Serial.print("Data = ");
  Serial.println(Data, HEX);
  Serial.print("Command = ");
  Serial.println(Command, HEX);
  Serial.print("Address = ");
  Serial.println(Address, HEX);
}
