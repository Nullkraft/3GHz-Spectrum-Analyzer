'MAX2871_TestCom_2.bas for Spectrum Analyzer Freq Synthesis.
dim shared as ulong NI,x,y,z,r,Fract1,Fract2,MOD2,Reg(6,201),FpfdH,FchspH
dim shared as double Fvco,N,FracT,Num,Fchsp,Re,Fout
dim shared as string Regstr,Ans,Tx(6),ComStr,Ans1,Ans2,FchspStr,t
dim shared as string Frefstr,RefDivstr,Quote,delaystr,AnsT,Message
dim shared as integer div,Flag,Range,OldRange,delay,RefDiv,f
dim shared as single Fstart,Fstop,Finc,Fpfd,Fref
Flag=0


sub Transmit 'This routine initializes MAX2871 
print "First we initialize the MAX2871 (twice)   "
for z=0 to 1 'do the Reg 5-4-3-2-1-0 cycle twice
for y=0 to 5 : x=(5-y)
  write #1,Reg(x,0)
  sleep 100
next y
next z
Flag=1
end sub

sub TransmitF 'For writing to Register 0 only. Either frequency  
  write #1,Reg(0,f)
  sleep delay  
  Flag=1
 end sub


Setup:
screen 18 :cls
print "MAX2871_TestCom_2.bas by WN2A for Spectrum Analyzer Freq Synthesizer LO#2 AND #3"
print "Use with Arduino MAX2871_Load_Word_115200_1.ino"
print "This program initializes the 6 registers (5-4-3-2-1-0)twice"
print "Toggles 2 frequencies: 187.827839 [MHz] / 374.596154 [MHz]"
print "Refer to README_MAX2871_TestCom.bas.txt !!":print "Array Reg(f,x) used here"
input "Enter ttyACM Port 0,1 or 2 < 0 default> ";Ans1 : if Ans1="" then Ans1="0"
Ans2="115200"
ComStr="/dev/ttyACM"+Ans1+":"+Ans2+",N,8,1"
open com Comstr as #1
print : input #1 ,Message : print Message  :print 
input "Enter delay < 10 millseconds >";delaystr : if delaystr="" then delaystr="10"
delay=val(delaystr)

restore Datarow0
for x=0 to 5 : read Reg(x,0) :next 
restore Datarow1
for x=0 to 5 : read Reg(x,1) :next 

input "Select Mode: Toggle Manually or Automatically <a> ";AnsT
print "To Quit, press <q> then Enter"

Start:
if inkey$="q" then goto Sleepy
if AnsT="a" then goto Jump1 
input "Ready to Toggle? ; <q> to Quit "; Ans : if Ans="q" then goto Sleepy
print "Now Transmitting on 187.827839 MHz       ";

Jump1:
f=0
if Flag=1 then TransmitF else Transmit
if AnsT="a" then goto Jump2
input "Ready to Toggle? ; <q> to Quit "; Ans : if Ans="q" then goto Sleepy
print "Now Transmitting on 374.596154 MHz       ";
Jump2:
f=1
if Flag=1 then TransmitF else Transmit
goto Start

Sleepy:
close #1
sleep

system
Datarow0:
data   6565056,541097977,1476465218,4160782339,1674572284,2151677957 'R0>>R5 187.827839
Datarow1:
data  13093080,541097977,1476465218,4160782339,1674572284,2151677957 'R0>>R5 374.596154

