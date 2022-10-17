/**
 * I, Noah Tomkins, 0000790079 certify that this material is my original work.
 * No other person's work has been used without due acknowledgement.
 */

#include <Arduino.h>
#include <Bounce2.h>

#define PIR     D5
#define BUTTON  D6
#define LED		LED_BUILTIN

enum SecurityState {
	ARMED, ENTRY, DISARMED, ALARM
};

SecurityState state = ARMED;
bool motion = false;

unsigned long timeEntered;
unsigned long timeSinceLastBlink;

unsigned long phaseTime = 10000; // 10 seconds
unsigned int blinksPerSecond = 4; // Blinks Per Second
unsigned int blinkPause = (1000 / blinksPerSecond) / 2;
bool ledState = false;

Bounce bounce = Bounce();

/*** HELPER METHODS ***/
String stateToString(SecurityState securityState) {
	switch (securityState) {
		case ARMED:
			return "Armed";
		case ENTRY:
			return "Entry";
		case DISARMED:
			return "Disarmed";
		case ALARM:
			return "Alarm";
		default:
			return "Unknown";
	}
}

bool buttonPressed() {
	return bounce.changed() && bounce.read() == HIGH;
}

/*** Alarm Modes ***/
void changeState(SecurityState newState) {
	Serial.println("Security State changed from " + stateToString(state) + " to " + stateToString(newState));
	state = newState;
}

void armed() {
	if (motion) {
		changeState(ENTRY);
		timeEntered = millis();
		timeSinceLastBlink = millis();
	}	
}

void entry() {
	if (millis() - timeEntered < phaseTime) {

		if (millis() - timeSinceLastBlink >= blinkPause) {
			ledState = !ledState;
			timeSinceLastBlink = millis();
		}

		if (buttonPressed()) {
			changeState(DISARMED);
			ledState = false;
		}
	} else {
		changeState(ALARM);
	}	
}

void disarmed() {
	ledState = false;

	if (buttonPressed()) {
		changeState(ARMED);
	}
}

void alarm() {
	ledState = true;
}

/*** MAIN PROGRAM EXECUTION ***/

void setup() {
	Serial.begin(115200);
	Serial.println("\n\nCOMP-10184 - Alarm System");
    Serial.println("Name: Noah Tomkins");
    Serial.println("Student ID: 000790079");

	pinMode(LED, OUTPUT);
	pinMode(PIR, INPUT);

	bounce.attach(BUTTON, INPUT_PULLUP);
	bounce.interval(5);
}

void loop() {
	bounce.update();
	motion = digitalRead(PIR);

	if (state == ARMED) armed();
	else if (state == ENTRY) entry();
	else if (state == DISARMED) disarmed();
	else if (state == ALARM) alarm();

	digitalWrite(LED, !ledState);
}
