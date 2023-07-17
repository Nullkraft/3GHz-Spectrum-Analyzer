v 20201216 2
N 30400 53500 30600 53500 4
N 30400 53500 30400 53300 4
C 33800 54700 1 0 0 3.3V-plus-1.sym
C 32800 54300 1 270 0 capacitor-1_small.sym
{
T 33500 54100 5 10 0 1 270 0 1
device=CAPACITOR
T 33700 54100 5 10 0 1 270 0 1
symversion=0.1
T 32800 54300 5 10 0 1 0 0 1
footprint=0402
T 33000 54100 5 10 1 1 0 0 1
refdes=C500
T 33000 53700 5 10 1 1 0 0 1
value=0.01 uF
T 32800 54300 5 10 0 1 0 0 1
man-part-num=885012205031
}
C 32800 53400 1 0 0 gnd-1.sym
C 30300 53000 1 0 0 gnd-1.sym
C 30600 53300 1 0 0 ASV_Series_Oscillator.sym
{
T 31200 54600 5 10 1 1 0 6 1
refdes=U500
T 30600 52800 5 10 0 1 0 0 1
man-part-num=EC2600TS-66.000MTR
T 30600 53300 5 10 0 1 0 0 1
device=OSCILLATOR
T 30900 53100 5 10 1 1 0 0 1
value=66.000 MHz
T 30600 53300 5 10 0 1 0 0 1
footprint=ASx_Series_Oscillator
}
N 28300 54300 28000 54300 4
{
T 27100 54300 5 10 1 1 0 0 1
netname=REF_EN1
}
N 30400 50800 30600 50800 4
N 30400 50800 30400 50600 4
C 32800 51600 1 270 0 capacitor-1_small.sym
{
T 33500 51400 5 10 0 1 270 0 1
device=CAPACITOR
T 33700 51400 5 10 0 1 270 0 1
symversion=0.1
T 32800 51600 5 10 0 1 0 0 1
footprint=0402
T 33000 51400 5 10 1 1 0 0 1
refdes=C501
T 33000 51000 5 10 1 1 0 0 1
value=0.01 uF
T 32800 51600 5 10 0 1 0 0 1
man-part-num=885012205031
}
C 32800 50700 1 0 0 gnd-1.sym
N 32300 51600 34000 51600 4
N 34000 51600 34000 52000 4
C 30300 50300 1 0 0 gnd-1.sym
C 30600 50600 1 0 0 ASV_Series_Oscillator.sym
{
T 31200 51900 5 10 1 1 0 6 1
refdes=U501
T 30500 50100 5 10 0 1 0 0 1
man-part-num=ASFL1-66.666HMZ-EC-T
T 30600 50600 5 10 0 1 0 0 1
device=OSCILLATOR
T 30900 50400 5 10 1 1 0 0 1
value=66.666 MHz
T 30600 50600 5 10 0 1 0 0 1
footprint=ASx_Series_Oscillator
}
N 28300 51600 28000 51600 4
{
T 27100 51600 5 10 1 1 0 0 1
netname=REF_EN2
}
N 32500 50300 35000 50300 4
N 35000 50300 35000 53000 4
C 29600 53400 1 90 0 resistor-1.sym
{
T 29200 53700 5 10 0 1 90 0 1
device=RESISTOR
T 29700 53900 5 10 1 1 0 0 1
refdes=R501
T 29600 53400 5 10 0 1 0 0 1
footprint=0402
T 29700 53600 5 10 1 1 0 0 1
value=1.5 K
T 29600 53400 5 10 0 1 0 0 1
man-part-num=CRCW04021K50FKEDC
}
C 29600 50700 1 90 0 resistor-1.sym
{
T 29200 51000 5 10 0 1 90 0 1
device=RESISTOR
T 29700 51200 5 10 1 1 0 0 1
refdes=R503
T 29600 50700 5 10 0 1 0 0 1
footprint=0402
T 29700 50900 5 10 1 1 0 0 1
value=1.5 K
T 29600 50700 5 10 0 1 0 0 1
man-part-num=CRCW04021K50FKEDC
}
C 29400 53100 1 0 0 gnd-1.sym
C 29400 50400 1 0 0 gnd-1.sym
N 32300 53500 32500 53500 4
N 32500 53500 32500 53000 4
N 32300 54300 34000 54300 4
N 32300 50800 32500 50800 4
N 32500 50800 32500 50300 4
N 34000 54300 34000 54700 4
C 33800 52000 1 0 0 3.3V-plus-1.sym
N 32500 53000 35500 53000 4
{
T 35600 53000 5 10 1 1 0 0 1
netname=REF_CLK
}
T 37000 53000 9 10 1 0 0 0 1
XX
T 26400 54300 9 10 1 0 0 0 1
C
L 36900 53100 36500 53100 3 0 0 0 -1 -1
L 26600 51700 27000 51700 3 0 0 0 -1 -1
L 26600 54400 27000 54400 3 0 0 0 -1 -1
V 37100 53100 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 26400 54400 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 26400 51700 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
T 26400 51600 9 10 1 0 0 0 1
D
C 29200 54400 1 180 0 resistor-1.sym
{
T 28900 54000 5 10 0 1 180 0 1
device=RESISTOR
T 28500 54500 5 10 1 1 0 0 1
refdes=R500
T 29200 54400 5 10 0 1 90 0 1
footprint=0402
T 28500 54000 5 10 1 1 0 0 1
value=1 K
T 29200 54400 5 10 0 1 90 0 1
man-part-num=CRCW04021K00FKEDC
}
C 29200 51700 1 180 0 resistor-1.sym
{
T 28900 51300 5 10 0 1 180 0 1
device=RESISTOR
T 28500 51800 5 10 1 1 0 0 1
refdes=R502
T 29200 51700 5 10 0 1 90 0 1
footprint=0402
T 28500 51300 5 10 1 1 0 0 1
value=1 K
T 29200 51700 5 10 0 1 90 0 1
man-part-num=CRCW04021K00FKEDC
}
N 29200 54300 30600 54300 4
N 29200 51600 30600 51600 4
