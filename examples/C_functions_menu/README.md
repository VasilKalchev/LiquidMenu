Example 3: functions_menu
==================
his example demonstrates how to attach functions to the
 "lines" in the menu.
 *
 Attaching functions to the different "lines" in the menu makes it
 possible to execute an action through the menu (e.g. dimming an LED,
 adjusting preferences...). To attach a function to a line you need
 to create with signature "void functionName(void)". After creating it,
 the function is ready to be attached to a line. To do that call bool
 LiquidLine::attach_function(uint8_t number, void (*function)(void)).
 LiquidLine is the line object you wish to attach the function to,
 number is the "id" of the function for that line, void (*function)(void)
 is just the name of the function without the "()". The "id" of the function
 makes possible attaching multiple functions to a single line. An example use
 of multiple functions on a single line is for incrementing or decrementing
 some value (e.g. dimming an LED). It is convenient to attach all the different
 incrementing functions (e.g. increase_brightness(), increase_volume()) to the
 different lines (e.g. LED brightness: 5, Sound volume: 10) using the same id
 (e.g. 1). Same goes for the decrementing functions (e.g. LED brightness: 5 (line)
 with attached function decrease_brightness() (function) with id 2,
 Sound volume: 10 (line) with attached function decrease_volume() (function) with id 2).
 Similar functions should be attached to their corresponding lines with the same "id",
 because then they can be called from the same event (e.g. pressing button "UP"
 calls the incrementing function of whatever the focused line is).
 This example has a line that shows the current PWM of the LED and lines
 that allow a fade or blink loop to be turned ON/OFF and configured.

 The circuit:
 https://raw.githubusercontent.com/VaSe7u/LiquidMenu/master/examples/C_functions_menu/functions_menu.png
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
 - ----
 - Button (left) to Arduino pin A0 and ground
 - Button (right) to Arduino pin 7 and ground
 - Button (up) to Arduino pin 8 and ground
 - Button (down) to Arduino pin 9 and ground
 - Button (enter) to Arduino pin 10 and ground
 - A PWM controlled device (LED...) to Arduino pin 6

 Created July 24, 2016
 by Vasil Kalchev
![schematic](https://github.com/VasilKalchev/LiquidMenu/blob/master/examples/C_functions_menu/functions_menu.png?raw=true)
This example demonstrates how to attach functions to the "lines" in the menu.
