'LO_CALC_1.bas LO Calculation Program for LO1,LO2 and LO3
dim shared as integer x,y,LO1,R,LOstart,Stoploop,BestLO1,IF1Low,IF1High
dim shared as single Fref1,Fref2,Fref,RF,RF1,RF2,RFstep,IF1,IF2,IF3,Fpfd1,Fpfd2,Fpfd,GuardBand
dim shared as single PfdHarmL,PfdHarmH,DeltaL,DeltaH,Delta,OldDelta,BestIF1
dim shared as double LO2,LO3
dim shared as string AnsStr,FilenameStr,Okay
dim shared as ubyte gg : gg=FreeFile
declare sub Rules
'declare sub HelpMe

Start:
screen 20 : color 15,1 :cls
print "LO_CALC_1.bas LO Calculation Program for LO1,LO2 and LO3"
print " The purpose of this program is to find viable LO frequencies for the RF Spectrum Analyser"
input " For Help, enter h, if not, just <enter> ";AnsStr 
    if AnsStr="h" then 
    #include "LO_CALC_1_Help.bi"  'its better not to junk up the main program with a Help File
    goto Start
    end if
print
input "Enter RF Start ,<1000> MHz default ";RF1 : if RF1=0 then RF1=1000
input "Enter RF Stop <2000> MHz  default ";RF2 :if RF2=0 then RF2=2000
input "Enter RF Frequency Step Size <0.25> MHz default";RFstep :if RFstep=0 then RFstep=0.25
input "Fref1 ,<60> MHz default ";Fref1 :if Fref1=0 then Fref1=60
input "Fref2 ,<100> MHz default ";Fref2 :if Fref2=0 then Fref2=100
input "Enter GuardBand <1.8> MHz default "; GuardBand  '315MHz Filter Rejection Points GuardBand
input "Enter IF1 Band-Pass Low End: <3500> Default :";IF1Low :if IF1Low=0 then IF1Low=3500
input "                   High End: <3700> Default :";IF1High :if IF1High=0 then IF1High=3700
print
if GuardBand=0 then GuardBand=1.8
input "The File to be saved will be called 'LO_Calcs_1.txt' unless you enter a new name";FilenameStr
if FilenameStr="" then FilenameStr="LO_Calcs_1.txt"
open "LO_Calcs_1.txt" for output as #gg
R=2: Fpfd1=Fref1/R : Fpfd2=Fref2/R

y=1 :RF=RF1
do until RF>RF2

Fpfd=Fpfd1
Rules 
print #gg, "#";y;tab(8);"RF=";RF;tab(20);"REF1: BestLO1=";BestLO1;" BestIF1=";BestIF1;tab(58);"Delta=";Delta;Okay;
if Delta>GuardBand then 
   print #gg,""
   goto JumpRef2
  end if 

Fpfd=Fpfd2
Rules
print #gg, tab(20);"REF2: BestLO1=";BestLO1;" BestIF1=";BestIF1;tab(58);"Delta=";Delta;Okay
'print " REF2: BestLO1=";BestLO1;" BestIF1=";BestIF1;" Delta=";Delta
JumpRef2:
y=y+1
RF=RF+RFstep
loop 

sub Rules 'enter RF,Fpfd. Find an LO1 and IF1 freq to meet criteria if possible
x=0 :Stoploop=0

do   
LOstart=x*Fpfd   
x=x+1
loop until LOstart>3400 'above IF1 PassBand frequency LOstart depends on Fpfd
'LOstart will typically be 3630 for Fpfd=30MHz and 3650 for Fpfd=50MHz. Dependent on Fpfd

x=0 :LO1=LOstart :OldDelta=0 :Delta=0
do until LO1>6800 or Stoploop=1
IF1=LO1-RF 
if (IF1<3300) or (IF1>3900) then goto GetAnother ' not valid IF1 freq. Out of IF1 Band

 PfdHarmL=Fpfd*int(IF1/Fpfd) :PfdHarmH= PfdHarmL+Fpfd
 DeltaL=abs(IF1-PfdHarmL)  : DeltaH=abs(PfdHarmH-IF1)

Delta=DeltaH  'preset
if DeltaL<Delta then Delta=DeltaL 'Delta now is the lessor(i.e. worst) of the two

if Delta>GuardBand and Delta>OldDelta then 
     OldDelta=Delta
     BestLO1=LO1
  end if  
BestIF1=BestLO1-RF    

GetAnother:
if LO1>6800 then Stoploop=1
LO1=LO1+Fpfd 'LO1 is Int-N , so add Fpfd to find next the frequency
loop
if Delta<Guardband then
      Okay="** NG **"
      else
      Okay=""
      end if
end sub 

   
EndProg:
close #gg
print "Bye!   Your File is named: ";FilenameStr

sleep
