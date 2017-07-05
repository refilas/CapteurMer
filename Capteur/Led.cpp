#include "Led.h"

Led::Led()
{
}

// Constructeur paramétrer
Led::Led(int redPin, int greenPin, int bluePin)
{
	pinRed = redPin;
	pinGreen = greenPin;
	pinBlue = bluePin;
}

// Fonction de controle des couleurs
void Led::controleLed(int red, int green, int blue)
{
	analogWrite(pinRed, red);
	analogWrite(pinGreen, green);
	analogWrite(pinBlue, blue);
}

// Fonction de controle du bon fonctionnement de la led RGB
void Led::testLed()
{
	delay(1000);
	Serial.println("Debut du test LED");
	analogWrite(pinRed, 255);
	delay(1500);
	analogWrite(pinRed, 0);
	analogWrite(pinGreen, 255);
	delay(1500);
	analogWrite(pinGreen, 0);
	analogWrite(pinBlue, 255);
	delay(1500);
	analogWrite(pinBlue, 0);
	Serial.println("Fin du test LED.");
}

