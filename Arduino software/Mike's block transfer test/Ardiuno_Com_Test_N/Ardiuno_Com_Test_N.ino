
//************************************************
//  Name    : Arduino_Com_Test_N.ino
//  Author  : WN2A
//  Date    : 23 August 2021
//  Version : 1.0
//  Use with FreeBasic Program FB_COM_BLOCKTEST_8.bas
//************************************************
#define numWords 8                // Enter # of Words
unsigned long wordArray[numWords];
unsigned int root;
bool updateDisplay;
int wordIndex;


void setup() {
  Serial.begin(1000000);
  Serial.println("Arduino_Com_Test_N.ino 24 August 2021  1 Megabaud");
  wordIndex = 0;
  updateDisplay = false;
}

void loop() {
  if (Serial.available()) {
    wordArray[wordIndex] = Serial.parseInt();
    if (wordArray[wordIndex] > 0) {
      wordIndex++;
    }
    if (wordIndex > (numWords - 1))
      updateDisplay = true;
  }

  if (updateDisplay) {
    for (int i = 0; i < numWords; i++) {
      root = sqrt(wordArray[i]);                     // integer value to return
      Serial.print("Square root of ");
      Serial.print(wordArray[i]);
      Serial.print(" is ");
      Serial.println(root);
    }
    updateDisplay = false;
  }

}
