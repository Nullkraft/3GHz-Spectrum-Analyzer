PROJECT#3 MAX2871 Synthesizer Tests 24June2020 WN2A

1) List of files in this folder:

   A) Arduino UNO R3 Code:  MAX2871_Load_Word_115200_1.ino
                    All tests reported here done with this program.
   
   B) FB Host Code:  Used for static (Manual) and dynamic (Toggling)
                   MAX2871_Load_Word_115200_1 (Linux Executable)
                   MAX2871_Load_Word_115200_1.bas  (FB source code)
                   
   C) FB Host Code: Used to Command a single frequency manually or sweep.
                   MAX2871_Command_3 (Linux Executable)
                   MAX2871_Command_3.bas (FB source code)
                   
   D) Screenshot of MAX2871_Command_3 running: MAX2871_Command_3_Window_1.png                
                   Note that the Arduino program sends an ID messgae to the Host 
                   program, after the Arduino is running.
                   Also, the sweep rate can be seen in this report as 
                   201 Points/Sweep x 204 sweeps= 41004 total sweeps, in 254 seconds
                   or ~161.4 points/second, or ~6.2 milliseconds/point.
                   That makes sense as the FB program was set for a 6 millisecond delay
                   resulting about the same rep rate. Setting the program for 5 or less
                   millisecond delay results is lost bytes. This as found to be a problem
                   most likely with the Arduino sketch, and the way it fetches serial
                   data. This results in limiting the sweep time. See comments (2B) below.
                   
   E) Oscilloscope trace of PROJECT#3 running with  A) and B).
                  File:  MAX2871_LockDet_AlternatingCode.jpg
                  This was the best case switching I observed, toggling between two
                  frequencies, about 3.5 milliseconds. Attempting to toggle any faster
                  than this resulted in breakup (lost bytes) from the Host to Arduino. 
                  This is related to the sweep time issue in D) . See note 2B.
                  
                   Both frequencies chosen in Fractional-N mode. Observing
                  either frequency (when manually i.e. statically toggled) looked clean
                  on my spectrum analyzer.  
                  
   F) Oscilloscope trace of PROJECT#3 running with  A) and C).   This is in the Frequency
                  Sweep mode, being driven by the FB program.
                  You can see the ~6.2 millisec rep rate. Setting the Host program for
                  5 or less millisec delay results in breakup.
                  
   G) MAX2871_1400-1410_201PTS_1.mpg. Video of the sweep mode A) and C). Sweeping 1400-1410MHz
                  201 points. 
                  
2) Notes:

   A) This part does give stable output measured on my Frequency Counter (23.5 to 500MHz+) and
      also viewed on an old HP8558B Spectrum Analyser to over 1500 MHz. Power was about +3dBm average.
      Spurious could not be measured accurately, due to equipment limitations, but looks better than
      -60 dBc.
      
   B) Both Laptops (Dell Mini Netbook and MacBook Pro) ran FreeBasic Host programs on Puppy Linux Slacko
      version 6.3.2. Both Laptops could deliver 115200 Baud Commands via the USB bus, with or without any
      delay added in the Host program. The Host program/Laptops showed no sign of breakup, and the Arduino
      Rx LED stays lit, regardless of the delay setting from 0 to 10 milliseconds.
        My suspicion is with my Arduino Sketch MAX2871_Load_Word_115200_1.ino (code snippet below):
        
        void loop() {
                while (Serial.available()) {  //wait for serial
              delayMicroseconds(1000);        // give it a millisec to fill buffer
              char c = Serial.read();         //read in a byte and convert to char
              readString += c;                // concatenate 
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
 Shifty(x);                                  // calls the Shift Out Routine
 }
}

          I commented out the serial receive lines above. While this code can receive each byte, and
      preform the byte>char>Integer conversions quickly and without any issues, the problem
      may be in identifying which is the first byte in a 32-bit word. The way this seems to work is that
      for a delay of ~5-6 milliseconds between words, it has no problem. The "blank time" easily
      allows for buffer clearing. Reducing this blank time to <5 milliseconds results in overwhelming
      the serial buffer (with this sketch code).
        To be sure, I do not use the Serial.print commands in this loop structure, and I do not enable 
      the serial monitor. That would have mde matters worse. Possibly, there is better Arduino code 
      that can be used here, so sweep times can be possibly improved toward ~3 milliseconds/point.
         The Shift Out Routine (Shifty(x)) was profiled on a scope and found to be plenty fast.
       More accurate measurements of this will be looked into, but the serial receive port code
       is much more suspect.
      
      
    C) Another little quirk noticed: In order for the Host Program to get control of the Arduino,
       this procedure was needed:
         a) Connect Arduino to Laptop via USB cable
         b) Open the Arduino IDE, check the Serial Port # (0,1,2,etc)
         c) Open the Serial Monitor. Now close the Serial Monitor and IDE.
         d) Run the FB program, either one B) or C) above.
         
         You didn't need to load any sketches or enter anything into the Serial Monitor,
         but it seemed weird that you need to open the IDE AND the Serial Monitor.
              
   D) Some thoughts about obtaining faster sweep rates by reducing USB communication and
       having the Arduino do the Fractional-N calculations.These are very computation heavy
       functions, being floating point and long unsigned data. It's very easy and efficent 
       done in FB code, because of a 32 (or 64 bit) operating system, 64 bit processor. This
       was verified in speed profiles, showing that the FB code was plenty fast. That was true
       on the Dell Mini Netbook also. 
         The addition of spur-reduction code will make computation more resource-hungry, both
         in program memory and speed. So while communication is faster when off-loading code
         to the Arduino, it will result in slower sweep times from a slower processor.
         It seems to be much easier to fix the Arduino sketch  1A) to get faster sweeps
         -OR- see if other Arduino boards have quicker serial periferials, keeping in mind
         periferial performance does not always track processor clock speeds.
         
Summary: From the standpoint of an inexpensive synthesizer ($11.53) with inexpensive Arduino controllers
          (~$9.95 or less ) providing a good 23.5-6000 MHz sweep generator with reasonable sweep
          times is very impressive. A slight improvement toward ~3 millsecond step time should make it very
          useful, especially for the PROJECT#1. 
          
          
          
 

       
                                       
                  
                  
                                            
          
                