#ifndef LED_H
#define LED_H
#if ARDUINO < 100
#include <WProgram.h> // Pour les versions inferieur a 1.0 arduino
#else
#include <Arduino.h>
#endif



class Led {
public:
	Led();
	Led(int, int, int);
	void controleLed(int red, int green, int blue);
	void testLed();

private:
	int pinRed;
	int pinGreen;
	int pinBlue;
};


#endif


