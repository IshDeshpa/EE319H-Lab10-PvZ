EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
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
	8300 2650 8400 2750
Entry Wire Line
	8300 2750 8400 2850
Entry Wire Line
	8300 2850 8400 2950
Entry Wire Line
	8300 2950 8400 3050
Entry Wire Line
	8300 3050 8400 3150
Entry Wire Line
	8300 3150 8400 3250
Entry Wire Line
	8300 3250 8400 3350
Entry Wire Line
	8300 3350 8400 3450
Wire Bus Line
	8800 2700 8400 2700
$Comp
L power:+3.3V #PWR05
U 1 1 622D0CA9
P 6350 2400
F 0 "#PWR05" H 6350 2250 50  0001 C CNN
F 1 "+3.3V" V 6365 2528 50  0000 L CNN
F 2 "" H 6350 2400 50  0001 C CNN
F 3 "" H 6350 2400 50  0001 C CNN
	1    6350 2400
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 622D1314
P 6350 2500
F 0 "#PWR06" H 6350 2350 50  0001 C CNN
F 1 "+5V" V 6365 2628 50  0000 L CNN
F 2 "" H 6350 2500 50  0001 C CNN
F 3 "" H 6350 2500 50  0001 C CNN
	1    6350 2500
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR07
U 1 1 622D1B23
P 6350 4900
F 0 "#PWR07" H 6350 4650 50  0001 C CNN
F 1 "GND" V 6355 4772 50  0000 R CNN
F 2 "" H 6350 4900 50  0001 C CNN
F 3 "" H 6350 4900 50  0001 C CNN
	1    6350 4900
	0    -1   -1   0   
$EndComp
Text Label 8250 2850 2    50   ~ 0
DAC2
Text Label 8250 2950 2    50   ~ 0
DAC3
Text Label 8250 3050 2    50   ~ 0
DAC4
Text Label 8250 3150 2    50   ~ 0
DAC5
Text Label 8250 3250 2    50   ~ 0
DAC6
Text Label 8250 3350 2    50   ~ 0
DAC7
Wire Wire Line
	8250 2650 8300 2650
Wire Wire Line
	8250 2750 8300 2750
Wire Wire Line
	8250 2850 8300 2850
Wire Wire Line
	8250 2950 8300 2950
Wire Wire Line
	8250 3050 8300 3050
Wire Wire Line
	8250 3150 8300 3150
Wire Wire Line
	8250 3250 8300 3250
Wire Wire Line
	8250 3350 8300 3350
Text Label 4950 2900 2    50   ~ 0
SCK
Text Label 4950 3000 2    50   ~ 0
TFT_CS
Text Label 4950 3100 2    50   ~ 0
MISO
Text Label 4950 3200 2    50   ~ 0
MOSI
Text Label 4950 3400 2    50   ~ 0
RESET
Text Label 4950 3300 2    50   ~ 0
D_C
Text Label 2100 3200 0    50   ~ 0
SCK
Text Label 2100 3300 0    50   ~ 0
MOSI
Text Label 2100 3600 0    50   ~ 0
D_C
Text Label 2100 3700 0    50   ~ 0
RESET
Text Label 8250 2650 2    50   ~ 0
DAC0
Text Label 8250 2750 2    50   ~ 0
DAC1
$Comp
L power:+3.3V #PWR01
U 1 1 623CEA24
P 2100 3000
F 0 "#PWR01" H 2100 2850 50  0001 C CNN
F 1 "+3.3V" V 2115 3128 50  0000 L CNN
F 2 "" H 2100 3000 50  0001 C CNN
F 3 "" H 2100 3000 50  0001 C CNN
	1    2100 3000
	0    1    1    0   
$EndComp
Text Label 2100 3500 0    50   ~ 0
CARD_CS
$Comp
L power:+3.3V #PWR03
U 1 1 623D46EE
P 3250 3000
F 0 "#PWR03" H 3250 2850 50  0001 C CNN
F 1 "+3.3V" V 3265 3128 50  0000 L CNN
F 2 "" H 3250 3000 50  0001 C CNN
F 3 "" H 3250 3000 50  0001 C CNN
	1    3250 3000
	0    1    1    0   
$EndComp
Text Label 3250 3700 0    50   ~ 0
RESET
Text Label 3250 3600 0    50   ~ 0
D_C
Text Label 3250 3500 0    50   ~ 0
CARD_CS
Text Label 3250 3400 0    50   ~ 0
TFT_CS
Text Label 3250 3300 0    50   ~ 0
MOSI
Text Label 3250 3200 0    50   ~ 0
SCK
Text Label 3250 3100 0    50   ~ 0
MISO
Text Label 2100 3100 0    50   ~ 0
MISO
Text Label 2100 3400 0    50   ~ 0
TFT_CS
Text Label 6350 2900 0    50   ~ 0
DAC2
Text Label 6350 3000 0    50   ~ 0
DAC3
Text Label 6350 3100 0    50   ~ 0
DAC4
Text Label 6350 3200 0    50   ~ 0
DAC5
Text Label 6350 3300 0    50   ~ 0
DAC6
Text Label 6350 3400 0    50   ~ 0
DAC7
Text Label 6350 2700 0    50   ~ 0
DAC0
Text Label 6350 2800 0    50   ~ 0
DAC1
Text Label 4950 4500 2    50   ~ 0
BL2
Text Label 4950 4600 2    50   ~ 0
BR2
Text Notes 1650 4550 0    50   ~ 0
CONTROLLER 1
Text Notes 2800 4550 0    50   ~ 0
CONTROLLER 2
$Comp
L power:GND #PWR04
U 1 1 623D46F8
P 3250 4400
F 0 "#PWR04" H 3250 4150 50  0001 C CNN
F 1 "GND" V 3255 4272 50  0000 R CNN
F 2 "" H 3250 4400 50  0001 C CNN
F 3 "" H 3250 4400 50  0001 C CNN
	1    3250 4400
	0    -1   -1   0   
$EndComp
Text Label 3250 3800 0    50   ~ 0
A2
Text Label 3250 3900 0    50   ~ 0
B2
Text Label 3250 4000 0    50   ~ 0
J2_X
Text Label 3250 4100 0    50   ~ 0
J2_Y
Text Label 3250 4200 0    50   ~ 0
BL2
Text Label 3250 4300 0    50   ~ 0
BR2
$Comp
L power:GND #PWR02
U 1 1 623B2C73
P 2100 4400
F 0 "#PWR02" H 2100 4150 50  0001 C CNN
F 1 "GND" V 2105 4272 50  0000 R CNN
F 2 "" H 2100 4400 50  0001 C CNN
F 3 "" H 2100 4400 50  0001 C CNN
	1    2100 4400
	0    -1   -1   0   
$EndComp
Text Label 2100 3800 0    50   ~ 0
A1
Text Label 2100 3900 0    50   ~ 0
B1
Text Label 2100 4000 0    50   ~ 0
J1_X
Text Label 2100 4100 0    50   ~ 0
J1_Y
Text Label 2100 4200 0    50   ~ 0
BL1
Text Label 2100 4300 0    50   ~ 0
BR1
$Comp
L Connector_Generic:Conn_01x15 J1
U 1 1 623E8F7E
P 1900 3700
F 0 "J1" H 1818 4617 50  0000 C CNN
F 1 "Control_01" H 1818 4526 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical" H 1900 3700 50  0001 C CNN
F 3 "~" H 1900 3700 50  0001 C CNN
	1    1900 3700
	-1   0    0    -1  
$EndComp
Text Notes 1750 4300 1    50   ~ 0
BUTTONS
Text Notes 1750 3500 1    50   ~ 0
DISPLAY
Wire Notes Line
	1850 3100 1800 3100
Wire Notes Line
	1800 3100 1800 3700
Wire Notes Line
	1800 3700 1850 3700
Wire Notes Line
	1850 3800 1800 3800
Wire Notes Line
	1800 3800 1800 4300
Wire Notes Line
	1800 4300 1850 4300
NoConn ~ 4950 2700
$Sheet
S 8800 2600 750  750 
U 622C729F
F0 "Sheet622C729E" 50
F1 "DACSpeaker.sch" 50
F2 "P[0..7]" I L 8800 2700 50 
F3 "DACOUT" I L 8800 2800 50 
$EndSheet
Text Label 8400 2700 0    50   ~ 0
DAC[0..7]
Text Notes 2900 4300 1    50   ~ 0
BUTTONS
Text Notes 2900 3500 1    50   ~ 0
DISPLAY
$Comp
L Connector_Generic:Conn_01x15 J2
U 1 1 623E6737
P 3050 3700
F 0 "J2" H 2968 4617 50  0000 C CNN
F 1 "Control_02" H 2968 4526 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical" H 3050 3700 50  0001 C CNN
F 3 "~" H 3050 3700 50  0001 C CNN
	1    3050 3700
	-1   0    0    -1  
$EndComp
Wire Notes Line
	3000 3100 2950 3100
Wire Notes Line
	2950 3100 2950 3700
Wire Notes Line
	2950 3700 3000 3700
Wire Notes Line
	3000 3800 2950 3800
Wire Notes Line
	2950 3800 2950 4300
Wire Notes Line
	2950 4300 3000 4300
Text Label 7250 3900 2    50   ~ 0
TEXAS
Text Label 8800 2800 2    50   ~ 0
DACOUT
Text Label 7250 4000 2    50   ~ 0
DACOUT
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 625078C1
P 7450 3900
F 0 "J4" H 7530 3892 50  0000 L CNN
F 1 "Texas_DAC_Out" H 7530 3801 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7450 3900 50  0001 C CNN
F 3 "~" H 7450 3900 50  0001 C CNN
	1    7450 3900
	1    0    0    -1  
$EndComp
NoConn ~ 6350 4500
NoConn ~ 6350 4600
NoConn ~ 6350 4700
Wire Bus Line
	8400 2700 8400 3450
NoConn ~ 6350 4400
NoConn ~ 6350 4300
Text Label 6350 3900 0    50   ~ 0
TEXAS
Wire Wire Line
	4650 3700 4950 3700
Wire Wire Line
	4950 3600 4650 3600
$Comp
L power:GND #PWR09
U 1 1 624FE8DC
P 4650 3800
F 0 "#PWR09" H 4650 3550 50  0001 C CNN
F 1 "GND" H 4655 3627 50  0000 C CNN
F 2 "" H 4650 3800 50  0001 C CNN
F 3 "" H 4650 3800 50  0001 C CNN
	1    4650 3800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 624FE089
P 4450 3700
F 0 "J3" H 4368 3375 50  0000 C CNN
F 1 "UART" H 4368 3466 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4450 3700 50  0001 C CNN
F 3 "~" H 4450 3700 50  0001 C CNN
	1    4450 3700
	-1   0    0    1   
$EndComp
$Comp
L EK-TM4C123GXL:EK-TM4C123GXL U1
U 1 1 622C0FA6
P 5650 3600
F 0 "U1" H 5650 5067 50  0000 C CNN
F 1 "EK-TM4C123GXL" H 5650 4976 50  0000 C CNN
F 2 "EK-TM4C123GXL:MODULE_EK-TM4C123GXL" H 5650 3600 50  0001 L BNN
F 3 "" H 5650 3600 50  0001 L BNN
F 4 "N/A" H 5650 3600 50  0001 L BNN "MAXIMUM_PACKAGE_HIEGHT"
F 5 "Texas Instruments" H 5650 3600 50  0001 L BNN "MANUFACTURER"
F 6 "April 2013" H 5650 3600 50  0001 L BNN "PARTREV"
F 7 "Manufacturer Recommendations" H 5650 3600 50  0001 L BNN "STANDARD"
	1    5650 3600
	1    0    0    -1  
$EndComp
Text Label 4950 3800 2    50   ~ 0
BR1
Text Label 4950 3900 2    50   ~ 0
BL1
Text Label 4950 4400 2    50   ~ 0
J2_Y
Text Label 4950 4300 2    50   ~ 0
J2_X
Text Label 4950 4200 2    50   ~ 0
J1_Y
Text Label 4950 4100 2    50   ~ 0
J1_X
Text Label 6350 3800 0    50   ~ 0
B2
Text Label 6350 3700 0    50   ~ 0
A2
Text Label 6350 4000 0    50   ~ 0
B1
Text Label 6350 4100 0    50   ~ 0
A1
Text Label 6350 3600 0    50   ~ 0
CARD_CS
$EndSCHEMATC
