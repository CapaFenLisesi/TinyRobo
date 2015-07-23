EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:shardy
LIBS:74hct125d
LIBS:arm-swd-header
LIBS:esp8266-esp-01
LIBS:esp8266-esp-03
LIBS:hm-11
LIBS:lpc11u14fbd48
LIBS:mcp1700t-3302e-tt
LIBS:mcp73831t-2aci-ot
LIBS:micro_usb_socket
LIBS:mke02z64vld2
LIBS:mke04z8vtg4
LIBS:nrf24l01p_smd
LIBS:nrf51822-04
LIBS:nRF24L01+
LIBS:pp_ws2812b
LIBS:stm32f37xxx_48pin
LIBS:stm32f102xx_48pin
LIBS:stm32f302xx_48pin
LIBS:switch_china_spdt
LIBS:uart_pp
LIBS:usb_plug
LIBS:open-project
LIBS:spdt
LIBS:tiny_robo_v2-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "6 jul 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +3.3V #PWR01
U 1 1 55774122
P 9250 3400
F 0 "#PWR01" H 9250 3360 30  0001 C CNN
F 1 "+3.3V" H 9250 3510 30  0000 C CNN
F 2 "" H 9250 3400 60  0001 C CNN
F 3 "" H 9250 3400 60  0001 C CNN
	1    9250 3400
	1    0    0    -1  
$EndComp
Text GLabel 8050 3450 0    60   Output ~ 0
Unreg
Text GLabel 7250 1300 0    60   Output ~ 0
Unreg
Text GLabel 1525 1850 0    60   Input ~ 0
5Vin
Text GLabel 4000 4525 2    60   Input ~ 0
5Vin
$Comp
L CONN_5 P1
U 1 1 55770CA4
P 3350 5100
F 0 "P1" V 3300 5100 50  0000 C CNN
F 1 "CONN_5" V 3400 5100 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05" H 3350 5100 60  0001 C CNN
F 3 "" H 3350 5100 60  0001 C CNN
	1    3350 5100
	-1   0    0    1   
$EndComp
Text GLabel 6275 4950 2    60   Output ~ 0
Fault2
Text GLabel 6275 4825 2    60   Output ~ 0
Fault1
$Comp
L GND #PWR02
U 1 1 5577082A
P 6475 1425
F 0 "#PWR02" H 6475 1425 30  0001 C CNN
F 1 "GND" H 6475 1355 30  0001 C CNN
F 2 "" H 6475 1425 60  0001 C CNN
F 3 "" H 6475 1425 60  0001 C CNN
	1    6475 1425
	1    0    0    -1  
$EndComp
Text GLabel 10675 5625 2    60   Input ~ 0
SCL
Text GLabel 10675 5450 2    60   Input ~ 0
SDA
Text GLabel 10575 5300 2    60   Output ~ 0
Fault2
Text GLabel 10575 6325 2    60   Output ~ 0
Fault1
Text GLabel 6375 5200 2    60   Input ~ 0
SCL
Text GLabel 6375 5075 2    60   Input ~ 0
SDA
$Comp
L GND #PWR03
U 1 1 55770098
P 8050 6850
F 0 "#PWR03" H 8050 6850 30  0001 C CNN
F 1 "GND" H 8050 6780 30  0001 C CNN
F 2 "" H 8050 6850 60  0001 C CNN
F 3 "" H 8050 6850 60  0001 C CNN
	1    8050 6850
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5576FFFF
P 9275 4300
F 0 "R5" V 9355 4300 50  0000 C CNN
F 1 "10k" V 9275 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 9275 4300 60  0001 C CNN
F 3 "" H 9275 4300 60  0001 C CNN
	1    9275 4300
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 5576FE5F
P 10025 5050
F 0 "R8" V 10105 5050 50  0000 C CNN
F 1 "10k" V 10025 5050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 10025 5050 60  0001 C CNN
F 3 "" H 10025 5050 60  0001 C CNN
	1    10025 5050
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5576FE58
P 10175 6075
F 0 "R9" V 10255 6075 50  0000 C CNN
F 1 "10k" V 10175 6075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 10175 6075 60  0001 C CNN
F 3 "" H 10175 6075 60  0001 C CNN
	1    10175 6075
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5576FE2F
P 9650 4300
F 0 "R7" V 9730 4300 50  0000 C CNN
F 1 "1k" V 9650 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 9650 4300 60  0001 C CNN
F 3 "" H 9650 4300 60  0001 C CNN
	1    9650 4300
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5576FE2B
P 9450 4300
F 0 "R6" V 9530 4300 50  0000 C CNN
F 1 "1k" V 9450 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 9450 4300 60  0001 C CNN
F 3 "" H 9450 4300 60  0001 C CNN
	1    9450 4300
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5576FE28
P 7850 6350
F 0 "R4" V 7930 6350 50  0000 C CNN
F 1 "0.22" V 7850 6350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 7850 6350 60  0001 C CNN
F 3 "" H 7850 6350 60  0001 C CNN
	1    7850 6350
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5576FE1B
P 7650 6350
F 0 "R3" V 7730 6350 50  0000 C CNN
F 1 "0.22" V 7650 6350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 7650 6350 60  0001 C CNN
F 3 "" H 7650 6350 60  0001 C CNN
	1    7650 6350
	1    0    0    -1  
$EndComp
$Comp
L DRV8830 U5
U 1 1 5576FBE9
P 8775 6125
F 0 "U5" H 8775 5825 50  0000 C CNN
F 1 "DRV8830" H 8775 6425 50  0000 C CNN
F 2 "MSOP:MSOP10" H 8775 6125 60  0001 C CNN
F 3 "" H 8775 6125 60  0001 C CNN
	1    8775 6125
	1    0    0    -1  
$EndComp
$Comp
L DRV8830 U4
U 1 1 5576FBE5
P 8775 5100
F 0 "U4" H 8775 4800 50  0000 C CNN
F 1 "DRV8830" H 8775 5400 50  0000 C CNN
F 2 "MSOP:MSOP10" H 8775 5100 60  0001 C CNN
F 3 "" H 8775 5100 60  0001 C CNN
	1    8775 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5575FDC6
P 4250 4950
F 0 "#PWR04" H 4250 4950 30  0001 C CNN
F 1 "GND" H 4250 4880 30  0001 C CNN
F 2 "" H 4250 4950 60  0001 C CNN
F 3 "" H 4250 4950 60  0001 C CNN
	1    4250 4950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 5575F5D2
P 4500 4800
F 0 "#PWR05" H 4500 4760 30  0001 C CNN
F 1 "+3.3V" H 4500 4910 30  0000 C CNN
F 2 "" H 4500 4800 60  0001 C CNN
F 3 "" H 4500 4800 60  0001 C CNN
	1    4500 4800
	1    0    0    -1  
$EndComp
$Comp
L ESP8266-ESP-03 U3
U 1 1 5575F418
P 5150 5400
F 0 "U3" H 5150 6125 60  0000 C CNN
F 1 "ESP8266-ESP-03" H 5150 5275 60  0000 C CNN
F 2 "esp-8266:ESP8266-ESP-03" H 5150 5400 60  0001 C CNN
F 3 "" H 5150 5400 60  0001 C CNN
	1    5150 5400
	1    0    0    -1  
$EndComp
NoConn ~ 7800 1900
$Comp
L +BATT #PWR06
U 1 1 54EFA680
P 5650 975
F 0 "#PWR06" H 5650 925 20  0001 C CNN
F 1 "+BATT" H 5650 1075 30  0000 C CNN
F 2 "" H 5650 975 60  0001 C CNN
F 3 "" H 5650 975 60  0001 C CNN
	1    5650 975 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 54EFA66A
P 8550 1325
F 0 "#PWR07" H 8550 1285 30  0001 C CNN
F 1 "+3.3V" H 8550 1435 30  0000 C CNN
F 2 "" H 8550 1325 60  0001 C CNN
F 3 "" H 8550 1325 60  0001 C CNN
	1    8550 1325
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 54EFA433
P 6375 950
F 0 "P2" V 6325 950 40  0000 C CNN
F 1 "BATT" V 6425 950 40  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 6375 950 60  0001 C CNN
F 3 "" H 6375 950 60  0001 C CNN
	1    6375 950 
	0    -1   -1   0   
$EndComp
$Comp
L MIC5265 U2
U 1 1 54EFA34D
P 7500 1800
F 0 "U2" H 7350 2000 60  0000 C CNN
F 1 "MIC5265" H 7500 1600 60  0000 C CNN
F 2 "Housings_SOT-23_SOT-143_TSOT-6:SOT-23-5" H 7500 1800 60  0000 C CNN
F 3 "~" H 7500 1800 60  0000 C CNN
	1    7500 1800
	1    0    0    -1  
$EndComp
$Comp
L MCP73831 U1
U 1 1 54EFA33B
P 4850 1950
F 0 "U1" H 4700 2150 60  0000 C CNN
F 1 "MCP73831" H 4850 1750 60  0000 C CNN
F 2 "Housings_SOT-23_SOT-143_TSOT-6:SOT-23-5" H 4850 1950 60  0000 C CNN
F 3 "~" H 4850 1950 60  0000 C CNN
	1    4850 1950
	1    0    0    -1  
$EndComp
$Comp
L LED Charge1
U 1 1 54EF9FA6
P 3650 2050
F 0 "Charge1" H 3650 2150 50  0000 C CNN
F 1 "LED" H 3650 1950 50  0000 C CNN
F 2 "LEDs:LED-0805" H 3650 2050 60  0001 C CNN
F 3 "" H 3650 2050 60  0001 C CNN
	1    3650 2050
	1    0    0    -1  
$EndComp
$Comp
L CP1 C4
U 1 1 54EF9F83
P 8550 1900
F 0 "C4" H 8600 2000 50  0000 L CNN
F 1 "10uF" H 8600 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8550 1900 60  0001 C CNN
F 3 "" H 8550 1900 60  0001 C CNN
	1    8550 1900
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 54EF9F5C
P 4200 2050
F 0 "R1" V 4280 2050 50  0000 C CNN
F 1 "470" V 4200 2050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4200 2050 60  0001 C CNN
F 3 "" H 4200 2050 60  0001 C CNN
	1    4200 2050
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 54EF9F56
P 5400 2300
F 0 "R2" V 5480 2300 50  0000 C CNN
F 1 "2k" V 5400 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5400 2300 60  0001 C CNN
F 3 "" H 5400 2300 60  0001 C CNN
	1    5400 2300
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 54EF9F4F
P 8225 1900
F 0 "C3" H 8275 2000 50  0000 L CNN
F 1 "0.1uF" H 8275 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8225 1900 60  0001 C CNN
F 3 "" H 8225 1900 60  0001 C CNN
	1    8225 1900
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 54EF9F4A
P 5650 2250
F 0 "C2" H 5700 2350 50  0000 L CNN
F 1 "4.7uF" H 5700 2150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5650 2250 60  0001 C CNN
F 3 "" H 5650 2250 60  0001 C CNN
	1    5650 2250
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 54EF9F43
P 3225 2050
F 0 "C1" H 3275 2150 50  0000 L CNN
F 1 "4.7uF" H 3275 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3225 2050 60  0001 C CNN
F 3 "" H 3225 2050 60  0001 C CNN
	1    3225 2050
	1    0    0    -1  
$EndComp
$Comp
L DIODE D1
U 1 1 54EF9F33
P 2850 1850
F 0 "D1" H 2850 1950 40  0000 C CNN
F 1 "GF1A" H 2850 1750 40  0000 C CNN
F 2 "Diodes_SMD:Diode-DO-214AB_Handsoldering" H 2850 1850 60  0001 C CNN
F 3 "" H 2850 1850 60  0001 C CNN
	1    2850 1850
	1    0    0    -1  
$EndComp
$Comp
L THERMISTOR 0ZCB0050FF2G1
U 1 1 54EF9F20
P 2275 1850
F 0 "0ZCB0050FF2G1" V 2375 1900 50  0000 C CNN
F 1 "Thermal fuse" V 2175 1850 50  0000 C CNN
F 2 "Resistors_SMD:R_1210_HandSoldering" H 2275 1850 60  0001 C CNN
F 3 "" H 2275 1850 60  0001 C CNN
	1    2275 1850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR08
U 1 1 54EF9ED1
P 4450 2700
F 0 "#PWR08" H 4450 2700 30  0001 C CNN
F 1 "GND" H 4450 2630 30  0001 C CNN
F 2 "" H 4450 2700 60  0001 C CNN
F 3 "" H 4450 2700 60  0001 C CNN
	1    4450 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 3775 10025 3775
Wire Wire Line
	9250 3775 9250 3400
Wire Wire Line
	9650 3775 9650 4050
Connection ~ 6900 1700
Wire Wire Line
	6900 1500 6900 1900
Wire Wire Line
	6900 1500 7350 1500
Wire Wire Line
	7350 1500 7350 1300
Wire Wire Line
	7350 1300 7250 1300
Wire Wire Line
	3225 2600 8550 2600
Wire Wire Line
	8375 5925 8150 5925
Wire Wire Line
	7575 4700 8150 4700
Wire Wire Line
	8150 4700 8150 5925
Wire Wire Line
	7775 4500 8375 4500
Wire Wire Line
	8375 4500 8375 4900
Wire Wire Line
	3750 4900 3775 4900
Connection ~ 3225 2600
Wire Wire Line
	3225 2600 3225 2250
Wire Wire Line
	6275 4950 6075 4950
Wire Wire Line
	6075 4950 6075 5000
Wire Wire Line
	6075 5000 5725 5000
Wire Wire Line
	7200 1800 7050 1800
Wire Wire Line
	7050 1800 7050 2600
Wire Wire Line
	6750 1900 7200 1900
Connection ~ 5650 1300
Wire Wire Line
	5650 1300 6275 1300
Wire Wire Line
	8650 5525 8650 6600
Connection ~ 7850 6600
Connection ~ 9650 3775
Wire Wire Line
	10025 3775 10025 4800
Connection ~ 9650 5000
Wire Wire Line
	9650 4550 9650 6025
Wire Wire Line
	10675 5450 9650 5450
Connection ~ 10025 5300
Wire Wire Line
	6375 5075 6125 5075
Wire Wire Line
	6125 5075 6125 5100
Wire Wire Line
	6125 5100 5725 5100
Connection ~ 9650 5450
Wire Wire Line
	8375 6025 7850 6025
Wire Wire Line
	7850 6025 7850 6100
Connection ~ 9275 3775
Wire Wire Line
	8225 5200 8375 5200
Wire Wire Line
	8375 6225 8225 6225
Wire Wire Line
	8375 6325 8375 6500
Wire Wire Line
	8375 6500 9275 6500
Wire Wire Line
	9275 6500 9275 6225
Wire Wire Line
	9275 6225 9175 6225
Connection ~ 9275 5100
Wire Wire Line
	9275 4550 9275 6125
Wire Wire Line
	9650 6025 9175 6025
Wire Wire Line
	9450 5925 9175 5925
Wire Wire Line
	9175 4900 9450 4900
Wire Wire Line
	4575 4800 4500 4800
Connection ~ 5650 1850
Wire Wire Line
	5150 1850 6050 1850
Wire Wire Line
	8550 1325 8550 1700
Connection ~ 8550 2100
Connection ~ 5650 2600
Wire Wire Line
	8550 2600 8550 2100
Connection ~ 8225 1700
Wire Wire Line
	8550 1700 7800 1700
Connection ~ 3225 1850
Wire Wire Line
	4550 1850 3050 1850
Connection ~ 3450 1850
Wire Wire Line
	3450 1850 3450 2050
Wire Wire Line
	5400 2050 5400 1950
Wire Wire Line
	5400 1950 5150 1950
Wire Wire Line
	5150 2600 5150 2050
Connection ~ 5150 2600
Connection ~ 5400 2600
Wire Wire Line
	5650 2600 5650 2450
Wire Wire Line
	4450 2600 4450 2700
Wire Wire Line
	5400 2600 5400 2550
Connection ~ 4450 2600
Wire Wire Line
	4450 2050 4550 2050
Wire Wire Line
	3950 2050 3850 2050
Wire Wire Line
	2525 1850 2650 1850
Wire Wire Line
	8550 2100 8225 2100
Connection ~ 7050 2600
Wire Wire Line
	4250 4900 4250 4950
Wire Wire Line
	9650 5000 9175 5000
Wire Wire Line
	9450 4550 9450 5925
Wire Wire Line
	10175 5825 10175 4800
Wire Wire Line
	10175 4800 10025 4800
Wire Wire Line
	9275 3775 9275 4050
Wire Wire Line
	9275 6125 9175 6125
Wire Wire Line
	9275 5100 9175 5100
Wire Wire Line
	9175 5200 9275 5200
Connection ~ 9275 5200
Wire Wire Line
	8375 5300 8375 5525
Wire Wire Line
	8375 5525 8650 5525
Connection ~ 8650 6500
Connection ~ 8225 5200
Wire Wire Line
	8050 6850 8050 6600
Wire Wire Line
	7650 6125 7650 5000
Wire Wire Line
	7650 5000 8375 5000
Connection ~ 9450 5625
Wire Wire Line
	6375 5200 5725 5200
Connection ~ 10175 6325
Wire Wire Line
	10675 5625 9450 5625
Connection ~ 9450 3775
Wire Wire Line
	8650 6600 7650 6600
Connection ~ 8050 6600
Wire Wire Line
	5650 2050 5650 975 
Wire Wire Line
	6900 1700 7200 1700
Wire Wire Line
	6475 1425 6475 1300
Wire Wire Line
	5725 4900 6050 4900
Wire Wire Line
	6050 4900 6050 4825
Wire Wire Line
	6050 4825 6275 4825
Wire Wire Line
	4575 4900 3900 4900
Wire Wire Line
	3900 4900 3900 5000
Wire Wire Line
	3900 5000 3750 5000
Connection ~ 4250 4900
Wire Wire Line
	3775 4900 3775 4525
Wire Wire Line
	3775 4525 4000 4525
Wire Wire Line
	1525 1850 2025 1850
Wire Wire Line
	8375 5100 8275 5100
Wire Wire Line
	8275 5100 8275 4600
Wire Wire Line
	8275 4600 7575 4600
Wire Wire Line
	8375 6125 8000 6125
Wire Wire Line
	8000 6125 8000 4800
Wire Wire Line
	8000 4800 7700 4800
Connection ~ 8550 2600
Wire Wire Line
	9450 3775 9450 4050
Wire Wire Line
	8050 3450 8225 3450
Wire Wire Line
	8225 3450 8225 6225
Wire Wire Line
	7650 6000 7650 6150
Connection ~ 7650 6000
Connection ~ 7650 6100
Wire Wire Line
	3750 5300 4425 5300
Wire Wire Line
	4425 5300 4425 5050
Wire Wire Line
	4425 5050 4575 5050
Wire Wire Line
	3750 5200 4500 5200
Wire Wire Line
	4500 5200 4500 5250
Wire Wire Line
	4500 5250 4575 5250
Wire Wire Line
	4575 5150 4500 5150
Wire Wire Line
	4500 5150 4500 5100
Wire Wire Line
	4500 5100 3750 5100
$Comp
L SPDT SW1
U 1 1 559C6DAD
P 6400 2000
F 0 "SW1" H 6200 2150 50  0000 C CNN
F 1 "SPDT" H 6250 1850 50  0000 C CNN
F 2 "switchSMD:spdt_pcm12stmr" H 6400 2000 60  0001 C CNN
F 3 "" H 6400 2000 60  0000 C CNN
	1    6400 2000
	1    0    0    -1  
$EndComp
Connection ~ 6900 1900
Wire Wire Line
	6050 1850 6050 2175
Connection ~ 9450 4900
Wire Wire Line
	6750 2100 6750 2175
Wire Wire Line
	6750 2175 6050 2175
Connection ~ 6050 2000
Wire Wire Line
	9175 6325 10575 6325
Wire Wire Line
	9175 5300 10575 5300
$Comp
L CONNECTOR P4
U 1 1 559D6607
P 6250 3450
F 0 "P4" H 6600 3550 70  0000 C CNN
F 1 "CONNECTOR" H 6600 3350 70  0000 C CNN
F 2 "Wire_Pads:SolderWirePad_single_0-8mmDrill" H 6250 3450 60  0001 C CNN
F 3 "" H 6250 3450 60  0000 C CNN
	1    6250 3450
	1    0    0    -1  
$EndComp
$Comp
L CONNECTOR P5
U 1 1 559D66C8
P 6250 3775
F 0 "P5" H 6600 3875 70  0000 C CNN
F 1 "CONNECTOR" H 6600 3675 70  0000 C CNN
F 2 "Wire_Pads:SolderWirePad_single_0-8mmDrill" H 6250 3775 60  0001 C CNN
F 3 "" H 6250 3775 60  0000 C CNN
	1    6250 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 3450 6250 3950
$Comp
L GND #PWR09
U 1 1 559D6D0E
P 6250 3950
F 0 "#PWR09" H 6250 3950 30  0001 C CNN
F 1 "GND" H 6250 3880 30  0001 C CNN
F 2 "" H 6250 3950 60  0000 C CNN
F 3 "" H 6250 3950 60  0000 C CNN
	1    6250 3950
	1    0    0    -1  
$EndComp
Connection ~ 6250 3775
$Comp
L CONN_01X02 P3
U 1 1 559D733F
P 7375 4300
F 0 "P3" H 7375 4450 50  0000 C CNN
F 1 "CONN_01X02" V 7475 4300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 7375 4300 60  0001 C CNN
F 3 "" H 7375 4300 60  0000 C CNN
	1    7375 4300
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X02 P6
U 1 1 559D7420
P 7375 4825
F 0 "P6" H 7375 4975 50  0000 C CNN
F 1 "CONN_01X02" V 7475 4825 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 7375 4825 60  0001 C CNN
F 3 "" H 7375 4825 60  0000 C CNN
	1    7375 4825
	-1   0    0    1   
$EndComp
Wire Wire Line
	7575 4250 7775 4250
Wire Wire Line
	7775 4250 7775 4500
Wire Wire Line
	7575 4600 7575 4350
Wire Wire Line
	7575 4775 7575 4700
Wire Wire Line
	7700 4800 7700 4875
Wire Wire Line
	7700 4875 7575 4875
$EndSCHEMATC