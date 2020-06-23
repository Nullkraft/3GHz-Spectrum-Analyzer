README for PROJECT#1 Spectrum Analyzer 22 June 2020- WN2A

A spectrum analyzer compatible with Arduino UNO R3 Microcontroller and others.

1) Schematics: are	spectrumwvi-page1.sch
					specrtumwvi-page2.sch
					specrtumwvi-page3.sch
					specrtumwvi-page4.sch
					specrtumwvi-page5.sch
    
   A combination of Mark Stanley/Mike Masterson gEDA gschem schematics
    
2) BOM is Spectrum_Analyzer_gnetlist_BOM_3-28-20.csv or Spectrum_Analyzer_gnetlist_BOM_3-28-20.ods

3) Design is intended to meet preliminary specifications:
   Frequency Range: 23.5 to 3000 MHz.
   Spurious Free Dynamic Range: 70 dBC
   Powered either by External +7 ~ +13V (Schottky Diode OR'd)
   Arduino driver provided allowing for serial command & control
   Dual XTAL Reference allows minimizing Frac-N spurs.
   Individual AZ1117 low-noise regulators for Digital,RF and VCO
   Compatible with various Arduinos using the UNO pin arrangement
