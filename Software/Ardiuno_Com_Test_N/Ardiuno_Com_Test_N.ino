
//************************************************
//  Name    : Arduino_Com_Test_N.ino
//  Author  : WN2A 
//  Date    : 23 August 2021
//  Version : 1.0
//  Use with FreeBasic Program FB_COM_BLOCKTEST_8.bas
//************************************************
unsigned long z[8];
unsigned int s;      
int Words=8; // Enter # of Words
int w;
int q;
int ctr=0; 
void setup() {
  Serial.begin(1000000);
  Serial.println("Arduino_Com_Test_N.ino 24 August 2021  1 Megabaud");
  w=Words-1;
}

void loop() {
  for (int x=0; x<=w ; x++) {
  if (Serial.available()) {
    z[x] = Serial.parseInt();
  }
  }
  
  q=1;
  for (int x =0; x<=w ; x++) {
  if (z[x] = 0 ) {
   q=0; 
  }
  }
  
  if ( q == 1 ) {
   for (int x =0; x<=w ; x++) {
    s=sqrt(z[x]);  //integer value to return
    ctr=ctr+1;
    s=s+ctr;
    Serial.println(s);
  }
}

}


   
