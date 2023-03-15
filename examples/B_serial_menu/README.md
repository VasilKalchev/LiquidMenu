Example 2: serial_menu
==================
 This example uses the serial communication to execute commands.

 The available commands will be printed on the serial monitor.
 This example uses attached callback functions which are explained
 in the functions_menu.ino example.

 The circuit:
 https://raw.githubusercontent.com/VaSe7u/LiquidMenu/master/examples/B_serial_menu/serial_menu.png
 - LCD RS pin to Arduino pin 12
 - LCD E pin to Arduino pin 11
 - LCD D4 pin to Arduino pin 5
 - LCD D5 pin to Arduino pin 4
 - LCD D6 pin to Arduino pin 3
 - LCD D7 pin to Arduino pin 2
 - LCD R/W pin to ground
 - LCD VSS pin to ground
 - LCD VDD pin to 5V
 - 10k ohm potentiometer: ends to 5V and ground, wiper to LCD V0
 - 150 ohm resistor from 5V to LCD Anode
 - LCD Cathode to ground

 Created July 24, 2016
 by Vasil Kalchev


![schematic](https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/B_serial_menu/serial_menu.png?raw=true)
This example uses the serial communication to execute commands.
