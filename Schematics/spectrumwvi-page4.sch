v 20130925 2
N 25900 58600 27400 58600 4
{
T 26400 58700 5 10 1 1 0 0 1
netname=REF_EN1
}
C 26700 60900 1 90 0 gnd-1.sym
N 33700 59000 31900 59000 4
{
T 32800 59100 5 10 1 1 0 0 1
netname=SPI_MOSI
}
N 27400 59000 25900 59000 4
{
T 26400 59100 5 10 1 1 0 0 1
netname=REF_EN2
}
N 21900 55600 23400 55600 4
{
T 22000 55700 5 10 1 1 0 0 1
netname=SEL_LO3
}
N 25900 60600 29800 60600 4
N 21900 55200 23400 55200 4
{
T 22000 55300 5 10 1 1 0 0 1
netname=SEL_ATTEN
}
C 23000 58900 1 270 0 gnd-1.sym
C 23000 58500 1 270 0 gnd-1.sym
N 22900 58000 23400 58000 4
N 21900 56000 23400 56000 4
{
T 22000 56100 5 10 1 1 0 0 1
netname=SEL_LO1
}
N 30700 52800 29200 52800 4
{
T 29200 52900 5 10 1 1 0 0 1
netname=LogAmp_315MHz
}
N 30700 52000 29200 52000 4
{
T 29200 52100 5 10 1 1 0 0 1
netname=LogAmp_45MHz
}
N 23400 56400 20000 56400 4
{
T 22200 56500 5 10 1 1 0 0 1
netname=PLL_MUX
}
L 17300 62400 17300 61800 3 0 0 0 -1 -1
L 17200 63300 17100 63300 3 0 0 0 -1 -1
L 17400 62500 17700 62500 3 0 0 0 -1 -1
T 17800 62400 9 10 1 0 0 0 1
To Digitizer board
C 26400 61300 1 0 0 nc-right-1.sym
{
T 26500 61800 5 10 0 0 0 0 1
value=NoConnection
T 26500 62000 5 10 0 0 0 0 1
device=DRC_Directive
}
N 26400 61400 25900 61400 4
C 26400 55100 1 0 0 nc-right-1.sym
{
T 26500 55600 5 10 0 0 0 0 1
value=NoConnection
T 26500 55800 5 10 0 0 0 0 1
device=DRC_Directive
}
C 26400 55500 1 0 0 nc-right-1.sym
{
T 26500 56000 5 10 0 0 0 0 1
value=NoConnection
T 26500 56200 5 10 0 0 0 0 1
device=DRC_Directive
}
N 26400 55200 25900 55200 4
N 26400 55600 25900 55600 4
C 24700 51000 1 0 0 header6-1.sym
{
T 24100 53750 5 10 0 0 0 0 1
device=HEADER6
T 25000 53550 5 10 1 1 0 0 1
refdes=J401
T 24700 51000 5 10 0 0 0 0 1
footprint=JUMPER6
T 24700 51000 5 10 0 1 0 0 1
documentation=Included w/ Adafruit parts
}
C 23300 52400 1 0 0 5V-plus-1.sym
{
T 23300 52400 5 10 0 0 0 0 1
netname=+5VDC
}
N 23500 52400 24700 52400 4
C 23300 51600 1 0 0 3.3V-plus-1.sym
{
T 23300 51600 5 10 0 0 0 0 1
netname=+3.3VDC
}
N 23500 51600 24700 51600 4
C 23300 53200 1 0 0 7.5V-plus-1.sym
{
T 23300 53200 5 10 0 0 0 0 1
netname=+7.5VDC
}
N 23500 53200 24700 53200 4
C 24400 52900 1 270 0 gnd-1.sym
C 24400 52100 1 270 0 gnd-1.sym
C 24400 51300 1 270 0 gnd-1.sym
L 17100 61700 17200 61700 3 0 0 0 -1 -1
C 22900 58100 1 180 0 nc-right-1.sym
{
T 22800 57600 5 10 0 0 180 0 1
value=NoConnection
T 22800 57400 5 10 0 0 180 0 1
device=DRC_Directive
}
C 22900 59300 1 180 0 nc-right-1.sym
{
T 22800 58800 5 10 0 0 180 0 1
value=NoConnection
T 22800 58600 5 10 0 0 180 0 1
device=DRC_Directive
}
N 22900 59200 23400 59200 4
T 24200 54500 9 10 1 0 0 0 1
Arduino Header (Signal Bus)
T 24300 50600 9 10 1 0 0 0 1
Power Supply Header
T 30200 50600 9 10 1 0 0 0 1
Digitizer Header (pg. 3)
T 27600 55200 9 10 1 0 0 0 2
Reserved for
Serial Comms
C 32300 61900 1 270 0 resistor-1.sym
{
T 32700 61600 5 10 0 1 270 0 1
device=RESISTOR
T 32200 61700 5 10 1 1 180 0 1
refdes=R401
T 32300 61900 5 10 0 1 270 0 1
footprint=0402
T 32200 61300 5 10 1 1 180 0 1
value=1.5 K
T 32300 61900 5 10 0 1 270 0 1
man-part-num=CRCW04021K50FKEDC
}
C 31000 61900 1 0 0 resistor-1.sym
{
T 31300 62300 5 10 0 1 0 0 1
device=RESISTOR
T 31200 62200 5 10 1 1 0 0 1
refdes=R400
T 31000 61900 5 10 0 1 0 0 1
footprint=0402
T 31200 61700 5 10 1 1 0 0 1
value=1 K
T 31000 61900 5 10 0 1 0 0 1
description=R502
T 31000 61900 5 10 0 1 0 0 1
man-part-num=CRCW04021K00FKEDC
}
N 27400 56400 25900 56400 4
{
T 26200 56500 5 10 1 1 0 0 1
netname=SEL_LO2
}
N 27400 57200 25900 57200 4
{
T 26200 57300 5 10 1 1 0 0 1
netname=\_SEL_ADC
}
C 30400 53300 1 270 0 gnd-1.sym
C 30400 52500 1 270 0 gnd-1.sym
C 30700 51000 1 0 0 header6-1.sym
{
T 30100 53750 5 10 0 0 0 0 1
device=HEADER6
T 31000 53550 5 10 1 1 0 0 1
refdes=J402
T 30700 51000 5 10 0 0 0 0 1
footprint=JUMPER6
T 30700 51000 5 10 0 0 0 0 1
documentation=Included w/ Adafruit parts
}
C 30400 51700 1 270 0 gnd-1.sym
C 29700 51300 1 180 0 nc-right-1.sym
{
T 29600 50800 5 10 0 0 180 0 1
value=NoConnection
T 29600 50600 5 10 0 0 180 0 1
device=DRC_Directive
}
N 29700 51200 30700 51200 4
N 33700 62000 31900 62000 4
{
T 33600 62300 5 10 1 1 180 0 1
netname=SPI_CLK
}
C 32300 60600 1 0 0 gnd-1.sym
T 22150 61050 9 10 1 0 180 0 1
C
T 22150 62050 9 10 1 0 180 0 1
A
T 17600 61250 9 10 1 0 0 0 1
CC
L 17300 61600 17300 61400 3 0 0 0 -1 -1
L 17400 61300 17500 61300 3 0 0 0 -1 -1
L 17300 61200 17300 61000 3 0 0 0 -1 -1
L 17200 60900 17300 61000 3 0 0 0 -1 -1
L 17300 61200 17400 61300 3 0 0 0 -1 -1
L 17400 61300 17300 61400 3 0 0 0 -1 -1
L 17300 61600 17200 61700 3 0 0 0 -1 -1
L 17200 61700 17300 61800 3 0 0 0 -1 -1
L 17300 62400 17400 62500 3 0 0 0 -1 -1
L 17300 62600 17400 62500 3 0 0 0 -1 -1
L 17300 62600 17300 63200 3 0 0 0 -1 -1
L 17300 63200 17200 63300 3 0 0 0 -1 -1
T 22050 61450 9 10 1 0 0 0 1
B
N 32400 60900 32400 61000 4
N 32400 61900 32400 62000 4
L 21900 62000 21500 62000 3 0 0 0 -1 -1
L 21500 61500 21900 61500 3 0 0 0 -1 -1
L 17100 60900 17200 60900 3 0 0 0 -1 -1
L 17300 60800 17300 60600 3 0 0 0 -1 -1
L 17400 60500 17500 60500 3 0 0 0 -1 -1
L 17300 60400 17300 60200 3 0 0 0 -1 -1
L 17200 60100 17300 60200 3 0 0 0 -1 -1
L 17300 60400 17400 60500 3 0 0 0 -1 -1
L 17400 60500 17300 60600 3 0 0 0 -1 -1
L 17300 60800 17200 60900 3 0 0 0 -1 -1
T 17600 60450 9 10 1 0 0 0 1
DD
L 26900 55800 27000 55800 3 0 0 0 -1 -1
L 26900 55800 27000 55800 3 0 0 0 -1 -1
L 27100 55700 27100 55500 3 0 0 0 -1 -1
L 27200 55400 27500 55400 3 0 0 0 -1 -1
L 26900 55000 27000 55000 3 0 0 0 -1 -1
L 27100 55300 27100 55100 3 0 0 0 -1 -1
L 27000 55000 27100 55100 3 0 0 0 -1 -1
L 27100 55300 27200 55400 3 0 0 0 -1 -1
L 27200 55400 27100 55500 3 0 0 0 -1 -1
L 27100 55700 27000 55800 3 0 0 0 -1 -1
L 21500 61000 21900 61000 3 0 0 0 -1 -1
L 27500 59000 27900 59000 3 0 0 0 -1 -1
L 17700 62500 17500 62600 3 0 0 0 -1 -1
L 17700 62500 17500 62400 3 0 0 0 -1 -1
L 20500 60500 20500 60900 3 0 0 0 -1 -1
L 20400 61000 20300 61000 3 0 0 0 -1 -1
L 20700 61600 20600 61600 3 0 0 0 -1 -1
L 20500 61100 20500 61500 3 0 0 0 -1 -1
L 20600 61600 20500 61500 3 0 0 0 -1 -1
L 20500 61100 20400 61000 3 0 0 0 -1 -1
L 20400 61000 20500 60900 3 0 0 0 -1 -1
L 20500 60500 20600 60400 3 0 0 0 -1 -1
T 20000 60950 9 10 1 0 0 0 1
AA
L 17100 60100 17200 60100 3 0 0 0 -1 -1
V 20100 61000 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 22100 62000 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 22100 61500 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 22100 61000 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 28100 59000 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 17700 61300 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
V 17700 60500 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
L 27900 58600 27500 58600 3 0 0 0 -1 -1
V 28100 58600 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
T 28050 58550 9 10 1 0 0 0 1
E
L 29100 53000 28700 53000 3 0 0 0 -1 -1
V 28500 53000 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
T 28450 52950 9 10 1 0 0 0 1
F
L 20700 60400 20600 60400 3 0 0 0 -1 -1
N 26400 61000 25900 61000 4
N 23300 58800 23400 58800 4
N 23300 58400 23400 58400 4
C 20700 53200 1 0 0 gnd-1.sym
N 20800 53500 19100 53500 4
C 19800 53000 1 0 0 Faraday-cage.sym
{
T 19800 53800 5 10 0 0 0 0 1
device=SHEILD
T 19800 53600 5 10 1 1 0 0 1
refdes=Sh405
T 19800 53000 5 10 0 0 0 0 1
footprint=Shield_6
}
C 18900 53000 1 0 0 Faraday-cage.sym
{
T 18900 53800 5 10 0 0 0 0 1
device=SHEILD
T 18900 53600 5 10 1 1 0 0 1
refdes=Sh401
}
L 20600 62800 20700 62800 3 0 0 0 -1 -1
V 20100 62400 200 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
L 20700 62000 20600 62000 3 0 0 0 -1 -1
L 20500 62100 20500 62300 3 0 0 0 -1 -1
L 20400 62400 20300 62400 3 0 0 0 -1 -1
L 20500 62500 20500 62700 3 0 0 0 -1 -1
L 20600 62800 20500 62700 3 0 0 0 -1 -1
L 20500 62500 20400 62400 3 0 0 0 -1 -1
L 20400 62400 20500 62300 3 0 0 0 -1 -1
L 20500 62100 20600 62000 3 0 0 0 -1 -1
T 20250 62450 9 10 1 0 180 0 1
BB
C 23200 55000 1 0 0 Arduino_Uno_header.sym
{
T 23700 60200 5 10 1 1 0 0 1
refdes=J400
T 24500 62700 5 10 0 0 0 0 1
device=ARDUINO_UNO_HEADER32
T 23200 55000 5 10 0 0 0 0 1
footprint=Header_Arduino
}
C 31000 58900 1 0 0 resistor-1.sym
{
T 31300 59300 5 10 0 1 0 0 1
device=RESISTOR
T 31200 59200 5 10 1 1 0 0 1
refdes=R404
T 31000 58900 5 10 0 1 0 0 1
footprint=0402
T 31200 58700 5 10 1 1 0 0 1
value=1 K
T 31000 58900 5 10 0 1 0 0 1
description=R502
T 31000 58900 5 10 0 1 0 0 1
man-part-num=CRCW04021K00FKEDC
}
C 32300 58900 1 270 0 resistor-1.sym
{
T 32700 58600 5 10 0 1 270 0 1
device=RESISTOR
T 32200 58700 5 10 1 1 180 0 1
refdes=R405
T 32300 58900 5 10 0 1 270 0 1
footprint=0402
T 32200 58300 5 10 1 1 180 0 1
value=1.5 K
T 32300 58900 5 10 0 1 270 0 1
man-part-num=CRCW04021K50FKEDC
}
C 32300 57600 1 0 0 gnd-1.sym
N 32400 57900 32400 58000 4
N 31000 59000 29800 59000 4
N 29800 59000 29800 59800 4
N 25900 59800 29800 59800 4
N 32400 58900 32400 59000 4
T 28050 58950 9 10 1 0 0 0 1
D
C 26400 60100 1 0 0 nc-right-1.sym
{
T 26500 60600 5 10 0 0 0 0 1
value=NoConnection
T 26500 60800 5 10 0 0 0 0 1
device=DRC_Directive
}
L 27000 60200 32600 60200 3 0 0 0 -1 -1
T 32800 60150 9 10 1 0 0 0 1
SPI_MISO
N 26400 60200 25900 60200 4
N 31000 62000 29800 62000 4
N 29800 62000 29800 60600 4
L 32600 60200 32700 60300 3 0 0 0 -1 -1
L 32600 60200 32700 60100 3 0 0 0 -1 -1
L 32700 60200 32800 60100 3 0 0 0 -1 -1
L 32700 60200 32800 60300 3 0 0 0 -1 -1
L 27000 60200 27100 60300 3 0 0 0 -1 -1
L 27000 60200 27100 60100 3 0 0 0 -1 -1
C 20900 55500 1 90 0 resistor-1.sym
{
T 20500 55800 5 10 0 1 90 0 1
device=RESISTOR
T 20200 56000 5 10 1 1 0 0 1
refdes=R402
T 20900 55500 5 10 0 1 90 0 1
footprint=0402
T 20200 55700 5 10 1 1 0 0 1
value=22 K
T 20900 55500 5 10 0 1 90 0 1
man-part-num=CRCW040222K0FKEDC
}
C 20700 55200 1 0 0 gnd-1.sym
