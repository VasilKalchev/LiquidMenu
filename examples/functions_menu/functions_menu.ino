#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include "Button.h"

// An array holding linearized 8bit PWM values.
const uint8_t log_8bit[11] PROGMEM = {0, 1, 2, 3, 6, 12, 22, 40, 74, 138, 255};

// PWM function using the above array.
void PWM_log(uint8_t pin, uint8_t level) {
	analogWrite(pin, pgm_read_byte_near(log_8bit + level));
}

// Pin mapping for the display
const byte LCD_RS = 12;
const byte LCD_E = 11;
const byte LCD_D4 = 5;
const byte LCD_D5 = 4;
const byte LCD_D6 = 3;
const byte LCD_D7 = 2;
//LCD R/W pin to ground
//10K potentiometer to VO
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// This variable can be passed to the LiquidMenu object to set the starting screen.
const byte startingScreen = 2;

// Button objects instantiation
const bool pullup = true;
Button left(4, pullup);
Button right(5, pullup);
Button up(6, pullup);
Button down(12, pullup);
Button enter(8, pullup);

/*
 * An enumerator can be used for the callback functions. It sets the number
 * for similar functions. Later when a function is attached it can be passed
 * with this enum rather than a magic number.
 */
// There are two types of functions for this example, one that increases some
// value and one that decreases it.
enum FunctionTypes {
	increase = 1,
	decrease = 2,
};

// These are the pin definitions and variables for their PWM state.
const byte redLed = 11;
byte redLed_level = 0;

const byte greenLed = 10;
byte greenLed_level = 0;

const byte blueLed = 9;
byte blueLed_level = 0;

// Variables used for setting "preferences".
bool isFading = false;
char* isFading_text;
//isFading_text = (char*)"OFF";
unsigned int fadePeriod = 100;
bool isBlinking = false;
char* isBlinking_text;
//isBlinking_text = (char*)"OFF";
unsigned int blinkPeriod = 1000;


LiquidLine welcome_line1(1, 0, "LiquidMenu ", VERSION);
LiquidLine welcome_line2(1, 1, "Functions ex.");
LiquidScreen welcome_screen(welcome_line1, welcome_line2);

LiquidLine rgbTitle_line(4, 0, "RGB LED");
LiquidLine red_line(0, 1, "R ", redLed_level);
LiquidLine green_line(5, 1, "G ", greenLed_level);
LiquidLine blue_line(10, 1, "B ", blueLed_level);
LiquidScreen rgb_screen(rgbTitle_line, red_line, green_line, blue_line);

LiquidLine fade_line(0, 0, "Fade - ", isFading_text);
LiquidLine fadePeriod_line(0, 1, "Period: ", fadePeriod, "ms");
LiquidScreen fade_screen(fade_line, fadePeriod_line);

LiquidLine blink_line(0, 0, "Blink - ", isBlinking_text);
LiquidLine blinkPeriod_line(0, 1, "Period: ", blinkPeriod, "ms");
LiquidScreen blink_screen(blink_line, blinkPeriod_line);

LiquidMenu menu(lcd, startingScreen);

// Callback functions
void increase_red() {
	if (redLed_level < 10) {
		redLed_level++;
	} else {
		redLed_level = 0;
	}
	PWM_log(redLed, redLed_level);
}

void decrease_red() {
	if (redLed_level > 0) {
		redLed_level--;
	} else {
		redLed_level = 10;
	}
	PWM_log(redLed, redLed_level);
}

void increase_green() {
	if (greenLed_level < 10) {
		greenLed_level++;
	} else {
		greenLed_level = 0;
	}
	PWM_log(greenLed, greenLed_level);
}

void decrease_green() {
	if (greenLed_level > 0) {
		greenLed_level--;
	} else {
		greenLed_level = 10;
	}
	PWM_log(greenLed, greenLed_level);
}

void increase_blue() {
	if (blueLed_level < 10) {
		blueLed_level++;
	} else {
		blueLed_level = 0;
	}
	PWM_log(blueLed, blueLed_level);
}

void decrease_blue() {
	if (blueLed_level > 0) {
		blueLed_level--;
	} else {
		blueLed_level = 10;
	}
	PWM_log(blueLed, blueLed_level);
}

void fade_switch() {
	zeroOutRgb();
	if (isFading == true) {
		isFading = false;
		isFading_text = (char*)"OFF";
	} else {
		isFading = true;
		isFading_text = (char*)"ON";
		isBlinking = false;
		isBlinking_text = (char*)"OFF";
	}
}

void increase_fadePeriod() {
	if (fadePeriod < 3000) {
		fadePeriod += 10;
	}
}

void decrease_fadePeriod() {
	if (fadePeriod > 10) {
		fadePeriod -= 10;
	}
}

void blink_switch() {
	zeroOutRgb();
	if (isBlinking == true) {
		isBlinking = false;
		isBlinking_text = (char*)"OFF";
	} else {
		isBlinking = true;
		isBlinking_text = (char*)"ON";
		isFading = false;
		isFading_text = (char*)"OFF";
	}
}

void increase_blinkPeriod() {
	if (blinkPeriod < 3000) {
		blinkPeriod += 50;
	}
}

void decrease_blinkPeriod() {
	if (blinkPeriod > 50) {
		blinkPeriod -= 50;
	}
}

void zeroOutRgb() {
	redLed_level = 0;
	greenLed_level = 0;
	blueLed_level = 0;
	PWM_log(redLed, redLed_level);
	PWM_log(greenLed, greenLed_level);
	PWM_log(blueLed, blueLed_level);
}

// Checks all the buttons.
void buttonsCheck() {
	if (right.check() == LOW) {
		menu.next_screen();
	}
	if (left.check() == LOW) {
		menu.previous_screen();
	}
	if (up.check() == LOW) {
		// Calls the function identified with
		// increase or 1 for the focused line.
		menu.call_function(increase);
	}
	if (down.check() == LOW) {
		menu.call_function(decrease);
	}
	if (enter.check() == LOW) {
		// Switches focus to the next line.
		menu.switch_focus();
	}
}

// Function prototypes for blinking and fading the RGB LED.
void blink();
void fade();

void setup() {
	Serial.begin(250000);

	pinMode(redLed, OUTPUT);
	pinMode(greenLed, OUTPUT);
	pinMode(blueLed, OUTPUT);

	lcd.begin(16, 2);

	// The increasing functions are attached with identification of 1.
	/*
	 * This function can later be called by pressing the 'UP' button
	 * when 'red_line' is focused. If some other line is focused it's
	 * corresponding function will be called.
	*/
	red_line.attach_function(increase, increase_red);
	// The decreasing functions are attached with identification of 2.
	red_line.attach_function(decrease, decrease_red);
	green_line.attach_function(increase, increase_green);
	green_line.attach_function(decrease, decrease_green);
	blue_line.attach_function(increase, increase_blue);
	blue_line.attach_function(decrease, decrease_blue);

    // Here the same function is attached with two different identifications.
    // It will be called on 'UP' or 'DOWN' button press.
	fade_line.attach_function(1, fade_switch);
	fade_line.attach_function(2, fade_switch);
	fadePeriod_line.attach_function(increase, increase_fadePeriod);
	fadePeriod_line.attach_function(decrease, decrease_fadePeriod);

	blink_line.attach_function(1, blink_switch);
	blink_line.attach_function(2, blink_switch);
	blinkPeriod_line.attach_function(increase, increase_blinkPeriod);
	blinkPeriod_line.attach_function(decrease, decrease_blinkPeriod);

	menu.add_screen(welcome_screen);
	menu.add_screen(rgb_screen);
	menu.add_screen(fade_screen);
	menu.add_screen(blink_screen);

	menu.update();
}

void loop() {
	buttonsCheck();

	static unsigned long lastMillis_blink = 0;
	if ((isFading == true) && ((millis() - lastMillis_blink) > fadePeriod)) {
		lastMillis_blink = millis();
		fade();
		menu.update();
	}

	static unsigned long lastMillis_fade = 0;
	if ((isBlinking == true) && ((millis() - lastMillis_fade) > blinkPeriod)) {
		lastMillis_fade = millis();
		blink();
		menu.update();
	}

}
