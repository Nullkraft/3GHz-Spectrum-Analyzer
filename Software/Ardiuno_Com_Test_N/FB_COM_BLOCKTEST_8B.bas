'FB_COM_BLOCKTEST_8B.bas for testing Binary USB Communication Laptop<>Arduino
' For Use with Arduino_Com_Test_8.ino

dim shared as string ModCom,portspeed,portnum,Message,ComStr
dim shared as ulong  Ans1,Ans2,Ans3,Ans4,Ans5,Ans6,Ans7,Ans8,Marray(8)
dim shared as integer h,w,N1,N2,N3,N4,N5,N6,N7,N8,Start,Total,x,Cycles
dim shared as single TperCycle,TperWord
declare sub Hello

' Unsigned short are 16 bit values
dim NArray(1 to 8) as Unsigned Short


' Begin execution of our program
Program_Start:
  screen 20 :  color 11,0
  print " FB_COM_BLOCKTEST_8B.bas by WN2A for Testing Binary Two Way-USB @ 1Megabaud"
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
  'open com ComStr as #1
  open com ComStr for Binary as #1
  print "Wait a few seconds...."
  sleep 4000  ' My machine absolutely requires this. Please don't remove for now.
  Hello

Loopy:
   for x=1 to 8 :read MArray(x) :next

  Start=timer

  for x=0 to (Cycles-1)
   put #1, , MArray() 
   Get #1, , NArray()
   print "Calc.Values are: ";MArray(1);" ";MArray(2);" ";MArray(3);" ";MArray(4);" ";MArray(5);" ";MArray(6);" ";MArray(7);" ";MArray(8)
  next

  Total=timer-Start : TperCycle=1000*Total/x :TperWord=TperCycle/8
  print "Total Time [seconds]= "; Total;"  Number of Cycles= ";x
  print "Number of Words/IntegerValues Sent/Received=8"
  print "Time per cycle[ms] =";TperCycle
  print "Time per Word/IntegerValue Transfer[ms]= ";TperWord

exitProg:

 ' close #1
  print
  print "Goodbye, Adios, Aloha, 73!"
  sleep 5000
  system

sub Hello
  MessageLoop: ' An optional section telling you the Arduino is communicating with the PC
  if inkey="q" then system
  input #1, Message
  if Message="" then goto MessageLoop
  print
  print " Arduino Message: ";Message
  print
  sleep 1000
end sub

data (2^31),(2^21),(2^24),(2^28),(2^26),(2^19),(2^26),(2^31)
















