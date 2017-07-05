#ifndef TRAITEMENT_H
#define TRAITEMENT_H
#if ARDUINO < 100
#include <WProgram.h> // Pour les versions d arduino inferieur a 1.0
#else
#include <Arduino.h>
#endif

class Traitement {

public:
	Traitement();

	static float moyenne(int tableau[], int nombresDonnees);
	static void resultat(int tableau[], int nombresDonnees);

};


#endif
