/**********************************************************************
*   Bibliothèques des fonctions gérant le clavier matriciel
***********************************************************************
*    Fichiers requis:   clavier.h
***********************************************************************
*    Cablages et notes:			décrire les connexions des E/S au clavier
*       
*		Entrées:
*			Les lignes du clavier reliées à RC4 jusqu'à RC7
*		Sorties:
*			Les colonnes du clavier reliées à RC0 jusqu'à RC3
*		
***********************************************************************/

#include "clavier.h"

/**********************************************************************************************************
* Explication de la fonction : prépositionnement et initialisation des E/S liées au clavier
*
* Argument : 	Aucun
* Retour : 	Aucun
************************************************************************************************************/
void init_clavier(void)
{
    //initialisation du port C relié au clavier matriciel
    LATC&=0b11110000;   //mise de RC0 de RC3 dans un état connu(colonnes à 0)
    TRISC=0b11110000;   //RC0 à RC3 en sortie (colonnes clavier)
                        //RC4 à RC7 en entrée (lignes clavier)
}

/***************************************************************************************************
* Explication de la fonction : Cette fonction permet de sélectionner la colonne suivante du clavier
* Argument : 	num_col = numéro courant de la colonne à tester (0 à 3)
* Retour : 		numéro suivant de la colonne à tester (0 à 3)
*****************************************************************************************************/
unsigned char ChangeColonneClavier(unsigned char num_col)
{							
	LATC  &=  0b11110000;
	num_col++;      // colonne suivante
	if(num_col>=4) num_col=0;   //numéro de colonne allant de 0 à 3
 	switch(num_col)			// selon le numéro de la colonne à tester
	{
		case 0 :
			LATCbits.LATC0 = 1;	// sélection de la colonne 0
			break;
		case 1 :				
			LATCbits.LATC1 = 1;	// sélection de la colonne 1
			break;
		case 2 :				
           		LATCbits.LATC2 = 1; // sélection de la colonne 2
			break;
		case 3 :				
            		LATCbits.LATC3 = 1; // sélection de la colonne 3
			break;
	}
	return num_col; // retourne le numero de la colonne
}

/**********************************************************************************************
* Explication de la fonction : Cette fonction permet de détecter une touche appuyée du clavier
* Argument : 	num_col = numéro de la colonne à tester (0 à 3)
* Retour : 		valeur de la touche appuyée: 	0x00 à 0X09 (touches 0 à 9)
*				+ 0xA à 0xD = 10 à 13 (touches A à D)
*				+ 0xE = 14 (touche *) et  0xF = 15 (touche #)
*				0xFF (aucune touche appuyée)
***********************************************************************************************/
unsigned char GetClavier(unsigned char num_col)
{
	unsigned char EtatClavier;
	static unsigned char EtatClavierPrecedentColonne0 =0,
						EtatClavierPrecedentColonne1 =0,
						EtatClavierPrecedentColonne2 =0,
						EtatClavierPrecedentColonne3 =0;
								// variables static (gardées en mémoire après appel)
								// retenant l'état du clavier précédent pour chaque colonne

	EtatClavier = PORTC>>4;			// lecture de ligne sur le PORTC
 	switch(num_col)					// selon la colonne testée
	{
		case 0 :
			if( EtatClavier == EtatClavierPrecedentColonne0 ) return 0xFF;
			EtatClavierPrecedentColonne0 = EtatClavier;
		 	switch(EtatClavier)		// en fonction de l'état du clavier
			{
				case 0b00000001 :       //si 1 ligne appuyée
					return 1;           //retourne la touche 1 appuyée
				case 0b00000010 :      //si 2 ligne appuyée
					return 4;           //retourne la touche 4 appuyée
				case 0b00000100 :      //si 3 ligne appuyée
					return 7;           //retourne la touche 7 appuyée
				case 0b00001000 :      //si 4 ligne appuyée
					return 14;           //retourne la touche * appuyée
				default :
					return 0xFF;        //retourne aucune touche appuyée
			}
		case 1 :

			if( EtatClavier == EtatClavierPrecedentColonne1 ) return 0xFF;
			EtatClavierPrecedentColonne0 = EtatClavier;
		 	switch(EtatClavier)		// en fonction de l'état du clavier
			{
				case 0b00000001 :       //si 1 ligne appuyée
					return 2;           //retourne la touche 2 appuyée
				case 0b00000010 :       //si 2 ligne appuyée
					return 5;           //retourne la touche 5 appuyée
				case 0b00000100 :       //si 3 ligne appuyée
					return 8;           //retourne la touche 8 appuyée
				case 0b00001000 :       //si 4 ligne appuyée
					return 0;           //retourne la touche 0 appuyée
				default :
					return 0xFF;        //retourne aucune touche appuyée
			}
		case 2 :
			if( EtatClavier == EtatClavierPrecedentColonne2 ) return 0xFF;
			EtatClavierPrecedentColonne0 = EtatClavier;
		 	switch(EtatClavier)		// en fonction de l'état du clavier
			{
				case 0b00000001 :       //si 1 ligne appuyée
					return 3;           //retourne la touche 3 appuyée
				case 0b00000010 :       //si 2 ligne appuyée
					return 6;           //retourne la touche 6 appuyée
				case 0b00000100 :       //si 3 ligne appuyée
					return 9;           //retourne la touche 9 appuyée
				case 0b00001000 :       //si 4 ligne appuyée
					return 15;           //retourne la touche # appuyée
				default :
					return 0xFF;        //retourne aucune touche appuyée
			}

		case 3 :

			if( EtatClavier == EtatClavierPrecedentColonne3 ) return 0xFF;
			EtatClavierPrecedentColonne0 = EtatClavier;
		 	switch(EtatClavier)		// en fonction de l'état du clavier
			{
				case 0b00000001 :       //si 1 ligne appuyée
					return 0xA;         //retourne la touche A appuyée
				case 0b00000010 :       //si 2 ligne appuyée
					return 0xB;         //retourne la touche B appuyée
				case 0b00000100 :       //si 3 ligne appuyée
					return 0xC;         //retourne la touche C appuyée
				case 0b00001000 :       //si 4 ligne appuyée
					return 0xD;         //retourne la touche D appuyée
				default :
					return 0xFF;        //retourne aucune touche appuyée
			}
		default :
					return 0xFF;        //retourne aucune touche appuyée
	}
}
