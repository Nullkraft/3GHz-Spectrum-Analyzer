'Table_Search_1.bas for data analysis
dim shared as double Freq,logAmp45,logAmp315,RefTest
dim shared as string Filename,Message
dim shared as integer Test,lowLimit,RefLine,ReadFile,SaveFile
dim shared as single dB45,dB315
screen 20
cls
Message=" Fractional Spur?"
print "Table_Search_1 for Data Analysis"
print "0dB reference is assigned to a A2D value of 501d, and scaled accordingly"
input "Enter Filename";Filename
input "Enter 0 dB reference  <509> default ";RefLine
if RefLine=0 then RefLine=509
input "Enter Low Limit criteria for spur <200> default";lowLimit
if lowLimit=0 then lowLimit=200
open Filename as #1
open "TableResults.csv" as #2
print #2,"Freq [MHz] ";" , ";"logAmp45";" , ";"dB 45";" , ";"log315";" , ";"dB 315";" , ";" Possible Source?"

do
Message=" Frac Spurs?"
input #1, Freq,logAmp45,logAmp315
Freq=(int(100*Freq+0.5))/100
' 40dB/volt; 204.8 counts/volt; 5.12 counts/dB
dB45=(logAmp45-RefLine)/5.12 : dB315=(logAmp315-RefLine)/5.12

if logAmp45<lowLimit and logAmp315<lowLimit then goto Jumpover

for Test=1 to 32  : RefTest=30/(2^Test)  'Test for SubHarmonics
if ((Freq/RefTest)-int(Freq/RefTest))=0 then Message="PFD SubHarmonic?"
next 

if ((Freq/30)-int(Freq/30))=0 then Message="PFD or PFD Harmonic?" 
if ((Freq/270)-int(Freq/270))=0 then Message="LO3 Harmonic?" 

print #2,Freq;" , ";logAmp45;" , ";dB45;" , ";logAmp315;" , ";dB315;" , ";Message

Jumpover:
loop until Freq=3000

close #1
close #2
sleep 1000
