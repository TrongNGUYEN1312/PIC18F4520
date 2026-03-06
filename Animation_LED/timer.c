/**********************************************************************
*   Bibliothèques des fonctions gérant le timer
***********************************************************************
*    Fichiers requis:   timer.h
***********************************************************************/
#include "timer.h"

/************************************************************************
;	explication de la fonction:
;		Initilisation du timer et des variables
;
;	Appel:		Aucun
;	Retour:		Aucun
*************************************************************************/
void init_timer(void)
{
    T0CON=0b10000100; // bits 2-0: prédiviseur
			//bit 3: PSA =0 prédiviseur activé
			//bit 4: choix du front de l'horloge => Front ascendant (peu importe ici)
			//bit 5: T0CS =0 => TIMER0 horloge interne Fosc
			//bit 6: T08BIT=0 => TIMER0 mode 16 bits pour atteindre 625 et avoir 0% d'erreur
			//bit7: TTMR0ON=1 => TIMER0 actif
    TMR0=0; //Mise à 0 du timer (High et Low pour atteindre 625 ce qui nous permettra 
	    //d'utiliser le prediviseur P=32 et d'atteindre 0% d'erreur)
}


/************************************************************************
;	explication de la fonction:
;		La fonction permet une temporisation pour une période de 10ms
;
;
;	Appel:		Aucun
;	Retour:		Aucun
*************************************************************************/
void WaitForTimer0_10ms(void)
{
    while(TMR0<625); // On attend 10 ms que TMR0 arrive à 625.
    TMR0=0;		// remise à 0 du timer
}



