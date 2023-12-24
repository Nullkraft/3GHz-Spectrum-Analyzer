v 20201216 2
N 47900 68500 48400 68500 4
{
T 47000 68450 5 10 1 1 0 0 1
netname=PLL_MUX
}
N 61800 61900 61500 61900 4
{
T 60050 61850 5 10 1 1 0 0 1
netname=LogAmp_45MHz
}
N 61500 62700 61800 62700 4
{
T 59950 62650 5 10 1 1 0 0 1
netname=LogAmp_315MHz
}
N 52100 71900 54400 71900 4
{
T 52300 72000 5 10 1 1 0 0 1
netname=SPI_MOSI
}
N 52100 72700 54400 72700 4
{
T 52300 72800 5 10 1 1 0 0 1
netname=SPI_CLK
}
C 47600 70600 1 270 0 gnd-1.sym
C 51700 73000 1 90 0 gnd-1.sym
C 47400 71200 1 0 0 nc-left-1.sym
{
T 47400 71600 5 10 0 0 0 0 1
value=NoConnection
T 47400 72000 5 10 0 0 0 0 1
device=DRC_Directive
}
N 47900 71300 48400 71300 4
N 47900 70900 48400 70900 4
C 56000 60900 1 0 0 header6-1.sym
{
T 55400 63650 5 10 0 0 0 0 1
device=HEADER6
T 56300 63350 5 10 1 1 0 0 1
refdes=J401
T 55400 60500 5 10 0 1 0 0 1
description=Included w/ Adafruit parts
T 56000 60900 5 10 0 0 0 0 1
footprint=JUMPER6
}
C 55700 62800 1 270 0 gnd-1.sym
N 54400 63100 56000 63100 4
{
T 54900 63200 5 10 1 1 0 0 1
netname=+5VDC
}
N 47900 70500 48400 70500 4
N 51400 73100 50900 73100 4
C 47600 71000 1 270 0 gnd-1.sym
C 47400 70000 1 0 0 nc-left-1.sym
{
T 47400 70400 5 10 0 0 0 0 1
value=NoConnection
T 47400 70800 5 10 0 0 0 0 1
device=DRC_Directive
}
N 47900 70100 48400 70100 4
C 61500 63200 1 270 0 gnd-1.sym
C 61500 62400 1 270 0 gnd-1.sym
C 55700 62000 1 270 0 gnd-1.sym
C 55700 61200 1 270 0 gnd-1.sym
C 61800 60900 1 0 0 header6-1.sym
{
T 61200 63650 5 10 0 0 0 0 1
device=HEADER6
T 62100 63350 5 10 1 1 0 0 1
refdes=J402
T 61200 60500 5 10 0 1 0 0 1
description=Included w/ Adafruit parts
T 61800 60900 5 10 0 0 0 0 1
footprint=JUMPER6
}
C 61500 61600 1 270 0 gnd-1.sym
C 61300 61000 1 0 0 nc-left-1.sym
{
T 61300 61400 5 10 0 0 0 0 1
value=NoConnection
T 61300 61800 5 10 0 0 0 0 1
device=DRC_Directive
}
C 35000 53800 0 0 0 title-bordered-D.sym
{
T 66500 54300 15 10 1 1 0 0 1
author=Mark Stanley
T 62100 54800 15 10 1 1 0 0 1
block-filename=control-page1.sch
T 63800 54300 15 10 1 1 0 0 1
block-num-pages=1
T 62100 54300 15 10 1 1 0 0 1
block-page=1
T 66500 54800 15 10 1 1 0 0 1
block-revision=2.0.0.00
T 62800 55300 15 10 1 1 0 0 1
block-title=Control Board (ADC, SRAM, and Feather RP2040)
}
N 52700 70700 52200 70700 4
{
T 53600 70650 5 10 1 1 0 6 1
netname=REF_EN1
}
N 52700 71100 52200 71100 4
{
T 53600 71050 5 10 1 1 0 6 1
netname=REF_EN2
}
C 48200 67100 1 0 0 Arduino_Uno_header.sym
{
T 48600 71900 5 10 1 1 0 0 1
refdes=J400
T 49500 74800 5 10 0 0 0 0 1
device=ARDUINO_UNO_HEADER32
T 48200 67100 5 10 0 1 0 0 1
footprint=Header_Arduino
}
C 39950 68500 1 0 0 Adafruit_feather.sym
{
T 42450 73100 5 10 0 0 0 0 1
device=Arduino Uno
T 41850 73300 5 10 1 1 0 0 1
refdes=U2
T 41050 68900 5 10 1 1 0 0 1
man-part-num=4884
T 40550 68100 5 10 1 1 0 0 1
description=Feather RP2040
T 39950 68500 5 10 0 0 0 0 1
footprint=Feather
}
N 47900 67300 48400 67300 4
{
T 46800 67250 5 10 1 1 0 0 1
netname=SEL_ATTEN
}
N 47900 67700 48400 67700 4
{
T 47050 67650 5 10 1 1 0 0 1
netname=SEL_LO3
}
N 47900 68100 48400 68100 4
{
T 47050 68050 5 10 1 1 0 0 1
netname=SEL_LO1
}
N 42550 69700 43100 69700 4
{
T 43975 69650 5 10 1 1 0 6 1
netname=SEL_LO2
}
N 66700 71400 66800 71400 4
{
T 66900 71350 5 10 1 1 0 0 1
netname=\_SEL_ADC_1
}
N 52300 68500 52700 68500 4
{
T 53600 68450 5 10 1 1 0 6 1
netname=SEL_LO2
}
N 43100 70000 42550 70000 4
{
T 44000 69950 5 10 1 1 0 6 1
netname=REF_EN1
}
N 43100 70300 42550 70300 4
{
T 44000 70250 5 10 1 1 0 6 1
netname=REF_EN2
}
N 39400 71200 39950 71200 4
{
T 38900 71150 5 10 1 1 0 0 1
netname=MUX
}
U 45900 74400 54600 74400 10 -1
{
T 45650 74500 5 10 1 1 0 0 1
netname=SPI_BUS
}
N 54400 61500 56000 61500 4
{
T 54900 61600 5 10 1 1 0 0 1
netname=+3.3VDC
}
C 54200 61500 1 0 0 3.3V-plus-1.sym
N 39400 70600 39950 70600 4
{
T 39300 70550 5 10 1 1 0 6 1
netname=LO3
}
N 39400 70900 39950 70900 4
{
T 39300 70850 5 10 1 1 0 6 1
netname=LO1
}
N 39400 70300 39950 70300 4
{
T 38725 70250 5 10 1 1 0 0 1
netname=ATTEN
}
C 54200 63100 1 0 0 5V-plus-1.sym
C 57200 70900 1 0 0 IS66WVS4M8.sym
{
T 58595 72500 5 10 1 1 0 0 1
refdes=U3
T 57195 72500 5 10 1 1 0 0 1
device=SDRAM
T 57200 70900 5 10 0 1 0 0 1
documentation=https://www.mouser.com/ProductDetail/ISSI/IS66WVS4M8BLL-104NLI?qs=doiCPypUmgHfByEUFkOsAw==
T 57200 70900 5 10 0 1 0 0 1
footprint=SOIC_8_150mil
T 57200 69600 5 10 0 0 0 0 1
man-part-num=IS66WVS4M8ALL
}
N 57200 73600 54800 73600 4
{
T 55800 73825 5 10 1 1 180 0 1
netname=SPI_CLK
}
N 57200 73100 54800 73100 4
{
T 54950 73325 5 10 1 1 180 6 1
netname=SPI_MOSI
}
N 56200 72100 56300 72100 4
{
T 55250 72050 5 10 1 1 0 0 1
netname=\_SEL_RAM\_
}
C 58900 72300 1 0 0 3.3V-plus-1.sym
C 57000 70600 1 0 0 gnd-1.sym
N 57200 70900 57100 70900 4
C 66700 61400 1 0 0 2.5V_Ref_LM4040.sym
{
T 66895 62400 5 10 1 1 0 0 1
refdes=U7
T 66595 60900 5 10 1 1 0 0 1
device=+2.5V ADC Reference
T 66792 62900 5 10 0 1 0 0 1
footprint=SOT23
T 66664 60700 5 10 1 1 0 0 1
man-part-num=LM4040CYM3-2.5TR
}
C 63700 71400 1 0 0 ADS7826.sym
{
T 64042 70900 5 10 1 1 0 0 1
man-part-num=ADS7826IDRBT 
T 65195 73000 5 10 1 1 0 0 1
refdes=U1
T 63892 73600 5 10 0 0 0 0 1
footprint=SON_8
T 64500 73000 5 10 1 1 0 0 1
device=ADC
}
C 63700 66600 1 0 0 ADS7826.sym
{
T 64042 66100 5 10 1 1 0 0 1
man-part-num=ADS7826IDRBT 
T 65195 68200 5 10 1 1 0 0 1
refdes=U4
T 63892 68800 5 10 0 0 0 0 1
footprint=SON_8
T 64500 68200 5 10 1 1 0 0 1
device=ADC
}
C 63400 66000 1 0 0 gnd-1.sym
N 63700 67000 63500 67000 4
N 63500 67000 63500 66300 4
N 63700 66600 63500 66600 4
C 63400 70800 1 0 0 gnd-1.sym
N 63700 71800 63500 71800 4
N 63500 71800 63500 71100 4
N 63700 71400 63500 71400 4
N 63400 72200 63700 72200 4
{
T 61850 72150 5 10 1 1 0 0 1
netname=LogAmp_315MHz
}
N 63700 67400 63400 67400 4
{
T 61950 67350 5 10 1 1 0 0 1
netname=LogAmp_45MHz
}
N 65700 71800 67900 71800 4
{
T 66100 71850 5 10 1 1 0 0 1
netname=MISO
}
C 66800 67700 1 90 0 gnd-1.sym
C 65600 68300 1 0 0 3.3V-plus-1.sym
N 65800 68300 65800 67800 4
C 66000 62200 1 90 0 resistor-1.sym
{
T 65600 62500 5 10 0 0 90 0 1
device=RESISTOR
T 66100 62800 5 10 1 1 0 0 1
refdes=R10
T 66100 62500 5 10 1 1 0 0 1
value=680 Ω
T 66000 62200 5 10 0 0 90 0 1
footprint=0603
T 66000 62200 5 10 0 1 90 0 1
man-part-num=RMC16K681FTP
}
C 65800 60300 1 0 0 gnd-1.sym
C 65700 63100 1 0 0 3.3V-plus-1.sym
N 65900 60600 65900 61000 4
N 66700 61400 66500 61400 4
N 66500 61400 66500 60700 4
N 66500 60700 65900 60700 4
N 65900 61600 65900 62200 4
N 66700 62000 65900 62000 4
N 65900 62000 65500 62000 4
{
T 65400 61950 5 10 1 1 0 6 1
netname=+2.5V_Aref
}
C 62800 67700 1 0 0 resistor-1.sym
{
T 63100 68100 5 10 0 0 0 0 1
device=RESISTOR
T 62800 67700 5 10 0 0 0 0 1
footprint=0805
T 62800 67700 5 10 0 1 0 0 1
man-part-num=L-0805M5R00FGTS
T 62850 67950 5 10 1 1 0 0 1
refdes=R4
T 63250 67950 5 10 1 1 0 0 1
value=5 Ω
}
C 62500 68100 1 0 0 gnd-1.sym
N 63500 68500 63700 68500 4
N 63700 68500 63700 67800 4
N 62600 68400 62600 68500 4
N 62600 68500 62900 68500 4
N 62800 67800 62600 67800 4
{
T 62500 67750 5 10 1 1 0 6 1
netname=+2.5V_Aref
}
C 62800 72500 1 0 0 resistor-1.sym
{
T 63100 72900 5 10 0 0 0 0 1
device=RESISTOR
T 62800 72500 5 10 0 0 0 0 1
footprint=0805
T 62800 72500 5 10 0 1 0 0 1
man-part-num=L-0805M5R00FGTS
T 62850 72750 5 10 1 1 0 0 1
refdes=R1
T 63350 72750 5 10 1 1 0 0 1
value=5 Ω
}
C 62500 72900 1 0 0 gnd-1.sym
N 63500 73300 63700 73300 4
N 63700 73300 63700 72600 4
N 62600 73200 62600 73300 4
N 62600 73300 62900 73300 4
N 62800 72600 62600 72600 4
{
T 62500 72550 5 10 1 1 0 6 1
netname=+2.5V_Aref
}
N 66700 66600 66800 66600 4
{
T 66900 66550 5 10 1 1 0 0 1
netname=\_SEL_ADC_2
}
N 42550 69400 43100 69400 4
{
T 43200 69350 5 10 1 1 0 0 1
netname=\_SEL_ADC_1
}
N 42550 69100 43100 69100 4
{
T 43200 69050 5 10 1 1 0 0 1
netname=\_SEL_ADC_2
}
T 48800 66000 9 12 1 0 0 0 3
      Interface to
Spectrum Analyzer
           board
N 42550 65400 45700 65400 4
{
T 44650 65450 5 10 1 1 0 0 1
netname=SPI_MOSI
}
N 42550 66000 45700 66000 4
{
T 44650 66050 5 10 1 1 0 0 1
netname=SPI_CLK
}
C 39950 62100 1 0 0 Adafruit_mini.sym
{
T 40250 67500 5 10 0 0 0 0 1
device=Arduino Uno
T 40250 67300 5 10 0 0 0 0 1
footprint=Adafruit_Metro_Mini
T 41950 66300 5 10 1 1 0 0 1
refdes=U6
T 41050 63900 5 10 1 1 0 0 1
man-part-num=2590
T 40650 61700 5 10 1 1 0 0 1
description=Metro Mini 328
}
N 42550 63300 43050 63300 4
{
T 43925 63250 5 10 1 1 0 6 1
netname=SEL_LO2
}
N 43050 63600 42550 63600 4
{
T 43950 63550 5 10 1 1 0 6 1
netname=REF_EN1
}
N 43050 63900 42550 63900 4
{
T 43950 63850 5 10 1 1 0 6 1
netname=REF_EN2
}
N 42550 63000 43050 63000 4
{
T 43150 62950 5 10 1 1 0 0 1
netname=\_SEL_ADC_1
}
N 42550 62700 43050 62700 4
{
T 43150 62650 5 10 1 1 0 0 1
netname=\_SEL_ADC_2
}
N 56200 67600 56300 67600 4
{
T 55050 67550 5 10 1 1 0 0 1
netname=\_SEL_FLASH\_
}
C 57000 66100 1 0 0 gnd-1.sym
N 57200 66400 57100 66400 4
U 45900 74400 45900 65100 10 1
N 40950 70000 40950 72800 4
N 40950 72800 45700 72800 4
{
T 43200 72850 5 10 1 1 0 0 1
netname=SPI_CLK
}
C 45700 72800 1 270 1 busripper-1.sym
{
T 46100 72800 5 8 0 0 270 6 1
device=none
}
N 41250 69700 41250 72500 4
N 41250 72500 45700 72500 4
{
T 43200 72550 5 10 1 1 0 0 1
netname=SPI_MOSI
}
C 45700 72500 1 270 1 busripper-1.sym
{
T 46100 72500 5 8 0 0 270 6 1
device=none
}
N 41550 69400 41550 72200 4
N 41550 72200 45700 72200 4
{
T 43200 72250 5 10 1 1 0 0 1
netname=SPI_MISO
}
C 45700 72200 1 270 1 busripper-1.sym
{
T 46100 72200 5 8 0 0 270 6 1
device=none
}
N 56700 70400 60400 70400 4
{
T 59700 70450 5 10 1 1 0 0 1
netname=MISO
}
C 45700 66000 1 270 1 busripper-1.sym
{
T 46100 66000 5 8 0 0 270 6 1
device=none
}
C 45700 65400 1 270 1 busripper-1.sym
{
T 46100 65400 5 8 0 0 270 6 1
device=none
}
U 54600 74400 54600 71700 10 1
N 65700 67000 67900 67000 4
{
T 66100 67050 5 10 1 1 0 0 1
netname=MISO
}
C 54400 72700 1 270 1 busripper-1.sym
{
T 54800 72700 5 8 0 0 270 6 1
device=none
}
C 47500 61700 1 0 0 gnd-1.sym
L 51900 65500 51700 65500 3 0 1 0 -1 -1
L 51500 65500 51300 65500 3 0 1 0 -1 -1
L 51100 65500 50900 65500 3 0 1 0 -1 -1
L 50700 65500 50500 65500 3 0 1 0 -1 -1
L 50300 65500 50100 65500 3 0 1 0 -1 -1
L 49900 65500 49700 65500 3 0 1 0 -1 -1
L 49500 65500 49300 65500 3 0 1 0 -1 -1
L 49100 65500 48900 65500 3 0 1 0 -1 -1
L 48700 65500 48500 65500 3 0 1 0 -1 -1
L 48300 65500 48100 65500 3 0 1 0 -1 -1
L 47900 65500 47700 65500 3 0 1 0 -1 -1
L 47500 65500 47300 65500 3 0 1 0 -1 -1
L 59900 65500 59700 65500 3 0 1 0 -1 -1
L 59500 65500 59300 65500 3 0 1 0 -1 -1
L 59100 65500 58900 65500 3 0 1 0 -1 -1
L 58700 65500 58500 65500 3 0 1 0 -1 -1
L 58300 65500 58100 65500 3 0 1 0 -1 -1
L 57900 65500 57700 65500 3 0 1 0 -1 -1
L 57500 65500 57300 65500 3 0 1 0 -1 -1
L 57100 65500 56900 65500 3 0 1 0 -1 -1
L 56700 65500 56500 65500 3 0 1 0 -1 -1
L 56300 65500 56100 65500 3 0 1 0 -1 -1
L 55900 65500 55700 65500 3 0 1 0 -1 -1
L 55500 65500 55300 65500 3 0 1 0 -1 -1
L 55100 65500 54900 65500 3 0 1 0 -1 -1
L 54700 65500 54500 65500 3 0 1 0 -1 -1
L 54300 65500 54100 65500 3 0 1 0 -1 -1
L 53900 65500 53700 65500 3 0 1 0 -1 -1
L 53500 65500 53300 65500 3 0 1 0 -1 -1
L 53100 65500 52900 65500 3 0 1 0 -1 -1
L 52700 65500 52500 65500 3 0 1 0 -1 -1
L 52300 65500 52100 65500 3 0 1 0 -1 -1
L 67900 65500 67700 65500 3 0 1 0 -1 -1
L 67500 65500 67300 65500 3 0 1 0 -1 -1
L 67100 65500 66900 65500 3 0 1 0 -1 -1
L 66700 65500 66500 65500 3 0 1 0 -1 -1
L 66300 65500 66100 65500 3 0 1 0 -1 -1
L 65900 65500 65700 65500 3 0 1 0 -1 -1
L 65500 65500 65300 65500 3 0 1 0 -1 -1
L 65100 65500 64900 65500 3 0 1 0 -1 -1
L 64700 65500 64500 65500 3 0 1 0 -1 -1
L 64300 65500 64100 65500 3 0 1 0 -1 -1
L 63900 65500 63700 65500 3 0 1 0 -1 -1
L 63500 65500 63300 65500 3 0 1 0 -1 -1
L 63100 65500 62900 65500 3 0 1 0 -1 -1
L 62700 65500 62500 65500 3 0 1 0 -1 -1
L 62300 65500 62100 65500 3 0 1 0 -1 -1
L 61900 65500 61700 65500 3 0 1 0 -1 -1
L 61500 65500 61300 65500 3 0 1 0 -1 -1
L 61100 65500 60900 65500 3 0 1 0 -1 -1
L 60700 65500 60500 65500 3 0 1 0 -1 -1
L 60300 65500 60100 65500 3 0 1 0 -1 -1
L 68700 65500 68500 65500 3 0 1 0 -1 -1
L 68300 65500 68100 65500 3 0 1 0 -1 -1
L 43100 59200 42900 59200 3 0 1 0 -1 -1
L 42700 59200 42500 59200 3 0 1 0 -1 -1
L 42300 59200 42100 59200 3 0 1 0 -1 -1
L 41900 59200 41700 59200 3 0 1 0 -1 -1
L 41500 59200 41300 59200 3 0 1 0 -1 -1
L 41100 59200 40900 59200 3 0 1 0 -1 -1
L 40700 59200 40500 59200 3 0 1 0 -1 -1
L 40300 59200 40100 59200 3 0 1 0 -1 -1
L 39900 59200 39700 59200 3 0 1 0 -1 -1
L 39500 59200 39300 59200 3 0 1 0 -1 -1
L 39100 59200 38900 59200 3 0 1 0 -1 -1
L 38700 59200 38500 59200 3 0 1 0 -1 -1
L 38300 59200 38100 59200 3 0 1 0 -1 -1
L 37900 59200 37700 59200 3 0 1 0 -1 -1
L 37500 59200 37300 59200 3 0 1 0 -1 -1
L 37100 59200 36900 59200 3 0 1 0 -1 -1
L 36700 59200 36500 59200 3 0 1 0 -1 -1
L 36300 59200 36100 59200 3 0 1 0 -1 -1
L 35900 59200 35700 59200 3 0 1 0 -1 -1
L 35500 59200 35300 59200 3 0 1 0 -1 -1
L 51100 59200 50900 59200 3 0 1 0 -1 -1
L 50700 59200 50500 59200 3 0 1 0 -1 -1
L 50300 59200 50100 59200 3 0 1 0 -1 -1
L 49900 59200 49700 59200 3 0 1 0 -1 -1
L 49500 59200 49300 59200 3 0 1 0 -1 -1
L 49100 59200 48900 59200 3 0 1 0 -1 -1
L 48700 59200 48500 59200 3 0 1 0 -1 -1
L 48300 59200 48100 59200 3 0 1 0 -1 -1
L 47900 59200 47700 59200 3 0 1 0 -1 -1
L 47500 59200 47300 59200 3 0 1 0 -1 -1
L 47100 59200 46900 59200 3 0 1 0 -1 -1
L 46700 59200 46500 59200 3 0 1 0 -1 -1
L 46300 59200 46100 59200 3 0 1 0 -1 -1
L 45900 59200 45700 59200 3 0 1 0 -1 -1
L 45500 59200 45300 59200 3 0 1 0 -1 -1
L 45100 59200 44900 59200 3 0 1 0 -1 -1
L 44700 59200 44500 59200 3 0 1 0 -1 -1
L 44300 59200 44100 59200 3 0 1 0 -1 -1
L 43900 59200 43700 59200 3 0 1 0 -1 -1
L 43500 59200 43300 59200 3 0 1 0 -1 -1
L 59100 59200 58900 59200 3 0 1 0 -1 -1
L 58700 59200 58500 59200 3 0 1 0 -1 -1
L 58300 59200 58100 59200 3 0 1 0 -1 -1
L 57900 59200 57700 59200 3 0 1 0 -1 -1
L 57500 59200 57300 59200 3 0 1 0 -1 -1
L 57100 59200 56900 59200 3 0 1 0 -1 -1
L 56700 59200 56500 59200 3 0 1 0 -1 -1
L 56300 59200 56100 59200 3 0 1 0 -1 -1
L 55900 59200 55700 59200 3 0 1 0 -1 -1
L 55500 59200 55300 59200 3 0 1 0 -1 -1
L 55100 59200 54900 59200 3 0 1 0 -1 -1
L 54700 59200 54500 59200 3 0 1 0 -1 -1
L 54300 59200 54100 59200 3 0 1 0 -1 -1
L 53900 59200 53700 59200 3 0 1 0 -1 -1
L 53500 59200 53300 59200 3 0 1 0 -1 -1
L 53100 59200 52900 59200 3 0 1 0 -1 -1
L 52700 59200 52500 59200 3 0 1 0 -1 -1
L 52300 59200 52100 59200 3 0 1 0 -1 -1
L 51900 59200 51700 59200 3 0 1 0 -1 -1
L 51500 59200 51300 59200 3 0 1 0 -1 -1
L 67100 59200 66900 59200 3 0 1 0 -1 -1
L 66700 59200 66500 59200 3 0 1 0 -1 -1
L 66300 59200 66100 59200 3 0 1 0 -1 -1
L 65900 59200 65700 59200 3 0 1 0 -1 -1
L 65500 59200 65300 59200 3 0 1 0 -1 -1
L 65100 59200 64900 59200 3 0 1 0 -1 -1
L 64700 59200 64500 59200 3 0 1 0 -1 -1
L 64300 59200 64100 59200 3 0 1 0 -1 -1
L 63900 59200 63700 59200 3 0 1 0 -1 -1
L 63500 59200 63300 59200 3 0 1 0 -1 -1
L 63100 59200 62900 59200 3 0 1 0 -1 -1
L 62700 59200 62500 59200 3 0 1 0 -1 -1
L 62300 59200 62100 59200 3 0 1 0 -1 -1
L 61900 59200 61700 59200 3 0 1 0 -1 -1
L 61500 59200 61300 59200 3 0 1 0 -1 -1
L 61100 59200 60900 59200 3 0 1 0 -1 -1
L 60700 59200 60500 59200 3 0 1 0 -1 -1
L 60300 59200 60100 59200 3 0 1 0 -1 -1
L 59900 59200 59700 59200 3 0 1 0 -1 -1
L 59500 59200 59300 59200 3 0 1 0 -1 -1
L 68700 59200 68500 59200 3 0 1 0 -1 -1
L 68300 59200 68100 59200 3 0 1 0 -1 -1
L 67900 59200 67700 59200 3 0 1 0 -1 -1
L 67500 59200 67300 59200 3 0 1 0 -1 -1
L 46600 75500 46600 75300 3 0 1 0 -1 -1
L 46600 75100 46600 74900 3 0 1 0 -1 -1
L 46600 74700 46600 74500 3 0 1 0 -1 -1
L 46600 74300 46600 74100 3 0 1 0 -1 -1
L 46600 73900 46600 73700 3 0 1 0 -1 -1
L 46600 73500 46600 73300 3 0 1 0 -1 -1
L 46600 73100 46600 72900 3 0 1 0 -1 -1
L 46600 72700 46600 72500 3 0 1 0 -1 -1
L 46600 72300 46600 72100 3 0 1 0 -1 -1
L 46600 71900 46600 71700 3 0 1 0 -1 -1
L 46600 71500 46600 71300 3 0 1 0 -1 -1
L 46600 71100 46600 70900 3 0 1 0 -1 -1
L 46600 70700 46600 70500 3 0 1 0 -1 -1
L 46600 70300 46600 70100 3 0 1 0 -1 -1
L 46600 69900 46600 69700 3 0 1 0 -1 -1
L 46600 69500 46600 69300 3 0 1 0 -1 -1
L 46600 69100 46600 68900 3 0 1 0 -1 -1
L 46600 68700 46600 68500 3 0 1 0 -1 -1
L 46600 68300 46600 68100 3 0 1 0 -1 -1
L 46600 67900 46600 67700 3 0 1 0 -1 -1
L 46600 67500 46600 67300 3 0 1 0 -1 -1
L 46600 67100 46600 66900 3 0 1 0 -1 -1
L 46600 66700 46600 66500 3 0 1 0 -1 -1
L 46600 66300 46600 66100 3 0 1 0 -1 -1
L 46600 65900 46600 65700 3 0 1 0 -1 -1
L 46600 65500 46600 65300 3 0 1 0 -1 -1
L 46600 65100 46600 64900 3 0 1 0 -1 -1
L 46600 64700 46600 64500 3 0 1 0 -1 -1
L 46600 64300 46600 64100 3 0 1 0 -1 -1
L 46600 63900 46600 63700 3 0 1 0 -1 -1
L 46600 63500 46600 63300 3 0 1 0 -1 -1
L 46600 63100 46600 62900 3 0 1 0 -1 -1
L 46600 62700 46600 62500 3 0 1 0 -1 -1
L 46600 62300 46600 62100 3 0 1 0 -1 -1
L 46600 61900 46600 61700 3 0 1 0 -1 -1
L 46600 61500 46600 61300 3 0 1 0 -1 -1
L 46600 61100 46600 60900 3 0 1 0 -1 -1
L 46600 60700 46600 60500 3 0 1 0 -1 -1
L 46600 60300 46600 60100 3 0 1 0 -1 -1
L 46600 59900 46600 59700 3 0 1 0 -1 -1
L 46600 59500 46600 59300 3 0 1 0 -1 -1
T 39150 74800 9 18 1 0 0 0 1
CONTROLLER (CHOOSE ONE)
L 54200 75500 54200 75300 3 0 1 0 -1 -1
L 54200 75100 54200 74900 3 0 1 0 -1 -1
L 54200 74700 54200 74500 3 0 1 0 -1 -1
L 54200 74300 54200 74100 3 0 1 0 -1 -1
L 54200 73900 54200 73700 3 0 1 0 -1 -1
L 54200 73400 54200 73300 3 0 1 0 -1 -1
L 54200 73100 54200 72900 3 0 1 0 -1 -1
L 54200 72700 54200 72500 3 0 1 0 -1 -1
L 54200 72300 54200 72100 3 0 1 0 -1 -1
L 54200 71800 54200 71700 3 0 1 0 -1 -1
L 54200 71500 54200 71300 3 0 1 0 -1 -1
L 54200 71100 54200 70900 3 0 1 0 -1 -1
L 54200 70700 54200 70500 3 0 1 0 -1 -1
L 54200 70300 54200 70100 3 0 1 0 -1 -1
L 54200 69900 54200 69700 3 0 1 0 -1 -1
L 54200 69500 54200 69300 3 0 1 0 -1 -1
L 54200 69100 54200 68900 3 0 1 0 -1 -1
L 54200 68700 54200 68500 3 0 1 0 -1 -1
L 54200 68300 54200 68100 3 0 1 0 -1 -1
L 54200 67900 54200 67700 3 0 1 0 -1 -1
L 54200 67500 54200 67300 3 0 1 0 -1 -1
L 54200 67100 54200 66900 3 0 1 0 -1 -1
L 54200 66700 54200 66500 3 0 1 0 -1 -1
L 54200 66300 54200 66100 3 0 1 0 -1 -1
L 54200 65900 54200 65700 3 0 1 0 -1 -1
L 52800 65400 52800 65300 3 0 1 0 -1 -1
L 52800 65100 52800 64900 3 0 1 0 -1 -1
L 52800 64700 52800 64500 3 0 1 0 -1 -1
L 52800 64300 52800 64100 3 0 1 0 -1 -1
L 52800 63900 52800 63700 3 0 1 0 -1 -1
L 52800 63500 52800 63300 3 0 1 0 -1 -1
L 52800 63100 52800 62900 3 0 1 0 -1 -1
L 52800 62700 52800 62500 3 0 1 0 -1 -1
L 52800 62300 52800 62100 3 0 1 0 -1 -1
L 52800 61900 52800 61700 3 0 1 0 -1 -1
L 52800 61500 52800 61300 3 0 1 0 -1 -1
L 52800 61100 52800 60900 3 0 1 0 -1 -1
L 52800 60700 52800 60500 3 0 1 0 -1 -1
L 52800 60300 52800 60100 3 0 1 0 -1 -1
L 52800 59900 52800 59700 3 0 1 0 -1 -1
L 52800 59500 52800 59300 3 0 1 0 -1 -1
T 47700 74800 9 18 1 0 0 0 2
           INTERFACE TO
SPECTRUM ANALYZER BOARD
T 56800 74800 9 18 1 0 0 0 1
MEMORY
T 63500 74800 9 18 1 0 0 0 1
ANALOG TO DIGITAL
L 61300 75500 61300 75300 3 0 1 0 -1 -1
L 61300 75100 61300 74900 3 0 1 0 -1 -1
L 61300 74700 61300 74500 3 0 1 0 -1 -1
L 61300 74300 61300 74100 3 0 1 0 -1 -1
L 61300 73900 61300 73700 3 0 1 0 -1 -1
L 61300 73500 61300 73300 3 0 1 0 -1 -1
L 61300 73100 61300 72900 3 0 1 0 -1 -1
L 61300 72700 61300 72500 3 0 1 0 -1 -1
L 61300 72300 61300 72100 3 0 1 0 -1 -1
L 61300 71900 61300 71700 3 0 1 0 -1 -1
L 61300 71500 61300 71300 3 0 1 0 -1 -1
L 61300 71100 61300 70900 3 0 1 0 -1 -1
L 61300 70700 61300 70500 3 0 1 0 -1 -1
L 61300 69100 61300 68900 3 0 1 0 -1 -1
L 61300 68700 61300 68500 3 0 1 0 -1 -1
L 61300 68300 61300 68100 3 0 1 0 -1 -1
L 61300 67900 61300 67700 3 0 1 0 -1 -1
L 61300 67500 61300 67300 3 0 1 0 -1 -1
L 61300 67100 61300 66900 3 0 1 0 -1 -1
L 61300 66700 61300 66500 3 0 1 0 -1 -1
L 61300 66300 61300 66100 3 0 1 0 -1 -1
L 61300 65900 61300 65700 3 0 1 0 -1 -1
L 58400 65400 58400 65300 3 0 1 0 -1 -1
L 58400 65100 58400 64900 3 0 1 0 -1 -1
L 58400 64700 58400 64500 3 0 1 0 -1 -1
L 58400 64300 58400 64100 3 0 1 0 -1 -1
L 58400 63900 58400 63700 3 0 1 0 -1 -1
L 58400 63500 58400 63300 3 0 1 0 -1 -1
L 58400 63100 58400 62900 3 0 1 0 -1 -1
L 58400 62700 58400 62500 3 0 1 0 -1 -1
L 58400 62300 58400 62100 3 0 1 0 -1 -1
L 58400 61900 58400 61700 3 0 1 0 -1 -1
L 58400 61500 58400 61300 3 0 1 0 -1 -1
L 58400 61100 58400 60900 3 0 1 0 -1 -1
L 58400 60700 58400 60500 3 0 1 0 -1 -1
L 58400 60300 58400 60100 3 0 1 0 -1 -1
L 58400 59900 58400 59700 3 0 1 0 -1 -1
L 58400 59500 58400 59300 3 0 1 0 -1 -1
T 48100 64300 9 18 1 0 0 0 1
FARADAY SHIELDS
L 64000 65400 64000 65300 3 0 1 0 -1 -1
L 64000 65100 64000 64900 3 0 1 0 -1 -1
L 64000 64700 64000 64500 3 0 1 0 -1 -1
L 64000 64300 64000 64100 3 0 1 0 -1 -1
L 64000 63900 64000 63700 3 0 1 0 -1 -1
L 64000 63500 64000 63300 3 0 1 0 -1 -1
L 64000 63100 64000 62900 3 0 1 0 -1 -1
L 64000 62700 64000 62500 3 0 1 0 -1 -1
L 64000 62300 64000 62100 3 0 1 0 -1 -1
L 64000 61900 64000 61700 3 0 1 0 -1 -1
L 64000 61500 64000 61300 3 0 1 0 -1 -1
L 64000 61100 64000 60900 3 0 1 0 -1 -1
L 64000 60700 64000 60500 3 0 1 0 -1 -1
L 64000 60300 64000 60100 3 0 1 0 -1 -1
L 64000 59900 64000 59700 3 0 1 0 -1 -1
L 64000 59500 64000 59300 3 0 1 0 -1 -1
T 54100 64100 9 18 1 0 0 0 2
  INTERFACE TO
POWER SUPPLIES
T 59900 64100 9 18 1 0 0 0 2
INTERFACE TO
    LOG AMPS
T 64800 64300 9 18 1 0 0 0 1
ADC REFERENCE
T 39050 60550 9 12 1 0 0 0 3
WHEN USING THE METRO MINI THE IO LEVEL
SELECTION              BE CHANGED TO +3.3V
ON THE MINI ITSELF
T 40400 60800 2 12 1 0 0 0 1
MUST
C 59800 72200 1 180 0 capacitor-1_small.sym
{
T 59600 71500 5 10 0 0 180 0 1
device=CAPACITOR
T 59200 71800 5 10 1 1 0 0 1
refdes=C3
T 59600 71800 5 10 1 1 0 0 1
value=0.1 uF
T 59800 72200 5 10 0 1 180 0 1
footprint=0402
T 59800 72200 5 10 0 1 180 0 1
man-part-num=885012205037
}
C 60100 72000 1 90 0 gnd-1.sym
C 66500 67900 1 180 0 capacitor-1_small.sym
{
T 66375 67275 5 10 0 0 180 0 1
device=CAPACITOR
T 65900 68000 5 10 1 1 0 0 1
refdes=C8
T 66300 68000 5 10 1 1 0 0 1
value=10 uF
T 66500 67900 5 10 0 0 90 0 1
footprint=0603
T 66500 67900 5 10 0 0 90 0 1
man-part-num=GRM188R61C106MA73D
}
N 59000 72100 59200 72100 4
N 59100 72300 59100 72100 4
C 56500 69700 1 0 0 3.3V-plus-1.sym
N 65700 67800 65900 67800 4
C 66800 72500 1 90 0 gnd-1.sym
C 65600 73100 1 0 0 3.3V-plus-1.sym
N 65800 73100 65800 72600 4
C 66500 72700 1 180 0 capacitor-1_small.sym
{
T 66375 72075 5 10 0 0 180 0 1
device=CAPACITOR
T 66500 72700 5 10 0 0 90 0 1
footprint=0603
T 66500 72700 5 10 0 0 90 0 1
man-part-num=GRM188R61C106MA73D
T 65900 72800 5 10 1 1 0 0 1
refdes=C6
T 66300 72800 5 10 1 1 0 0 1
value=10 uF
}
N 65700 72600 65900 72600 4
C 65800 61600 1 270 0 capacitor-1_small.sym
{
T 66500 61400 5 10 0 0 270 0 1
device=CAPACITOR
T 65800 61600 5 10 0 1 270 0 1
footprint=0402
T 65800 61600 5 10 0 1 270 0 1
man-part-num=885012205037
T 65300 61400 5 10 1 1 0 0 1
refdes=C9
T 65300 61000 5 10 1 1 0 0 1
value=0.1 uF
}
C 63500 73400 1 180 0 capacitor-1_small.sym
{
T 63300 72700 5 10 0 0 180 0 1
device=CAPACITOR
T 62900 73500 5 10 1 1 0 0 1
refdes=C4
T 63300 73500 5 10 1 1 0 0 1
value=0.1 uF
T 63500 73400 5 10 0 1 180 0 1
footprint=0402
T 63500 73400 5 10 0 1 180 0 1
man-part-num=885012205037
}
C 63500 68600 1 180 0 capacitor-1_small.sym
{
T 63300 67900 5 10 0 0 180 0 1
device=CAPACITOR
T 62900 68700 5 10 1 1 0 0 1
refdes=C7
T 63300 68700 5 10 1 1 0 0 1
value=0.1 uF
T 63500 68600 5 10 0 1 180 0 1
footprint=0402
T 63500 68600 5 10 0 1 180 0 1
man-part-num=885012205037
}
C 56300 72000 1 0 0 resistor-1.sym
{
T 56600 72400 5 10 0 0 0 0 1
device=RESISTOR
T 56500 72225 5 10 1 1 0 0 1
refdes=R3
T 56300 72000 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 56500 71825 5 10 1 1 0 0 1
value=100 Ω
T 56300 72000 5 10 0 1 0 0 1
footprint=0402
}
N 37300 63000 37400 63000 4
{
T 36375 62950 5 10 1 1 0 0 1
netname=PLL_MUX
}
N 37300 62400 37400 62400 4
{
T 37175 62350 5 10 1 1 0 6 1
netname=SEL_LO3
}
N 37300 62700 37400 62700 4
{
T 37175 62650 5 10 1 1 0 6 1
netname=SEL_LO1
}
N 37300 62100 37400 62100 4
{
T 36200 62050 5 10 1 1 0 0 1
netname=SEL_ATTEN
}
C 57200 67700 1 180 0 resistor-1.sym
{
T 56900 67300 5 10 0 0 180 0 1
device=RESISTOR
T 56500 67725 5 10 1 1 0 0 1
refdes=R5
T 57200 67700 5 10 0 0 180 0 1
man-part-num=ERJ-2GEJ101X
T 56500 67325 5 10 1 1 0 0 1
value=100 Ω
T 57200 67700 5 10 0 1 180 0 1
footprint=0402
}
C 37400 62900 1 0 0 resistor-1.sym
{
T 37700 63300 5 10 0 0 0 0 1
device=RESISTOR
T 37300 63075 5 10 1 1 0 0 1
refdes=R6
T 37400 62900 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 38050 63075 5 10 1 1 0 0 1
value=100 Ω
T 37400 62900 5 10 0 1 0 0 1
footprint=0402
}
C 37400 62600 1 0 0 resistor-1.sym
{
T 37700 63000 5 10 0 0 0 0 1
device=RESISTOR
T 37300 62775 5 10 1 1 0 0 1
refdes=R7
T 37400 62600 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 38050 62775 5 10 1 1 0 0 1
value=100 Ω
T 37400 62600 5 10 0 1 0 0 1
footprint=0402
}
C 37400 62300 1 0 0 resistor-1.sym
{
T 37700 62700 5 10 0 0 0 0 1
device=RESISTOR
T 37300 62475 5 10 1 1 0 0 1
refdes=R8
T 37400 62300 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 38050 62475 5 10 1 1 0 0 1
value=100 Ω
T 37400 62300 5 10 0 1 0 0 1
footprint=0402
}
C 37400 62000 1 0 0 resistor-1.sym
{
T 37700 62400 5 10 0 0 0 0 1
device=RESISTOR
T 37300 62175 5 10 1 1 0 0 1
refdes=R9
T 37400 62000 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 38050 62175 5 10 1 1 0 0 1
value=100 Ω
T 37400 62000 5 10 0 1 0 0 1
footprint=0402
}
N 38300 63000 39950 63000 4
{
T 38950 63025 5 10 1 1 0 0 1
netname=MUX
}
N 38300 62400 39950 62400 4
{
T 39350 62425 5 10 1 1 0 6 1
netname=LO3
}
N 38300 62700 39950 62700 4
{
T 39350 62725 5 10 1 1 0 6 1
netname=LO1
}
N 38300 62100 39950 62100 4
{
T 38775 62125 5 10 1 1 0 0 1
netname=ATTEN
}
C 43650 65600 1 0 0 resistor-1.sym
{
T 43950 66000 5 10 0 0 0 0 1
device=RESISTOR
T 43800 65825 5 10 1 1 0 0 1
refdes=R2
T 43650 65600 5 10 0 0 0 0 1
man-part-num=ERJ-6GEY0R00V
T 44150 65825 5 10 1 1 0 0 1
value=0 Ω
T 43650 65600 5 10 0 1 0 0 1
footprint=WN2A_0805
}
N 42550 65700 43650 65700 4
{
T 43550 65750 5 10 1 1 0 6 1
netname=R2<->R17
}
N 44550 65700 45700 65700 4
{
T 44650 65750 5 10 1 1 0 0 1
netname=SPI_MISO
}
T 42950 65600 8 10 0 1 90 6 1
netname=MISO
C 45700 65700 1 270 1 busripper-1.sym
{
T 46100 65700 5 8 0 0 270 6 1
device=none
}
N 52100 72300 53350 72300 4
{
T 53150 72350 5 10 1 1 0 6 1
netname=R2<->R17
}
C 54400 71900 1 270 1 busripper-1.sym
{
T 54800 71900 5 8 0 0 270 6 1
device=none
}
C 51300 71000 1 0 0 resistor-1.sym
{
T 51600 71400 5 10 0 0 0 0 1
device=RESISTOR
T 51400 71225 5 10 1 1 0 0 1
refdes=R11
T 51300 71000 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 51900 71225 5 10 1 1 0 0 1
value=100 Ω
T 51300 71000 5 10 0 1 0 0 1
footprint=0402
}
N 51300 71100 50900 71100 4
{
T 51200 71150 5 10 0 0 0 6 1
netname=REF_2
}
C 51300 70600 1 0 0 resistor-1.sym
{
T 51600 71000 5 10 0 0 0 0 1
device=RESISTOR
T 51400 70425 5 10 1 1 0 0 1
refdes=R12
T 51300 70600 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 51900 70425 5 10 1 1 0 0 1
value=100 Ω
T 51300 70600 5 10 0 1 0 0 1
footprint=0402
}
C 51400 68400 1 0 0 resistor-1.sym
{
T 51700 68800 5 10 0 0 0 0 1
device=RESISTOR
T 51500 68625 5 10 1 1 0 0 1
refdes=R13
T 51400 68400 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 52000 68625 5 10 1 1 0 0 1
value=100 Ω
T 51400 68400 5 10 0 1 0 0 1
footprint=0402
}
C 65800 71300 1 0 0 resistor-1.sym
{
T 66100 71700 5 10 0 0 0 0 1
device=RESISTOR
T 66000 71525 5 10 1 1 0 0 1
refdes=R14
T 65800 71300 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 66000 71125 5 10 1 1 0 0 1
value=100 Ω
T 65800 71300 5 10 0 1 0 0 1
footprint=0402
}
C 65800 66500 1 0 0 resistor-1.sym
{
T 66100 66900 5 10 0 0 0 0 1
device=RESISTOR
T 66000 66725 5 10 1 1 0 0 1
refdes=R15
T 65800 66500 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 66000 66325 5 10 1 1 0 0 1
value=100 Ω
T 65800 66500 5 10 0 1 0 0 1
footprint=0402
}
N 51300 70700 50900 70700 4
{
T 51200 70750 5 10 0 0 0 6 1
netname=REF_1
}
N 65800 71400 65700 71400 4
N 65800 66600 65700 66600 4
N 51400 68500 50900 68500 4
C 39100 72200 1 270 0 gnd-1.sym
C 39150 64900 1 270 0 gnd-1.sym
C 39150 64600 1 270 0 gnd-1.sym
N 39450 64500 39950 64500 4
N 39450 64800 39950 64800 4
N 39400 72100 39950 72100 4
C 51200 72600 1 0 0 resistor-1.sym
{
T 51500 73000 5 10 0 0 0 0 1
device=RESISTOR
T 51250 72825 5 10 1 1 0 0 1
refdes=R16
T 51650 72825 5 10 1 1 0 0 1
value=100 Ω
T 51200 72600 5 10 0 1 0 0 1
footprint=0402
T 51200 72600 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
}
C 51200 72200 1 0 0 resistor-1.sym
{
T 51500 72600 5 10 0 0 0 0 1
device=RESISTOR
T 51250 72425 5 10 1 1 0 0 1
refdes=R17
T 51650 72425 5 10 1 1 0 0 1
value=100 Ω
T 51200 72200 5 10 0 1 0 0 1
footprint=0402
T 51200 72200 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
}
C 51200 71800 1 0 0 resistor-1.sym
{
T 51500 72200 5 10 0 0 0 0 1
device=RESISTOR
T 51250 72025 5 10 1 1 0 0 1
refdes=R18
T 51650 72025 5 10 1 1 0 0 1
value=100 Ω
T 51200 71800 5 10 0 1 0 0 1
footprint=0402
T 51200 71800 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
}
N 39400 71800 39950 71800 4
{
T 38450 71750 5 10 1 1 0 0 1
netname=\_SEL_RAM\_
}
N 39400 71500 39950 71500 4
{
T 38250 71450 5 10 1 1 0 0 1
netname=\_SEL_FLASH\_
}
N 37300 63600 39950 63600 4
{
T 36350 63550 5 10 1 1 0 0 1
netname=\_SEL_RAM\_
}
N 37300 63300 39950 63300 4
{
T 36150 63250 5 10 1 1 0 0 1
netname=\_SEL_FLASH\_
}
N 51200 72700 50900 72700 4
N 51200 72300 50900 72300 4
N 51200 71900 50900 71900 4
C 58100 74200 1 180 0 resistor-1.sym
{
T 57800 73800 5 10 0 0 180 0 1
device=RESISTOR
T 57150 74225 5 10 1 1 0 0 1
refdes=R19
T 57550 74225 5 10 1 1 0 0 1
value=100 Ω
T 58100 74200 5 10 0 1 180 0 1
footprint=0402
T 58100 74200 5 10 0 0 180 0 1
man-part-num=ERJ-2GEJ101X
}
N 57200 74100 54800 74100 4
{
T 55800 74350 5 10 1 1 180 0 1
netname=SPI_MISO
}
C 58100 73700 1 180 0 resistor-1.sym
{
T 57800 73300 5 10 0 0 180 0 1
device=RESISTOR
T 57150 73725 5 10 1 1 0 0 1
refdes=R21
T 57650 73725 5 10 1 1 0 0 1
value=100 Ω
T 58100 73700 5 10 0 1 180 0 1
footprint=0402
T 58100 73700 5 10 0 0 180 0 1
man-part-num=ERJ-2GEJ101X
}
C 58100 73200 1 180 0 resistor-1.sym
{
T 57800 72800 5 10 0 0 180 0 1
device=RESISTOR
T 57150 73225 5 10 1 1 0 0 1
refdes=R20
T 57650 73225 5 10 1 1 0 0 1
value=100 Ω
T 58100 73200 5 10 0 1 180 0 1
footprint=0402
T 58100 73200 5 10 0 0 180 0 1
man-part-num=ERJ-2GEJ101X
}
N 60400 71300 59000 71300 4
{
T 60200 71350 5 10 1 1 0 6 1
netname=SPICLK
}
N 60400 70900 59000 70900 4
{
T 60200 70950 5 10 1 1 0 6 1
netname=MOSI
}
N 60400 66800 59000 66800 4
{
T 60200 66850 5 10 1 1 0 6 1
netname=SPICLK
}
N 60400 66400 59000 66400 4
{
T 60225 66450 5 10 1 1 0 6 1
netname=MOSI
}
U 60600 74400 60600 65800 10 1
C 60400 71300 1 270 1 busripper-1.sym
{
T 60800 71300 5 8 0 0 270 6 1
device=none
}
C 60400 70900 1 270 1 busripper-1.sym
{
T 60800 70900 5 8 0 0 270 6 1
device=none
}
C 60400 66400 1 270 1 busripper-1.sym
{
T 60800 66400 5 8 0 0 270 6 1
device=none
}
C 60400 66800 1 270 1 busripper-1.sym
{
T 60800 66800 5 8 0 0 270 6 1
device=none
}
C 60400 70400 1 270 1 busripper-1.sym
{
T 60800 70400 5 8 0 0 270 6 1
device=none
}
N 57200 71700 56700 71700 4
N 56700 70400 56700 71700 4
N 57200 67200 56700 67200 4
N 67900 72200 65700 72200 4
{
T 66700 72250 5 10 1 1 0 6 1
netname=SPICLK
}
N 67900 67400 65700 67400 4
{
T 66700 67450 5 10 1 1 0 6 1
netname=SPICLK
}
C 54200 62300 1 0 0 7.5V-plus-1.sym
N 56000 62300 54400 62300 4
{
T 55600 62350 5 10 1 1 0 6 1
netname=+7.5VDC
}
N 39950 64200 37900 64200 4
{
T 39100 64250 5 10 1 1 0 6 1
netname=+7.5VDC
}
C 37200 65000 1 0 0 7.5V-plus-1.sym
C 37300 64100 1 0 0 gnd-1.sym
N 37400 65000 37900 65000 4
N 37900 65000 37900 64200 4
C 37300 65000 1 270 0 capacitor-1_small.sym
{
T 38000 64800 5 10 0 0 270 0 1
device=CAPACITOR
T 37000 64800 5 10 1 1 0 0 1
refdes=C1
T 36800 64500 5 10 1 1 0 0 1
value=0.1 uF
T 37300 65000 5 10 0 1 270 0 1
footprint=0402
T 37300 65000 5 10 0 1 270 0 1
man-part-num=885012205037
}
N 56700 65900 60400 65900 4
{
T 59800 65950 5 10 1 1 0 0 1
netname=MISO
}
C 60400 65900 1 270 1 busripper-1.sym
{
T 60800 65900 5 8 0 0 270 6 1
device=none
}
N 56700 67200 56700 65900 4
L 47100 65500 46900 65500 3 0 1 0 -1 -1
L 46700 65500 46600 65500 3 0 1 0 -1 -1
N 58100 73600 60400 73600 4
{
T 59500 73850 5 10 1 1 180 6 1
netname=SPICLK
}
N 58100 73100 60400 73100 4
{
T 59700 73350 5 10 1 1 180 6 1
netname=MOSI
}
N 60400 74100 58100 74100 4
{
T 60150 74350 5 10 1 1 180 0 1
netname=MISO
}
U 68100 74400 68100 66800 10 1
C 67900 67400 1 270 1 busripper-1.sym
{
T 68300 67400 5 8 0 0 270 6 1
device=none
}
C 67900 67000 1 270 1 busripper-1.sym
{
T 68300 67000 5 8 0 0 270 6 1
device=none
}
C 67900 71800 1 270 1 busripper-1.sym
{
T 68300 71800 5 8 0 0 270 6 1
device=none
}
C 67900 72200 1 270 1 busripper-1.sym
{
T 68300 72200 5 8 0 0 270 6 1
device=none
}
C 60400 74100 1 270 1 busripper-1.sym
{
T 60800 74100 5 8 0 0 270 6 1
device=none
}
C 60400 73100 1 270 1 busripper-1.sym
{
T 60800 73100 5 8 0 0 270 6 1
device=none
}
C 60400 73600 1 270 1 busripper-1.sym
{
T 60800 73600 5 8 0 0 270 6 1
device=none
}
C 54800 74100 1 90 0 busripper-1.sym
{
T 54400 74100 5 8 0 0 90 0 1
device=none
}
C 54800 73100 1 90 0 busripper-1.sym
{
T 54400 73100 5 8 0 0 90 0 1
device=none
}
C 54800 73600 1 90 0 busripper-1.sym
{
T 54400 73600 5 8 0 0 90 0 1
device=none
}
U 60600 74400 68100 74400 10 0
L 61300 70300 61300 70100 3 0 1 0 -1 -1
L 61300 69900 61300 69700 3 0 1 0 -1 -1
L 61300 69500 61300 69300 3 0 1 0 -1 -1
N 39400 69400 41550 69400 4
{
T 38450 69350 5 10 1 1 0 0 1
netname=SPI_MISO
}
N 39400 69700 41250 69700 4
{
T 38450 69650 5 10 1 1 0 0 1
netname=SPI_MOSI
}
N 39400 70000 40950 70000 4
{
T 38550 69950 5 10 1 1 0 0 1
netname=SPI_CLK
}
C 68400 61800 1 180 0 nc-left-1.sym
{
T 68400 61400 5 10 0 0 180 0 1
value=NoConnection
T 68400 61000 5 10 0 0 180 0 1
device=DRC_Directive
}
C 51900 73600 1 180 0 nc-left-1.sym
{
T 51900 73200 5 10 0 0 180 0 1
value=NoConnection
T 51900 72800 5 10 0 0 180 0 1
device=DRC_Directive
}
N 51400 73500 50900 73500 4
C 51900 67800 1 180 0 nc-left-1.sym
{
T 51900 67400 5 10 0 0 180 0 1
value=NoConnection
T 51900 67000 5 10 0 0 180 0 1
device=DRC_Directive
}
N 51400 67700 50900 67700 4
C 51900 67400 1 180 0 nc-left-1.sym
{
T 51900 67000 5 10 0 0 180 0 1
value=NoConnection
T 51900 66600 5 10 0 0 180 0 1
device=DRC_Directive
}
N 51400 67300 50900 67300 4
N 52200 69700 52700 69700 4
{
T 52800 69650 5 10 1 1 0 0 1
netname=PWR_DETECT
}
N 42550 70600 43100 70600 4
{
T 43200 70550 5 10 1 1 0 0 1
netname=PWR_DETECT
}
N 42550 64200 43050 64200 4
{
T 43150 64150 5 10 1 1 0 0 1
netname=PWR_DETECT
}
C 51300 69600 1 0 0 resistor-1.sym
{
T 51600 70000 5 10 0 0 0 0 1
device=RESISTOR
T 51400 69825 5 10 1 1 0 0 1
refdes=R22
T 51300 69600 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 51900 69825 5 10 1 1 0 0 1
value=100 Ω
T 51300 69600 5 10 0 1 0 0 1
footprint=0402
}
N 51300 69700 50900 69700 4
C 36200 67300 1 0 0 switch-pushbutton-no-1.sym
{
T 36600 67600 5 10 1 1 0 0 1
refdes=S1
T 36600 67900 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
T 36200 67300 5 10 0 0 0 0 1
footprint=SM4_TOP
}
C 35800 66700 1 0 0 gnd-1.sym
N 36200 67300 35900 67300 4
N 35900 67300 35900 67000 4
N 39400 73000 39950 73000 4
{
T 38700 72950 5 10 1 1 0 0 1
netname=\_RESET\_
}
N 37300 65700 39950 65700 4
{
T 36600 65650 5 10 1 1 0 0 1
netname=\_RESET\_
}
N 38400 67300 38950 67300 4
{
T 39100 67250 5 10 1 1 0 0 1
netname=\_RESET\_
}
C 37500 67200 1 0 0 resistor-1.sym
{
T 37800 67600 5 10 0 0 0 0 1
device=RESISTOR
T 37700 67475 5 10 1 1 0 0 1
refdes=R23
T 37500 67200 5 10 0 0 0 0 1
man-part-num=ERJ-2GEJ101X
T 37700 67025 5 10 1 1 0 0 1
value=100 Ω
T 37500 67200 5 10 0 1 0 0 1
footprint=0402
}
N 37500 67300 37200 67300 4
C 48600 62400 1 0 0 Faraday-cage.sym
{
T 48600 63200 5 10 0 0 0 0 1
device=SHEILD
T 48600 63000 5 10 1 1 0 0 1
refdes=Sh1
T 48600 62400 5 10 0 1 0 0 1
footprint=Harwin_S097146R
}
N 51200 62900 48000 62900 4
N 48000 62900 48000 61100 4
N 48000 61100 51200 61100 4
N 51200 62000 48000 62000 4
N 47600 62000 48000 62000 4
C 51000 62400 1 0 0 Faraday-cage.sym
{
T 51000 63200 5 10 0 0 0 0 1
device=SHEILD
T 51000 63000 5 10 1 1 0 0 1
refdes=Sh5
T 51000 62400 5 10 0 1 0 0 1
footprint=Harwin_S097146R
}
C 50400 62400 1 0 0 Faraday-cage.sym
{
T 50400 63200 5 10 0 0 0 0 1
device=SHEILD
T 50400 63000 5 10 1 1 0 0 1
refdes=Sh4
T 50400 62400 5 10 0 1 0 0 1
footprint=Harwin_S097146R
}
C 49800 62400 1 0 0 Faraday-cage.sym
{
T 49800 63200 5 10 0 0 0 0 1
device=SHEILD
T 49800 63000 5 10 1 1 0 0 1
refdes=Sh3
T 49800 62400 5 10 0 1 0 0 1
footprint=Harwin_S097146R
}
C 49200 62400 1 0 0 Faraday-cage.sym
{
T 49200 63200 5 10 0 0 0 0 1
device=SHEILD
T 49200 63000 5 10 1 1 0 0 1
refdes=Sh2
T 49200 62400 5 10 0 1 0 0 1
footprint=Harwin_S097146R
}
C 48600 61500 1 0 0 Faraday-cage.sym
{
T 48600 62300 5 10 0 0 0 0 1
device=SHEILD
T 48600 61500 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 48600 62100 5 10 1 1 0 0 1
refdes=Sh6
}
C 51000 61500 1 0 0 Faraday-cage.sym
{
T 51000 62300 5 10 0 0 0 0 1
device=SHEILD
T 51000 61500 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 51000 62100 5 10 1 1 0 0 1
refdes=Sh10
}
C 50400 61500 1 0 0 Faraday-cage.sym
{
T 50400 62300 5 10 0 0 0 0 1
device=SHEILD
T 50400 61500 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 50400 62100 5 10 1 1 0 0 1
refdes=Sh9
}
C 49800 61500 1 0 0 Faraday-cage.sym
{
T 49800 62300 5 10 0 0 0 0 1
device=SHEILD
T 49800 61500 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 49800 62100 5 10 1 1 0 0 1
refdes=Sh8
}
C 49200 61500 1 0 0 Faraday-cage.sym
{
T 49200 62300 5 10 0 0 0 0 1
device=SHEILD
T 49200 61500 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 49200 62100 5 10 1 1 0 0 1
refdes=Sh7
}
C 49200 60600 1 0 0 Faraday-cage.sym
{
T 49200 61400 5 10 0 0 0 0 1
device=SHEILD
T 49200 60600 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 49200 61200 5 10 1 1 0 0 1
refdes=Sh11
}
C 51000 60600 1 0 0 Faraday-cage.sym
{
T 51000 61400 5 10 0 0 0 0 1
device=SHEILD
T 51000 60600 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 51000 61200 5 10 1 1 0 0 1
refdes=Sh14
}
C 50400 60600 1 0 0 Faraday-cage.sym
{
T 50400 61400 5 10 0 0 0 0 1
device=SHEILD
T 50400 60600 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 50400 61200 5 10 1 1 0 0 1
refdes=Sh13
}
C 49800 60600 1 0 0 Faraday-cage.sym
{
T 49800 61400 5 10 0 0 0 0 1
device=SHEILD
T 49800 60600 5 10 0 1 0 0 1
footprint=Harwin_S097146R
T 49800 61200 5 10 1 1 0 0 1
refdes=Sh12
}
C 57200 66100 1 0 0 WSON_8_6mm.sym
{
T 58495 68000 5 10 1 1 0 0 1
refdes=U5
T 57392 68000 5 10 1 1 0 0 1
device=FLASH
T 57392 68300 5 10 0 0 0 0 1
footprint=WSON_8
T 57200 66400 5 10 0 0 0 0 1
man-part-num=W25N01GVZEIG TR
}
N 56700 69700 56700 69500 4
N 56700 69500 57100 69500 4
N 57000 69500 57000 68900 4
N 57000 68900 57100 68900 4
C 59000 68500 1 0 0 gnd-1.sym
C 57100 68900 1 0 0 TPS7A02_+1.8REG.sym
{
T 57400 69800 5 10 1 1 0 0 1
device=TPS7A02
T 58800 69800 5 10 1 1 0 6 1
refdes=U8
T 57400 68500 5 10 1 1 0 0 1
description=1.8 VDC
T 57100 68900 5 10 0 0 0 0 1
footprint=TPS7A02
T 57100 68900 5 10 0 0 0 0 1
man-part-num=TPS7A0218YCHR
}
N 59100 69500 59400 69500 4
N 59400 69500 59400 67600 4
N 59400 67600 59000 67600 4
{
T 59300 67650 5 10 0 0 0 6 1
netname=1.8VDC
}
C 56800 68900 1 90 0 capacitor-1_small.sym
{
T 56100 69100 5 10 0 1 90 0 1
device=CAPACITOR
T 56800 68900 5 10 0 1 0 0 1
footprint=0402
T 56800 68900 5 10 0 1 0 0 1
man-part-num=0402W105K100HI
T 56300 69300 5 10 1 1 0 0 1
refdes=C2
T 56100 68900 5 10 1 1 0 0 1
value=1.0 uF
}
C 59900 68200 1 90 0 capacitor-1_small.sym
{
T 59200 68400 5 10 0 1 90 0 1
device=CAPACITOR
T 59900 68200 5 10 0 1 0 0 1
footprint=0402
T 59900 68200 5 10 0 1 0 0 1
man-part-num=0402W105K100HI
T 59900 68600 5 10 1 1 0 0 1
refdes=C5
T 59900 68200 5 10 1 1 0 0 1
value=1.0 uF
}
C 56600 68500 1 0 0 gnd-1.sym
C 59700 67800 1 0 0 gnd-1.sym
N 59800 68800 59400 68800 4
N 59800 68100 59800 68200 4
N 56700 68800 56700 68900 4
N 59100 68800 59100 68900 4
