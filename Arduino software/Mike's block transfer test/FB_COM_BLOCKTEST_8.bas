'FB_COM_BLOCKTEST_8.bas for testing USB Communication Laptop<>Arduino
' For Use with Arduino_Com_Test_8.ino

dim shared as string ModCom,portspeed,portnum,Comstr,Message
dim shared as ulong  Ans1,Ans2,Ans3,Ans4,Ans5,Ans6,Ans7,Ans8
dim shared as integer h,w,N1,N2,N3,N4,N5,N6,N7,N8,Start,Total,x,Cycles
dim shared as integer serPort = 1
dim shared as single TperCycle,TperWord

' Begin execution of our program
Program_Start:
  screen 18 :  color 11,0 
  print " FB_COM_BLOCKTEST_8.bas by WN2A for Testing Two Way-USB @ 1Megabaud"
  print " Communication PC<>Arduino  Use with Arduino_Com_Test_X.ino"
  print " Enter numerical value here, Arduino calculates Integer Square Root"
  print " Use 'q' to terminate a process"
  input " Enter Number of Cycles to Run <5000 default>  ";Cycles :if Cycles=0 then Cycles=5000
  input " Enter ttyACM or ttyUSB Port 0,1,2, etc < 0 default> "; portNum : if portNum="" then portNum="0"
  portSpeed="1000000"
  ComStr="/dev/ttyACM" + portNum + ":" + portSpeed + ",N,8,1"      ' NOT a colon separated command
  input "Is Communication USB <u> or ACM <default> ";ModCom
  if ModCom="u" then ComStr="/dev/ttyUSB" + portNum + ":" + portSpeed + ",N,8,1"      ' NOT a colon separated command
  print ComStr
    ' Opening the serial port...
  open com ComStr as #serPort
MessageLoop: ' An optional section telling you the Arduino is communicating with the PC
  if inkey="q" then goto exitProg 
  input #serPort, Message
  if Message="" then goto MessageLoop
  print " Arduino Message: ";Message

Loopy:
   Ans1=(2^31) :Ans2=(2^29) :Ans3=(2^24) :Ans4=(2^28):Ans5=(2^26) :Ans6=(2^19) :Ans7=(2^26):Ans8=(2^31)    
   Start=timer

   for x=0 to (Cycles-1)
   print #1, Ans1,Ans2,Ans3,Ans4,Ans5,Ans6,Ans7,Ans8 'Ans is numerical up to ulong

  input #serPort,N1,N2,N3,N4,N5,N6,N7,N8
  print "Calc.Values are: ";N1;" ";N2;" ";N3;" ";N4;" ";N5;" ";N6;" ";N7;" ";N8
  next
   Total=timer-Start : TperCycle=1000*Total/x :TperWord=TperCycle/8
   print "Total Time [seconds]= "; Total;"  Number of Cycles= ";x
   print "Number of Words/IntegerValues Sent/Received=8"
   print "Time per cycle[ms] =";TperCycle
   print "Time per Word/IntegerValue Transfer[ms]= ";TperWord
exitProg:
  close #serPort
  print "Goodbye,Adios,Aloha,73!"
  sleep 


















