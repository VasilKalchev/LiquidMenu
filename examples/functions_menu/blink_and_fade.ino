void fade() {
	static byte step = 0;
	static bool goingUp = true;
	if (step == 0) {
		if (goingUp) {
			redLed_level++;
		} else {
			redLed_level--;
		}
		if (redLed_level >= 10) {
			goingUp = false;
		}
		if (redLed_level <= 0 && goingUp == false) {
			step++;
			goingUp = true;
		}
		greenLed_level = 0;
		blueLed_level = 0;
	}
	if (step == 1) {
		if (goingUp) {
			greenLed_level++;
		} else {
			greenLed_level--;
		}
		if (greenLed_level >= 10) {
			goingUp = false;
		}
		if (greenLed_level <= 0 && goingUp == false) {
			step++;
			goingUp = true;
		}
		redLed_level = 0;
		blueLed_level = 0;
	}
	if (step >= 2) {
		if (goingUp) {
			blueLed_level++;
		} else {
			blueLed_level--;
		}
		if (blueLed_level >= 10) {
			goingUp = false;
		}
		if (blueLed_level <= 0 && goingUp == false) {
			step = 0;
			goingUp = true;
		}
		redLed_level = 0;
		greenLed_level = 0;
	}
	PWM_log(redLed, redLed_level);
	PWM_log(greenLed, greenLed_level);
	PWM_log(blueLed, blueLed_level);
}

void blink() {
	static byte blinkStep = 0;
	if (blinkStep == 0) {
		redLed_level = 10;
		greenLed_level = 0;
		blueLed_level = 0;
		blinkStep++;
	} else if (blinkStep == 1) {
		redLed_level = 0;
		greenLed_level = 10;
		blueLed_level = 0;
		blinkStep++;
	} else if (blinkStep >= 2) {
		redLed_level = 0;
		greenLed_level = 0;
		blueLed_level = 10;
		blinkStep = 0;
	}
	PWM_log(redLed, redLed_level);
	PWM_log(greenLed, greenLed_level);
	PWM_log(blueLed, blueLed_level);
}
