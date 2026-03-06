/**********************************************************************
*   Bibliothèques des fonctions gérant l'affichage
*			sur les 3 afficheurs 7 segments DIS1, DIS2 et DIS3
***********************************************************************
*    Fichiers requis:   7_segments.h
***********************************************************************
*    Cablages et notes:		décrire les connexions des E/S aux afficheurs 7 segments
*	Sorties:
*        	Les segments de a jusqu'à g reliés à RD0 jusqu'à RD7
*		 Transistors de selection reliés à RA1 (Unité), RA2 (Dizaine) et RA3 (Centaine)
*		
***********************************************************************/

#include "7_segments.h"

#define NB_DIGITS 3 //Nombre de chiffre max composant l'afficheur

/**********************************************************************************************************
* Explication de la fonction : prépositionnement et initialisation des E/S liées aux afficheurs 7 segments
*
* Argument : 	Aucun
* Retour : 		Aucun
************************************************************************************************************/
void init_7Segments(void)
{
// initialisation port D relié aux 7 Segements + point
	LATD=0;	//mise du port D dans un état connu (LEDs éteintes)
	TRISD=0b00000000; //Tout le port D en sortie (segments des afficheurs)

//initialisation port A relié aux transistors de sélection des afficheurs 7 Segments
	LATA &=0b11110001; //mise de RA0 à RA3 dans un état connu (LEDs eteintes)
//on ne change pas les 5 autres bits => masque
	TRISA&=0b11110001;  //RA1 à RA3 en sortie 
//on ne change pas les 5 autres bits => masque
}

/**********************************************************************************************************
* Explication de la fonction : Cette fonction permet de convertir un chiffre BCD (0 à 9) en code 7 segments
*
* Argument : 	valeur BCD à convertir (0 à 9)
*				pour toutes autres valeurs aucun segment ne sera allumé
* Retour : 		code 7 segments correspondant
************************************************************************************************************/
unsigned char ConversionBCD7Segments( unsigned char val_bcd)
{											
 	switch(val_bcd)
	{
		case 0 :
			return 0b00111111; //retourne le code 7 segment de 0
		case 1 :
			return 0b00000110; //retourne le code 7 segment de 1
		case 2 :
			return 0b01011011; //retourne le code 7 segment de 2
		case 3 :
			return 0b01001111; //retourne le code 7 segment de 3
		case 4 :
			return 0b01100110; //retourne le code 7 segment de 4
		case 5 :
			return 0b01101101; //retourne le code 7 segment de 5
		case 6 :
			return 0b01111101; //retourne le code 7 segment de 6
		case 7 :
			return 0b00000111; //retourne le code 7 segment de 7
		case 8 :
			return 0b01111111; //retourne le code 7 segment de 8
		case 9 :
			return 0b01101111; //retourne le code 7 segment de 9
		case 10 :
			return 0b01110111; //retourne le code 7 segment de A
		case 11 :
			return 0b01111100; //retourne le code 7 segment de B
		case 12 :
			return 0b00111001; //retourne le code 7 segment de C
		case 13 :
			return 0b01011110; //retourne le code 7 segment de D
		case 14 :
			return 0b01000000; //retourne le code 7 segment de '*' (un tiret)
		case 15 :
			return 0b01011100; //retourne le code 7 segment de '#' (un carré)
		default :
			return 0b00000000; //retourne le code 7 segment nul (tout éteints)
	}
}

/********************************************************************************************
* Explication de la fonction : Cette fonction permet de convertir les N digits au format BCD
*								en N digits au format code 7 segments
* Argument : *Digits = adresse du tableau des N digits au format  BCD,
*		 	*Digits7Segments = adresse du tableau des N digits au format code 7 segments,
* Retour : aucun (le tableau pointé par *Digits7Segments est modifié)
**********************************************************************************************/
void ConversionDigits7Segments( unsigned char * digits, unsigned char * digits7segments)
{

	unsigned char i;
	
	for (i=0; i<4;i++) //afin d'agir sur les 4 afficheurs
		digits7segments[i]=ConversionBCD7Segments(digits[i]); //renvoie un tableau de segments à allumer
//en fonction des 4 demandes

}

/********************************************************************************************
* Explication de la fonction : Cette fonction permet de convertir une valeur de type char
*								en N digits au format BCD (valeur de 0 à 9)
* Argument : valeur = la valeur à convertir
*			*Digits = adresse du tableau des N digits au format  BCD
* Retour : aucun (le tableau pointé par *Digits est modifié)
**********************************************************************************************/
void conversion_Char2BCD(unsigned char valeur,unsigned char * Digits)
{

	Digits[2]=(valeur)/100;			//Centaines
	Digits[1]=(valeur%100)/10;		//Dizaines
	Digits[0]=(valeur%10);		//Unités

//Milliers non nécessaires car NB_DIGITS a été fixé à 3

}

/**************************************************************************************************
* Explication de la fonction : permet d'afficher le code 7 segments sur l'afficheur sélectionné
* Argument : *digits7segments = adresse du tableau des 4 digits en code 7 Segments ,
*				num_aff = numéro de l'afficheur sélectionné (0 à 2)
* Retour : aucun
**************************************************************************************************/
void GestionAfficheur7Segments( unsigned char * digits7segments, unsigned char num_aff)
{
	LATA &= 0b11110000; //désélection de tous les afficheurs
	LATD = digits7segments[num_aff]; // envoi du code 7 segements sur le port D
 	switch(num_aff)		// selon le numéro d'afficheur
	{
		case 0 :
			LATAbits.LATA1 = 1;		// sélection de l'afficheur 0 (unités)
			break ;
		case 1 :
			LATAbits.LATA2 = 1;		// sélection de l'afficheur 1 (dizaines)
			break ;
		case 2 :
			LATAbits.LATA3 = 1;		// sélection de l'afficheur 2 (centaines)
			break ;
	}
}


/**************************************************************************************************
* Explication de la fonction : permet d'afficher une valeur courante sur les afficheurs 7 segments
* Argument : valeur à afficher (0 à 255)
* Retour : aucun
**************************************************************************************************/
void affichage_7Segments(unsigned char valeur)
{
	static unsigned char num_afficheur=0;
	unsigned char Digits[NB_DIGITS];
	unsigned char Digits7Segments[NB_DIGITS];

	conversion_Char2BCD(valeur,Digits);
	ConversionDigits7Segments(Digits, Digits7Segments );
	GestionAfficheur7Segments( Digits7Segments,  num_afficheur);
	num_afficheur++;
	if(num_afficheur>=NB_DIGITS)
		num_afficheur=0;
}
