EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Entry Wire Line
	4200 3200 4300 3100
Entry Wire Line
	4200 3600 4300 3500
Entry Wire Line
	4200 3800 4300 3700
Entry Wire Line
	4200 4000 4300 3900
Entry Wire Line
	4200 4200 4300 4100
Entry Wire Line
	4200 4400 4300 4300
Wire Bus Line
	4200 3050 3850 3050
Text Label 4400 4300 0    50   ~ 0
P5
Text Label 4400 4100 0    50   ~ 0
P4
Text Label 4400 3900 0    50   ~ 0
P3
Text Label 4400 3700 0    50   ~ 0
P2
Text Label 4400 3500 0    50   ~ 0
P1
Text Label 4400 3100 0    50   ~ 0
P0
Connection ~ 5450 4100
Connection ~ 5450 3900
Wire Wire Line
	5450 3900 5450 4100
Connection ~ 5450 3700
Wire Wire Line
	5450 3700 5450 3900
Connection ~ 5450 3500
Wire Wire Line
	5450 3500 5450 3700
$Comp
L MC34119:MC34119 IC1
U 1 1 622D64B3
P 8750 3700
F 0 "IC1" H 9127 3746 50  0000 L CNN
F 1 "MC34119" H 9127 3655 50  0000 L CNN
F 2 "MC34119:DIL08" H 8750 3700 50  0001 C CNN
F 3 "" H 8750 3700 50  0001 L BNN
	1    8750 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 622D7C1C
P 8350 3800
F 0 "#PWR010" H 8350 3550 50  0001 C CNN
F 1 "GND" H 8355 3627 50  0000 C CNN
F 2 "" H 8350 3800 50  0001 C CNN
F 3 "" H 8350 3800 50  0001 C CNN
	1    8350 3800
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 622D8C24
P 7700 3600
F 0 "C2" V 7448 3600 50  0000 C CNN
F 1 "0.1uF" V 7539 3600 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 7738 3450 50  0001 C CNN
F 3 "~" H 7700 3600 50  0001 C CNN
	1    7700 3600
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 622D9C79
P 8100 3600
AR Path="/622D9C79" Ref="R?"  Part="1" 
AR Path="/622C729F/622D9C79" Ref="R23"  Part="1" 
F 0 "R23" V 8200 3600 50  0000 C CNN
F 1 "20k" V 8100 3600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8030 3600 50  0001 C CNN
F 3 "~" H 8100 3600 50  0001 C CNN
	1    8100 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	8250 3600 8300 3600
$Comp
L Device:Speaker LS1
U 1 1 622DAE85
P 10250 3650
F 0 "LS1" H 10420 3646 50  0000 L CNN
F 1 "Speaker" H 10420 3555 50  0000 L CNN
F 2 "AST-03008MR-R:AST-03008MR-R" H 10250 3450 50  0001 C CNN
F 3 "~" H 10240 3600 50  0001 C CNN
	1    10250 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 3500 9050 3450
Wire Wire Line
	9050 3450 9300 3450
Wire Wire Line
	9050 3950 9050 3900
$Comp
L Device:CP C3
U 1 1 622DCD06
P 8500 4300
F 0 "C3" H 8300 4350 50  0000 L CNN
F 1 "1uF" H 8250 4250 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 8538 4150 50  0001 C CNN
F 3 "~" H 8500 4300 50  0001 C CNN
	1    8500 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 622DFC88
P 8700 4300
F 0 "C4" H 8800 4350 50  0000 L CNN
F 1 "4.7uF" H 8800 4250 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 8738 4150 50  0001 C CNN
F 3 "~" H 8700 4300 50  0001 C CNN
	1    8700 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 622E1125
P 8500 4600
F 0 "#PWR012" H 8500 4350 50  0001 C CNN
F 1 "GND" H 8505 4427 50  0000 C CNN
F 2 "" H 8500 4600 50  0001 C CNN
F 3 "" H 8500 4600 50  0001 C CNN
	1    8500 4600
	1    0    0    -1  
$EndComp
Connection ~ 8500 4450
Wire Wire Line
	8500 4450 8500 4600
Wire Wire Line
	8700 4450 8500 4450
Wire Wire Line
	8550 4000 8550 4050
Wire Wire Line
	8550 4050 8500 4050
Wire Wire Line
	8500 4050 8500 4150
Wire Wire Line
	8650 4000 8650 4050
Wire Wire Line
	8650 4050 8700 4050
Wire Wire Line
	8700 4050 8700 4150
$Comp
L power:GND #PWR014
U 1 1 622E6758
P 8850 4000
F 0 "#PWR014" H 8850 3750 50  0001 C CNN
F 1 "GND" H 8855 3827 50  0000 C CNN
F 2 "" H 8850 4000 50  0001 C CNN
F 3 "" H 8850 4000 50  0001 C CNN
	1    8850 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 622E6F90
P 8850 3400
F 0 "#PWR013" H 8850 3250 50  0001 C CNN
F 1 "+5V" H 8865 3573 50  0000 C CNN
F 2 "" H 8850 3400 50  0001 C CNN
F 3 "" H 8850 3400 50  0001 C CNN
	1    8850 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT RV1
U 1 1 622E9CEF
P 8550 2900
F 0 "RV1" V 8343 2900 50  0000 C CNN
F 1 "VOL_POT" V 8434 2900 50  0000 C CNN
F 2 "SamacSys_Parts:PTV09A4020FB103" H 8550 2900 50  0001 C CNN
F 3 "~" H 8550 2900 50  0001 C CNN
	1    8550 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	8700 3050 9300 3050
Wire Wire Line
	9300 3050 9300 3450
Connection ~ 9300 3450
Text HLabel 3850 3050 0    50   Input ~ 0
P[0..7]
Wire Wire Line
	8700 2900 8700 3050
$Comp
L power:GND #PWR011
U 1 1 6242B868
P 8400 2900
F 0 "#PWR011" H 8400 2650 50  0001 C CNN
F 1 "GND" H 8405 2727 50  0000 C CNN
F 2 "" H 8400 2900 50  0001 C CNN
F 3 "" H 8400 2900 50  0001 C CNN
	1    8400 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	7850 3600 7950 3600
Wire Wire Line
	8300 3600 8300 3050
Wire Wire Line
	8300 3050 8550 3050
Connection ~ 8300 3600
Wire Wire Line
	8300 3600 8350 3600
$Comp
L Device:C C1
U 1 1 624AE3B6
P 9700 3700
F 0 "C1" H 9448 3700 50  0000 C CNN
F 1 "C" H 9539 3700 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 9738 3550 50  0001 C CNN
F 3 "~" H 9700 3700 50  0001 C CNN
	1    9700 3700
	-1   0    0    1   
$EndComp
Wire Wire Line
	5450 4100 5450 4300
Connection ~ 5450 4300
Wire Wire Line
	5450 3100 5450 3500
Text Notes 5300 3100 0    50   ~ 0
96k
Text Notes 5300 3500 0    50   ~ 0
48k
Text Notes 5300 3700 0    50   ~ 0
24k
Text Notes 5300 3900 0    50   ~ 0
12k
Text Notes 5300 4100 0    50   ~ 0
6k
Text Notes 5300 4300 0    50   ~ 0
3k
Wire Wire Line
	10050 3450 10050 3650
Wire Wire Line
	9300 3450 9700 3450
Wire Wire Line
	10050 3750 10050 3950
Wire Wire Line
	9050 3950 9700 3950
Wire Wire Line
	9700 3550 9700 3450
Connection ~ 9700 3450
Wire Wire Line
	9700 3450 10050 3450
Wire Wire Line
	9700 3850 9700 3950
Connection ~ 9700 3950
Wire Wire Line
	9700 3950 10050 3950
Wire Wire Line
	7200 3600 7550 3600
$Comp
L Device:R R?
U 1 1 624E1C64
P 4850 4300
AR Path="/624E1C64" Ref="R?"  Part="1" 
AR Path="/622C729F/624E1C64" Ref="R6"  Part="1" 
F 0 "R6" V 4950 4300 50  0000 C CNN
F 1 "3k" V 4850 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 4300 50  0001 C CNN
F 3 "~" H 4850 4300 50  0001 C CNN
	1    4850 4300
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E27C2
P 4850 4100
AR Path="/624E27C2" Ref="R?"  Part="1" 
AR Path="/622C729F/624E27C2" Ref="R5"  Part="1" 
F 0 "R5" V 4950 4100 50  0000 C CNN
F 1 "6k" V 4850 4100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 4100 50  0001 C CNN
F 3 "~" H 4850 4100 50  0001 C CNN
	1    4850 4100
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E347A
P 4850 3900
AR Path="/624E347A" Ref="R?"  Part="1" 
AR Path="/622C729F/624E347A" Ref="R4"  Part="1" 
F 0 "R4" V 4950 3900 50  0000 C CNN
F 1 "12k" V 4850 3900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 3900 50  0001 C CNN
F 3 "~" H 4850 3900 50  0001 C CNN
	1    4850 3900
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E41D1
P 4850 3700
AR Path="/624E41D1" Ref="R?"  Part="1" 
AR Path="/622C729F/624E41D1" Ref="R3"  Part="1" 
F 0 "R3" V 4950 3700 50  0000 C CNN
F 1 "24k" V 4850 3700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 3700 50  0001 C CNN
F 3 "~" H 4850 3700 50  0001 C CNN
	1    4850 3700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E57EA
P 4700 3500
AR Path="/624E57EA" Ref="R?"  Part="1" 
AR Path="/622C729F/624E57EA" Ref="R2"  Part="1" 
F 0 "R2" V 4800 3500 50  0000 C CNN
F 1 "47k" V 4700 3500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4630 3500 50  0001 C CNN
F 3 "~" H 4700 3500 50  0001 C CNN
	1    4700 3500
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E6183
P 5050 3500
AR Path="/624E6183" Ref="R?"  Part="1" 
AR Path="/622C729F/624E6183" Ref="R10"  Part="1" 
F 0 "R10" V 5150 3500 50  0000 C CNN
F 1 "1k" V 5050 3500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4980 3500 50  0001 C CNN
F 3 "~" H 5050 3500 50  0001 C CNN
	1    5050 3500
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E6EF9
P 4850 2950
AR Path="/624E6EF9" Ref="R?"  Part="1" 
AR Path="/622C729F/624E6EF9" Ref="R1"  Part="1" 
F 0 "R1" V 4950 2950 50  0000 C CNN
F 1 "100k" V 4850 2950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 2950 50  0001 C CNN
F 3 "~" H 4850 2950 50  0001 C CNN
	1    4850 2950
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E6EFF
P 4850 3200
AR Path="/624E6EFF" Ref="R?"  Part="1" 
AR Path="/622C729F/624E6EFF" Ref="R9"  Part="1" 
F 0 "R9" V 4950 3200 50  0000 C CNN
F 1 "2.4M" V 4850 3200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 3200 50  0001 C CNN
F 3 "~" H 4850 3200 50  0001 C CNN
	1    4850 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	5200 3500 5450 3500
Wire Wire Line
	4900 3500 4850 3500
Wire Wire Line
	4550 3500 4300 3500
Wire Wire Line
	4300 3700 4700 3700
Wire Wire Line
	5000 3700 5450 3700
Wire Wire Line
	5000 3900 5450 3900
Wire Wire Line
	4700 3900 4300 3900
Wire Wire Line
	4300 4100 4700 4100
Wire Wire Line
	5000 4100 5450 4100
Wire Wire Line
	5000 4300 5450 4300
Wire Wire Line
	4700 4300 4300 4300
Wire Wire Line
	5450 4300 5450 4500
$Comp
L Device:R R?
U 1 1 624E1001
P 4850 4500
AR Path="/624E1001" Ref="R?"  Part="1" 
AR Path="/622C729F/624E1001" Ref="R7"  Part="1" 
F 0 "R7" V 4950 4500 50  0000 C CNN
F 1 "1.5k" V 4850 4500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 4500 50  0001 C CNN
F 3 "~" H 4850 4500 50  0001 C CNN
	1    4850 4500
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624DDF65
P 4850 4700
AR Path="/624DDF65" Ref="R?"  Part="1" 
AR Path="/622C729F/624DDF65" Ref="R8"  Part="1" 
F 0 "R8" V 4950 4700 50  0000 C CNN
F 1 "750" V 4850 4700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4780 4700 50  0001 C CNN
F 3 "~" H 4850 4700 50  0001 C CNN
	1    4850 4700
	0    1    1    0   
$EndComp
Text Notes 5300 4700 0    50   ~ 0
750
Text Notes 5250 4500 0    50   ~ 0
1.5k
Text Label 4400 4500 0    50   ~ 0
P6
Text Label 4400 4700 0    50   ~ 0
P7
Entry Wire Line
	4200 4800 4300 4700
Entry Wire Line
	4200 4600 4300 4500
Connection ~ 5450 4500
Wire Wire Line
	5450 4500 5450 4700
Wire Wire Line
	4300 4500 4700 4500
Wire Wire Line
	5000 4500 5450 4500
Wire Wire Line
	5000 4700 5450 4700
Wire Wire Line
	4300 4700 4700 4700
Wire Wire Line
	6350 3100 6350 3400
$Comp
L power:GND #PWR08
U 1 1 622D5203
P 6350 3400
F 0 "#PWR08" H 6350 3150 50  0001 C CNN
F 1 "GND" H 6355 3227 50  0000 C CNN
F 2 "" H 6350 3400 50  0001 C CNN
F 3 "" H 6350 3400 50  0001 C CNN
	1    6350 3400
	1    0    0    -1  
$EndComp
Connection ~ 5450 3100
Text HLabel 5450 4700 2    50   Input ~ 0
DACOUT
Text HLabel 7200 3600 0    50   Input ~ 0
DACOUT
Wire Wire Line
	5000 2950 5050 2950
Wire Wire Line
	5050 2950 5050 3100
Wire Wire Line
	5050 3100 5450 3100
Wire Wire Line
	5050 3100 5050 3200
Wire Wire Line
	5050 3200 5000 3200
Connection ~ 5050 3100
Wire Wire Line
	4700 2950 4650 2950
Wire Wire Line
	4650 2950 4650 3100
Wire Wire Line
	4650 3200 4700 3200
Wire Wire Line
	4300 3100 4650 3100
Connection ~ 4650 3100
Wire Wire Line
	4650 3100 4650 3200
$Comp
L Device:R R?
U 1 1 62505042
P 5800 2950
AR Path="/62505042" Ref="R?"  Part="1" 
AR Path="/622C729F/62505042" Ref="R11"  Part="1" 
F 0 "R11" V 5900 2950 50  0000 C CNN
F 1 "100k" V 5800 2950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5730 2950 50  0001 C CNN
F 3 "~" H 5800 2950 50  0001 C CNN
	1    5800 2950
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 62505048
P 5800 3200
AR Path="/62505048" Ref="R?"  Part="1" 
AR Path="/622C729F/62505048" Ref="R12"  Part="1" 
F 0 "R12" V 5900 3200 50  0000 C CNN
F 1 "2.4M" V 5800 3200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5730 3200 50  0001 C CNN
F 3 "~" H 5800 3200 50  0001 C CNN
	1    5800 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	5550 2950 5550 3100
Wire Wire Line
	5450 3100 5550 3100
Wire Wire Line
	5550 2950 5650 2950
Wire Wire Line
	5950 2950 6000 2950
Wire Wire Line
	6000 2950 6000 3100
Wire Wire Line
	6000 3100 6350 3100
Wire Wire Line
	5950 3200 6000 3200
Wire Wire Line
	6000 3200 6000 3100
Connection ~ 6000 3100
Wire Wire Line
	5550 3100 5550 3200
Wire Wire Line
	5550 3200 5650 3200
Connection ~ 5550 3100
Wire Bus Line
	4200 3050 4200 4800
Text Notes 7250 2550 0    50   ~ 0
Note: VOL_POT range will vary depending on speaker resistance
$EndSCHEMATC
