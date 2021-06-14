# WeatherStation
Computer System Engineering I Project. Högskolan, Halmstad.
Features of the Weather Station:

1.1.	Recording of temperature
Logging of temperature on a fixed time base. Each minute is recorded and stored over a time period of 7 days. When the memory buffer is filled, the recording starts over from the beginning.

1.2.	Presentation of logged data
Presentation of recorded data on the LCD by text. Each day is presented by minimum, average and maximum values for temperature.

1.3.	Detection of sun position
Find the orientation of the sun with the photo sensor. When requested by the user, enter a mode where the weather station finds the orientation of the sun (can be simulated by any bright light source). The orientation is found by scanning the range by the RC servo and recording the photo sensor output followed by finding the peak of the light and reporting the angle of the servo.

1.4.	Alarm at under or over temperature
Alarm. Create an alarm for lower and upper temperature. Limits are adjustable in runtime.

1.5.	Test- / Fast-Mode
Test or “fast” mode. Create a test or “fast” mode where time is changed so that one minute in simulated by a second.

1.6.	Diagram
Diagram. Create a diagram which illustrated graphically how the system is composed

1.7.	Documentation
Documentation of the project. A report which specifies the modules of the system, with a clear connection to this specification, so it is possible to track which specification is solved with which part of the system.

The electronic parts we used to implement the weather station:
-	Microprocessor: Atmel SAM3X8E
-	Keypad: AK-804 with 12 buttons
-	Photosensor: Photoresistor PGM5xxx-MP
-	Temperature sensor: MAX6575L
-	Display: NHD-240128WG-ATMI-VZ
-	Servo motor: HS-55 
-	Octal Bus Transceiver: 74HC245


All the documentation can be found in the following link:

[Weathersation_Documentation_Heindl_Rueda.docx](https://github.com/esRueda/WeatherStation/files/6646900/Weathersation_Documentation_Heindl_Rueda.docx)


Diagram of the hardware components and how they are connected:

![BlockDiagram](https://user-images.githubusercontent.com/15103657/121865197-fbe7d980-ccfd-11eb-85ab-928cc171fe03.png)
