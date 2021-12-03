'ADF4356_4_Sweep.bas for Spectrum Analyzer Freq Synthesis.
dim shared as ulong NI,x,y,z,r,Fract1,Fract2,MOD2,Reg(14),Fract2A,MOD2A,FpfdH,FchspH,z1,z0
dim shared as double Fvco,Fpfd,N,FracT,Num,Fchsp,Re,Fout,Fref
dim shared as string Regstr,Ans,Tx(14),T6(14),ComStr,Ans1,Ans2,FchspStr,porttype,Message
dim shared as integer Div,Flag,Range,OldRange,Freqs,logAmp45,logAmp315
dim shared as integer step_number
dim shared as integer Mult,Sweep,Record,delay,cycle,StartTime,Total
dim shared as single Fstart,Fstop,Finc
declare sub Hello

Setup:
screen 19 :cls
print "ADF4356_4_Sweep.bas for Spectrum Analyzer Freq Synthesizer LO#1 3499-6500 MHz"
print " Supports INT and FRAC1 calculations. "
print "For ADF4356 Fout=3600-6630 MHz with Arduino ADF4356_LO1_1.ino"
print " First IF Frequency is set to 3630 MHz"
print "Refer to applicable README if available":print
input  "Enter tty Port 0,1 or 2,etc <0> default ";Ans1 : if Ans1="" then Ans1="0"
input  "Enter <u> for USB port, enter for ACM port";porttype 
if porttype="u" then porttype="USB" else porttype="ACM"
input "Enter Baud Rate such as 115200 or any legit baud rate <115200> default ";Ans2
if Ans2="" then Ans2="115200"
ComStr="/dev/tty"+porttype+Ans1+":"+Ans2+",N,8,1"  :print "Serial Settings:";Comstr :print
open com Comstr as #1
input "Enter millisec delay between words <1>";delay :if delay=0 then delay=1
Hello

input "Enter Fref in [MHz]  Register R=2 preset <60> MHz default";Fref  
if Fref=0 then Fref=60 : Fpfd=Fref/2 


Start:
Finc=0
input "Enter Manual Frequency <enter> or Sweep <1> ";Sweep
if Sweep=0 then
       Freqs=1 
       cls
       goto Repeat
       endif
input "Single Sweep <enter> or Multiple Sweeps <1>? ";Mult
input "Record Text file (.csv) of the Data? <1> or Enter? ";Record
if Record=1 then open "SweepData_1.txt" for output as #42 'Douglas Adams was here
cls
Repeat:   

input " Enter Manual or Start RF Frequency [MHz] "; Fstart 
if Fstart=0 then goto exitProg
Fstart=Fstart+3630  ' First IF Frequency is set to 3630 MHz
if Freqs=1 then goto Measure
input " Enter Stop RF Frequency [MHz] "; Fstop 
Fstop=Fstop+3630 'First IF Frequency is set to 3630 MHz
input " Enter Number of Frequencies <2-401> "; Freqs
Finc= (Fstop-Fstart)/(Freqs-1) 'invalid for Freqs=1, but that's not important.
StartTime=Timer

Measure:  

do

'for Fout=Fstart to Fstop step Finc
for step_number=0 to Freqs-1  : 
  Fout = Fstart + step_number * Finc
  Fvco=1*Fout :  N=Fvco/Fpfd 
  if Freqs=1 then 
       print "Fvco= ";Fvco;" Fout= ";Fout;" [MHz]";
       print "  N,Fvco,Fpfd=";N;"  ";Fvco;" ";Fpfd ;
       end if
  FpfdH=int (1e6)*Fpfd ' Convert to Hz
  NI=int(N) : FracT=N-NI : Num=FracT*(2^24) :Fract1=int ((2^24)*FracT)
  Re=((2^24)*(FracT))-Fract1 :x=FpfdH:y=FchspH

  Tx(0)=str(hex((NI*16)+2097152)) 
  Tx(1)=str(hex((1+(Fract1)*16))) 

  Transmit:
  cycle=1 'for cycle=0 to 1
  for y=12 to 13 : x=13-y  'This is dumb
  Reg(x)=Val("&H"+Tx(x))
  Regstr="0x"+Hex(Reg(x),8)
   if Freqs=1 then print "Register ";x,Reg(x),"  0x";Tx(x) 'Tx(x) is string
    write #1,Reg(x)   'Reg(x) is unsigned long 32-bit Uinteger
    sleep (delay)
         
    if inkey="q" then goto exitProg
  next y
       input #1,logAmp45,logAmp315  'Read LogAmps
   if cycle=1 then  print "RF Freq MHz= ";(Fout-3630);" logAmp45= ";logAmp45;" logAmp315= ";logAmp315
   if (Record=1) and (cycle=1) then print #42,(Fout-3630);",";logAmp45;",";logAmp315
   'next cycle
    sleep (delay)
next step_number

sleep (delay)
if Freqs=1 then
    goto Repeat
    end if
loop until (inkey="q") or (Mult=0)


sub Hello
  MessageLoop: ' An optional section telling you the Arduino is communicating with the PC
  if inkey="q" then system
  input #1, Message
  if Message="" then goto MessageLoop
  print " Arduino Message: ";Message
  do
  input #1,Message
  print Message
  loop until Message=""
  sleep 1000
end sub

exitProg:
   Total=(Timer-StartTime)
  print "Total Time (secs)= ";Total; "   minutes= ";Total/60
  if Record=1 then close #42
  print "Goodbye,Adios,Aloha,73!"
  close #1
  sleep 
system




