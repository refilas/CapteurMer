#include "Traitement.h"

Traitement::Traitement() {
}


float Traitement::moyenne(int tableau[], int nombresDonnees)
{
  int i = 0;
  int total = 0;
  float moyenne = 0;

  for (i = 0; i < nombresDonnees; i++)
  {
    total = total + tableau[i];
  }
  moyenne = (total / nombresDonnees);
  return moyenne;
}


// Traitement des données pour les tests
void Traitement::resultat(int tableau[], int nombresDonnees)
{
  int resultat = moyenne(tableau, nombresDonnees);

  if (resultat <= 450)
  {
    Serial.println("Eau trouble");
  }
  else if (resultat > 450 && resultat <= 750)
  {
    Serial.println("Eau moyenne");
  }
  else
  {
    Serial.println("Eau clair");
  }

}
