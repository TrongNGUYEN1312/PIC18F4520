/**********************************************************************
*   Bibliothèques des fonctions gérant l'ADC
***********************************************************************
*    Fichiers requis:   ADC.h
***********************************************************************/
*    Cablages et notes:		décrire les connexions des E/S au potentiomètre 
*	Entrée:
*       	 Le potentiomètre relié à RA0
*		
***********************************************************************/
#include "ADC.h"

/************************************************************************
;	explication de la fonction:
;		Initialisation de l'ADC (potentiomètre et ADCON 0,1 et 2 )
;
;
;	Appel:		Aucun
;	Retour:		Aucun
*************************************************************************/
void init_ADC(void)
{
    TRISA|=0b00000001; //RA0 en entrée

    ADCON0=0b00000001 ; //bit 0: ADON=1 => module du convertisseur A/D activé
			//bit1: GO/DONE=0 =>pas de lancement de conversion pour le moment (init)
			//bits 5-2=0000 => voie AN0(RA0) sélectionnée
			//bits 7-6 non utilisés

    ADCON1=0b00001110 ; //bit 3-0 = 1110 car RA0 seulement est en analogique
			//Les boutons poussoirs RE0 et RE1 sont donc en numérique, tout comme les sorties RA1, RA2 et RA3
			//bit 4: VCFG0=0 => nous utilisons VDD=5V
			//bit 5: VCFG1=0 => nous utilisons VSS=0V
			//bits 7-6 non utilisés
			

    ADCON2=0b00000001 ; //bits 2-0 = 001 => Clock source 8 Tosc car Fosc=8MHz or 5.71<8<11.43
			// => d’après le tableau 19-1 page 229 dd la dataSheet on choisit 8 Tosc
			//bits 5-3 = 000 => TACQ = 0 car il n'y a pas de chamement de voie
			//bit 6 non utilisé
			//bit 7: ADFM=0 résultat justifié à gauche
}

/************************************************************************
;	explication de la fonction:
;		Permet de traduire un signal analogique en un signal numérique
;
;
;	Appel:		Aucun
;	Retour:		Retourne le résultat du 8ème bit de poids fort du résultat de la conversion
*************************************************************************/
unsigned char acquisition_ADC(void)
{
    	ADCON0bits.GO_DONE=1;			//Début de la conversion
	while(ADCON0bits.GO_DONE==1);		// Attente la fin de la conversion
	return 	ADRESH;				// Retourne le résultat du 8ème bit de poids fort du résultat de la conversion
}


