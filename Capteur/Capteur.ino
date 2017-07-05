/* Développer par Julien BOUCHET Pour un projet de Samuel Petit.
Création d un capteur mesurant la turbidit� de l eau dans le but de de créer un réseau de capteurs transmetant les données a un serveur web
Code source libre de droit.
*/
// Version 0.4a
//Derniere mise a jour 17/12/2016

#include "Traitement.h"
#include "Led.h"

#include <SPI.h>
#include <Ethernet.h>

// Définition des broches
#define PHOTORESISTANCE 0
#define PHOTORESISTANCE_RGB 1
#define CAPTEUR_RECEPTION 2
#define LED_R 5
#define LED_V 3
#define LED_B 6
#define LED 8

// Définition de constantes pour le temps
#define MINUTE 60000
#define HEURE 3600000
#define JOUR 86400000

// Contante de température provisoire
#define TEMP 20

// Définition des données du capteur pour l envoi en bdd
const String SPOT = "SaintJeanlaPoterie";
const String NOM = "test_temp";

// Temporisastion pour la réation de la photorésistance (100 Kohm par seconde)
#define TEMPORISATION 5000 

// Instance de l objet
Led ledRgb(LED_R, LED_V, LED_B);

// L'adresse MAC du shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xA5, 0x7E };
// L'adresse IP que prendra le shield
IPAddress ip(192, 168, 0, 39);
// L'objet qui nous servira a la communication
EthernetClient client;
// Le serveur à interroger
char serveur[]= "Luna-serveur";
// moment de la dernière requête
long derniereRequete = 0;
// temps minimum entre deux requêtes
const long updateInterval = 3000;
// mémorise l'état de la connexion entre deux tours de loop
bool etaitConnecte = false;

// Variable pour le test

int essais = true; // Si true, variable permetant l indentation du nom du spot pour le remplisage de la base de données
int test;


void setup() {
  test = 0;
  	// Ouverture de la liason série
	Serial.begin(9600);

	char erreur = 0;
	// On démarre le shield Ethernet SANS adresse ip (donc donnée via DHCP)
	erreur = Ethernet.begin(mac);
	if (erreur == 0) {
		Serial.println("Parametrage avec ip fixe...");
		// si une erreur a eu lieu cela signifie que l'attribution DHCP
		// ne fonctionne pas. On initialise donc en forçant une IP
		Ethernet.begin(mac, ip);
	}
	Serial.println("Init...");
	// Donne une seconde au shield pour s'initialiser
	delay(1000);
	Serial.println("Pret !");

	// Programmation des registre en sortie
	pinMode(LED, OUTPUT);
	pinMode(LED_R, OUTPUT);
	pinMode(LED_V, OUTPUT);
	pinMode(LED_B, OUTPUT);


	// Augmentation de précision de l entré analogique en utilisant une tension de 2,56V au lieu de 5V
	analogReference(INTERNAL);

	// Controle fonctionnel de la led RVB
	ledRgb.testLed();
}

void loop() {
        if(essais)
        {          
          test++; // Debug
        }    

	// Tableau contenant les mesures de turbidité, on fais cinq mesures
	int mesure[5];
	int mesure_RGB[5];
	int mesureCapteur[5];        
        
        
	for (int i = 0; i < 5; i++)
	{
		// Led classique
		digitalWrite(LED, HIGH);

		// Led RGB      
		ledRgb.controleLed(255, 255, 255);

		// Temps de r�ation de la photorésistance
		delay(TEMPORISATION);

		// Classement des données dans les tableaux
		mesure[i] = analogRead(PHOTORESISTANCE);
		mesure_RGB[i] = analogRead(PHOTORESISTANCE_RGB);
	}

	// Affichage direct de la clareté de l eau uniquement pour test, le traitement de cette information sera gérer par le serveur
	Traitement::resultat(mesure_RGB, 5);

	// Affchage de la valeur de mesure pour le test, remplacer par l envoi en base de données dans le futur
	Serial.print("Moyenne led classique : ");
	Serial.println(Traitement::moyenne(mesure, 5)); // On appelle la fonction permetant le calcul de la moyenne du tableau	
	Serial.print("Moyenne led RGB : ");
	Serial.println(Traitement::moyenne(mesure_RGB, 5)); // On appelle la fonction permetant le calcul de la moyenne du tableau
	Serial.println("Moyenne Capteur : ");
	Serial.println(Traitement::moyenne(mesureCapteur, 5));
	Serial.print("Lancer de depuis : ");
	Serial.print(millis() / 1000);
	Serial.println(" secondes");

	// Led classique
	digitalWrite(LED, LOW);

	// Led RGB  
	ledRgb.controleLed(0, 0, 0);

  // La methode map nous convertie la valeur sur une echelle de 0 a 10
	int valeur = map(Traitement::moyenne(mesure_RGB, 5),0,1023,0,10);
 
  Serial.print("Valeur mapper : ");
  Serial.println(valeur);
  
	requete(SPOT, NOM, valeur, TEMP);
	delay(10000);
}

void requete(String lieu, String nom, int valeur, int temperature)
{
	// On connecte notre Arduino sur le script php et le port 80
	char erreur = client.connect(serveur, 80);

	if (erreur == 1) {
		// Pas d'erreur ? on continu !
		Serial.println("Connexion OK, envoi en cours...");

		// On construit l'en-tete de la requete version GET			
		client.print("GET /send.php/?lieu=");
		client.print(lieu);
		client.print("&nom=");
		client.print(nom);
    if(essais)
    {
    client.print(test);
    }                       
		client.print("&valeur=");
		client.print(valeur);
    client.print("&temp=");
    client.print(TEMP);
		client.println("HTTP / 1.1");		
    client.println("Host: Luna-serveur");
		Serial.println("envoi terminé");	
		client.println("Connection: close");
		client.println();		

		// On enregistre le moment d'envoi de la dernière requete
		derniereRequete = millis();
	}
	else {
		// La connexion a échoué :(
		// On ferme le client
		client.stop();

		// On affiche l erreur
		Serial.println("Echec de la connexion");
		switch (erreur) {
		case(-1):
			Serial.println("Time out");
			break;
		case(-2):
			Serial.println("Serveur invalide");
			break;
		case(-3):
			Serial.println("Tronque");
			break;
		case(-4):
			Serial.println("Reponse invalide");
			break;
		}
	}
}

