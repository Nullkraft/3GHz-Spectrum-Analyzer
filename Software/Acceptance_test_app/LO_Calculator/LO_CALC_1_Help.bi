' Include file for LO_CALC_1.bas

screen 20 : color 15,2 :cls
print "Welcome to the LO_CALC_1.bas Help File" :print
print "BASIC USE:"
print tab(5);"1) After starting program, enter (or use defaults) for the Start,Stop frequencies, and the Step Size"
print tab(5);"2) Next is to enter REF1 and REF2 frequencies, or use defaults. REF1 will be used first in calculations, then REF2"
print tab(5);"3) The Guardband has a default of 1.8 MHz to reflect the 315 MHz filter Rejection BW"
print tab(5);"4) The usable IF1 BW is next, entered as low end and high end frequencies. <3300> and <3900> is widest available"
print tab(5);"   But, for the purposes of this program, the defaults lead to the same conclusions"
print tab(5);"5) You can change the saved filename, or go with default"
print
print "NOTES and OBSERVATIONS"
print tab(5);"1) For a given RF Frequency, the program tries to find a good combination of RF,LO1 and IF1 so that" 
print tab(5);"   Phase-Frequency Detector Harmonics do not fall into IF1's PassBand and cause de-sense."
print tab(5);"2) The program output is a text file with the data annotated, for example:"
print tab(5);" # 1    RF= 1000    REF1: BestLO1= 4320 BestIF1= 3320     Delta= 10"
print tab(5);" # 2    RF= 1000.25 REF1: BestLO1= 4320 BestIF1= 3319.75  Delta= 10.25"
print tab(5);" # 3    RF= 1000.5  REF1: BestLO1= 4320 BestIF1= 3319.5   Delta= 10.5"
print tab(5);" # 4    RF= 1000.75 REF1: BestLO1= 4320 BestIF1= 3319.25  Delta= 10.75" 
print tab(5);"Here REF1 provides good LO1/IF1 candidates intially, no need to switch to REF2, yet....but" : ?
print tab(5);"3) We find further along, a range of RF frequencies where REF2 is needed:"
print tab(5);"# 74   RF= 1018.25 REF1: BestLO1= 4320 BestIF1= 3301.75  Delta= 1.75** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3331.75  Delta= 18.25"
print tab(5);"# 75   RF= 1018.5  REF1: BestLO1= 4350 BestIF1= 3331.5   Delta= 1.5** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3331.5   Delta= 18.5"
print tab(5);"# 76   RF= 1018.75 REF1: BestLO1= 4350 BestIF1= 3331.25  Delta= 1.25** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3331.25  Delta= 18.75"
print tab(5);"# 77   RF= 1019    REF1: BestLO1= 4350 BestIF1= 3331     Delta= 1** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3331     Delta= 19"
print tab(5);"4) So here the use of REF2 becomes valuable,Repeatedly, as it allows us to avoid IF1 de-sense. But if we look further "
print tab(5);"# 194  RF= 1048.25 REF1: BestLO1= 4350 BestIF1= 3301.75  Delta= 1.75** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3301.75  Delta= 1.75** NG **"
print tab(5);"# 195  RF= 1048.5  REF1: BestLO1= 4350 BestIF1= 3301.5   Delta= 1.5** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3301.5   Delta= 1.5** NG **"
print tab(5);"# 196  RF= 1048.75 REF1: BestLO1= 4350 BestIF1= 3301.25  Delta= 1.25** NG **"
print tab(5);"                   REF2: BestLO1= 4350 BestIF1= 3301.25  Delta= 1.25** NG **"
print tab(5);" Now, much less frequently, but just as important, we find RF frequencies where neither REF1 or REF2 are no good.":?
print "CONCLUSIONS:"
print tab(5);"1) Most RF frequencies can be accomadated just by REF1 (or REF2)"
print tab(5);"2) When the REF1 doesn't work, a simple solution is to use REF2. It is simple as it requires only Specific Commands"
print tab(5);"   (or Instructions) to be used. It does require all three LO's to be reprogrammed, but only short-term"
print tab(5);"3) Less frequently, neither REF1 nor REF2 works. In this case, changing the values in the Reference Dividers in the LO's"
print tab(5);"   from R=2 to R=3 could work, but this requires additional programming with General Commands, at least for the near-term," 
print tab(5);"   until new Specific Commands are potentially assigned. In any case, the existing General Commands could be used for bench"
print tab(5);"   validation of this method, before committing to any changes to the Interface Standard."  :?
input" Okay??";AnsStr 

