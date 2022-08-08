'FB_CmdGen_ASCII_2.bas for testing USB Communication Laptop<>Arduino
' For Use with Arduino_Com_Check_2.ino

dim shared as string ModCom,portnum,Message,ComStr,portMode,Mode,Reply,quit,Verify,continuous,Radix,HexCmd,AnsStr
dim shared as string FreqStr,dBAttStr,XStr
dim shared as ulong  Cmd
dim shared as integer R,Ans,Div,F,M,N,BestF,BestM
dim shared as uinteger N1,x
dim shared as single pause,dBAtt,Fref,Fpfd,Freq,Fvco,minErrHz
dim shared as double Start,Total,TperCycle,minErr,Err1,Fract
declare sub TxCmd
declare sub FMN
Fref=60  'Default 60MHz Reference [MHz]
R=2      'Default Reference Divider

' Begin execution of our program
Program_Start:
'  screen 20
  color 11,0
  print " FB_CmdGen_ASCII_2.bas by WN2A for Testing Two Way-USB @ 1Megabaud (ASCII)"
  print " Communication PC<-->Arduino  Use with Arduino CmdProcessor_ASCII_2.bas"
'  print " You can select the device you want to control and any necessary parameters"
'  input " Enter ttyACM or ttyUSB Port 0,1,2, etc < 0 default> "; portNum : if portNum="" then portNum="0"
'  input " Enter portSpeed <1000000 default> ";portSpeed : if portSpeed="" then portspeed="1000000"
'  ComStr="/dev/ttyACM" + portNum + ":" + portSpeed + ",N,8,1"      ' NOT a colon separated command
'  input " Select Communication Device Class:  USB <u/U> or ACM <default> ";ModCom
'  if ModCom="u" or ModCom="U" then ComStr="/dev/ttyUSB" + portNum + ":" + portSpeed + ",N,8,1"      ' NOT a colon separated command

OpenPort:
'  ComStr="/dev/ttyUSB2" + ":" + portSpeed + ",N,8,1"      ' NOT a colon separated command
  dim as String port = Command(1)
  dim as String portSpeed = Command(2)
  ComStr = port + ":" + portSpeed + ",N,8,1"      ' NOT a colon separated command
  print " Serial port settings: " + ComStr
  open com ComStr as #1
        
  input "ATTENTION!! ***  OPEN and CLOSE Serial Monitor Now !! *** Enter milliseconds delay (optional) <0> is default ";pause
  print

  sleep 100 ' My machine absolutely requires this. Please don't remove for now.
  
  do
Main: 
  cls
  print "Select device from menu: "
  print "  0.......For Digital Attenuator"
  print "  1.......For LO1"
  print "  2.......For LO2"
  print "  3.......For LO3"
  print "  4.......For References <60MHz Default>"
  print "  5.......For PLL MUX (Provision)"
  print "  7.......For LED's and Miscellaneous"
  print "  8.......To Enter a Direct Command in Hexadecimal"
  print "  q.......To Quit"
  input "Enter your choice";AnsStr 
       if AnsStr="" then goto Main ' in case you hit enter by mistake. Happens a lot
       Ans = val(AnsStr)
  
 AnsString:
 Select Case AnsStr
    case "0" 'Digital Attenuator
    input "Enter dB Value 0-31.75dB in 0.25dB steps, <Enter> to goto Menu";dBAttStr
    if dBAttStr="" then goto Main
    dBAtt=val(dBAttStr) 
    HexCmd=Hex((65536)*(dBAtt*4)+255)
    TxCmd 
        
    case "1"  'LO1
    print "LO1 Frequency Control. Note: Select Reference Frequency from Menu>> #4..For References"
    input "Enter LO1 Frequency 3400-6800 [MHz], 30MHz Steps/30MHz Fpfd, Integer Synthesis <Enter> to goto Menu";FreqStr
    if FreqStr="" then goto Main
    Fpfd=Fref/R ' Note: Fref i set by Menu>> #4..For References. R is set as a constant at 2
    Freq=val(FreqStr) : Freq=(Freq/Fpfd) 
    HexCmd=Hex(Freq)+"11FF" 
    TxCmd
    print
         
    case "2"   'LO2
     print "LO2 Frequency Control. Note: Select Reference Frequency from Menu>> #4..For References"
    input "Enter LO2 Frequency [MHz] 3000-6000 MHz. Fractional Synthesis  <Enter> to goto Menu";FreqStr
    Div=1 'Output Divider Setting LO2
    if FreqStr="" then goto Main 
    HexCmd="000132FF"
    TxCmd  'send command Word for LO2
    Fvco=val(FreqStr)*(1) ' LO2 Output Divider =1
    Fpfd=Fref/R ' Note: Fref is set by Menu>> #4..For References. R is set as a constant at 2
    FMN  'call FMN subroutine, come back with BestF, BestM and N
    print "BestF =";BestF, "   BestM =";BestM," N =";N," Freq Error at Frf output [Hz]";cint(minErrHz/Div) 
    TxCmd 'Send Data Word 
    
      goto AnsString
      
    case "3"   'LO3
    print "LO3 Frequency Control. Note: Select Reference Frequency from Menu>> #4..For References"
    input "Enter LO3 Frequency [MHz] 270 or 360MHz. Fractional Synthesis.  <Enter> to goto Menu";FreqStr
    Div=16 'Output Divider Setting LO3
    if FreqStr="" then goto Main
    HexCmd="000133FF"
    TxCmd  'send command Word for LO3
    Fvco=(val(FreqStr))*(16) ' LO3 Output Divider =16
    Fpfd=Fref/R ' Note: Fref is set by Menu>> #4..For References. R is set as a constant at 2
    FMN  'call FMN subroutine, come back with BestF, BestM and N
    print "BestF= ";BestF; "   BestM= ";BestM;" N= ";N;" Freq Error at Frf output [Hz]";cint(minErrHz/Div) 
    TxCmd 'Send Data Word 
    
      goto AnsString
      
    case "4"   'References
        input "Enter<0> for no REF; <1> for REF1; <2> for REF2; <Enter> to goto Menu";XStr
         if XStr= "0" then 
         HexCmd="000004FF" : Fref=0
         endif           
         if XStr= "1" then 
         HexCmd="00000CFF" : Fref=60
         endif 
         
         if XStr= "2" then 
         HexCmd="000014FF" : Fref=100
         endif    
         if XStr= "" then goto Main
         TxCmd
      goto AnsString  
    case "5"   'PLL Mux   (Provision)
      print  "Work-In-Progress"  :sleep 2000
      goto Main
    case "7"   'LED's and Miscellaneous 
      input "Enter <1> to turn on LED <0> for off. <Enter> for Menu";XStr
         if XStr= "1" then HexCmd="00000FFF"  
         if XStr= "0" then HexCmd="000007FF"  
         if XStr= "" then goto Main
         TxCmd
      goto AnsString    
      
    case "8" ' Direct Commamd
            input "Enter Command in Hexadecimal (Full 8 Hex digits) or <enter> to quit ";HexCmd
            if HexCmd="" then goto ExitProg
			TxCmd
         
    case "q"
       goto ExitProg
    case else
        goto AnsString
    end select
     goto AnsString
     
   loop


sub	TxCmd   'Enter with HexCmd a string of Hex Chars 
	Cmd=val("&H"+HexCmd)	
    print "Value <Dec>= ";Cmd;" <Hex>= ";HexCmd
    print #1,  Cmd 'Ans is numerical up to ulong   
end sub 
  
  
sub FMN	'Enter Fvco; Exit with BestF,BestM and N
Fpfd=Fref/R : N=INT ((Fvco)/Fpfd) : Fract=(Fvco/Fpfd)-N
minErr=2^24
for M=2 to 4095
F=cint(Fract*M)
Err1=abs( Fvco-(Fpfd*(N+F/M)))

if Err1<minErr then
  minErr=Err1
  BestM=M
  BestF=F
  endif
next M

'if BestF=BestM, clear BestF,Make BestM=4095,Increment N
if  BestF=BestM then 
   BestF=0
   BestM=4095
   N=N+1
   endif
minErrHz=minErr*(1e6)
HexCmd=hex(BestF,3)+hex(BestM,3)+hex(N,2)
print 'HexCmd
end sub
 
  
   
ExitProg:
 
  close #1
  print  "Had Enough Fun???"
  print "Goodbye, Adios, Aloha, 73!"
  sleep 3000



















