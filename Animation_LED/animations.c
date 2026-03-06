/**********************************************************************
*   Bibliothèques des fonctions gérant les animations
***********************************************************************
*    Fichiers requis:   animations.h
***********************************************************************
*    Cablages et notes:		décrire les connexions des E/S aux LEDs et BPs
*       
*	Sorties:
*		 Les LEDs reliées à RB0 jusqu'à RB5
*	Entrée:
*		Les deux boutons poussoirs  reliés à RE0 et RE1
*
***********************************************************************/
#include "animations.h"
#include "clavier.h"

#define NUM_ANIM_MAX 6 // on définit le nombre d'animation 
#define SW1 PORTEbits.RE0 // on définit SW1 équivalent au bit du port à tester pour le BP SW1
#define SW2 PORTEbits.RE1 // on définit SW2 équivalent au bit du port à tester pour le BP SW2
#define APPUYE 0 // on définit l'état appuyé des boutons (logique négative)
#define RELACHE 1 // on définit l'état relaché des boutons (logique positive)

/**********************************************************************************************************
* Explication de la fonction : prépositionnement et initialisation des E/S liées aux LEDs et BPs
*
* Argument : 	Aucun
* Retour : 		Aucun
************************************************************************************************************/
void init_animations(void)
{
    //initialistaion du port B relié aux LEDs
    LATB&=0b11000000; // On initialise les sorties (LEDs) que l'on va utiliser
    // dans notre cas on utilise de RB0 à RB5
    TRISB=TRISB&0b11000000;	// On initialise les Leds en utilisant un masque car les deux dernier bits ne sont pas utilisés

    //initialistaion du port C relié aux boutons poussoirs
    TRISE=TRISE|0b00000011;	// Les boutons poussoirs sont des entrées 
//nous utilisons un masque car nous n’utilisons pas RE2 à RE7
//Les boutons poussoirs seront définis en numérique graçe à l'initialisation de ADCON1 dans init_ADC qui se produira après init_animations

}

/************************************************************************
;	explication de la fonction:
;   Permet la gestion des bouton afin de réaliser les changements d'animation
;
;
;	Appel:		le numero d'animation actuel
;	Retour:		le numero d'animation demander par l'utilisateur via les boutons
*************************************************************************/
unsigned char gestion_bouton(unsigned char num_animation)
{//Gestion des boutons en front montant
    static unsigned char EtatSW1Precedent, EtatSW2Precedent; //Déclaration des états des BPs précédent (gardé en mémoire grâce au type static)
    if(SW1==APPUYE && EtatSW1Precedent==RELACHE) // Animation suivante
    {
        num_animation++; // incrémentation du numéro de l'animation pour chaque appuie 
        if(num_animation>NUM_ANIM_MAX)
            num_animation=1; // retour à la première animation après le dernière
    }
    EtatSW1Precedent=SW1; // mise en mémoire de l'état de notre BP dans notre variable
    if(SW2==APPUYE && EtatSW2Precedent==RELACHE) // Animation précédente
    {
        num_animation--; // décrémentation du numéro de l'animation pour chaque appuie
        if(num_animation==0) 
            num_animation=NUM_ANIM_MAX; // permet de passer à la dernière animation
    }
    EtatSW2Precedent=SW2; // mise en mémoire de l'état de notre BP dans notre variable
	return num_animation; 
}

/************************************************************************
;	explication de la fonction:
;		Permet l'utilisation du clavier afin de réaliser les changements d'animation
;
;
;	Appel:		le numero d'animation actuel
;	Retour:		le numero d'animation demander par l'utilisateur via le clavier
*************************************************************************/
unsigned char gestion_clavier(unsigned char num_animation)
{
	static char num_col =0; // Récupération de la colonne dans un static pour le garder en mémoire
	unsigned char key; // Affectation de la touche enfoncé sur le clavier
	num_col = ChangeColonneClavier(num_col); //permet de balayer toutes les colonnes du clavier
	key =  GetClavier(num_col); // Récupération de la touche appuyé sur le clavier
    if(key>=1 && key<=NUM_ANIM_MAX) // Limitation de la taille du clavier en terme de colonne
        num_animation=key; // Affectation de l'animation en fonction de la touché appuyé sur la clavier 
	
	return num_animation;
}

/************************************************************************
;	explication de la fonction:
;		décrire ici ce que fait la fonction (à modifier)
;
;
;	Appel:		le numero d'animation actuel
;               l'etat actuel des LED
; 
;	Retour:		le numero de l'animation après action de l'utilisateur
*************************************************************************/
unsigned char changement_num_animation(unsigned char num_animation,unsigned char *LED)
{
	unsigned char num_actu=num_animation; // variable qui stock l'état initial des LEDs
	num_animation = gestion_bouton(num_animation); // Animation choisit en fonction des boutons poussoirs
	num_animation = gestion_clavier(num_animation); // Animation choisit grace au clavier
    	if(num_actu!=num_animation) // Remise à zéro des LEDs en cas de changement d'animation (plus esthétique)
        	*LED=0;
	return num_animation;
}

/************************************************************************
;	explication de la fonction:
;		décrire ici ce que fait la fonction (à modifier)
;
;
;	Appel:		numero de l'animation en cours 
;               l'etat actuel des LEDs
;
;	Retour:		l'etat des LED à l'étape suivante
*************************************************************************/
unsigned char Calcul_LED_animation(unsigned char num_animation,unsigned char etat_LED)
{//definition de l'etape suivante des LED en fonction de l'état actuel et de l'animation en cours
    switch (num_animation) // Choix de l'animation de la fonction switch
    {
        case 1:
            etat_LED=clignotement(etat_LED); // Animation de clignotement
            break;
        case 2:
            etat_LED=chenillardD(etat_LED); // Animation de chenillard droit
            break;
        case 3:
            etat_LED=chenillardG(etat_LED); // Animation de chenillard gauche
            break;
        case 4:
            etat_LED=vagueD(etat_LED); // Animation de clignotement
            break;
        case 5:
            etat_LED=vagueG(etat_LED); // Animation de clignotement
            break;
        case 6:
            etat_LED=alternance(etat_LED); // Animation de clignotement
            break;
    }
	return etat_LED;
}

/************************************************************************
;	explication de la fonction:
;		définition de l'étape suivante de l'etat des LEDs pour le chenillard gauche
;
;
;	Appel:		état des LEDs actuel
;	Retour:		état des LEDs à l'étape suivante
*************************************************************************/
unsigned char chenillardG(unsigned char etat_LED)
{
    if (etat_LED==0b00000000)		//Lorsque toutes les LEDs sont éteintes
	{
		etat_LED = 0b00000001;		//Allumer la première LED
	}
	else if(etat_LED!=0b00100000)
	{
		etat_LED = etat_LED*2;	//Allumer la LED suivante
	}
    else
    {
        etat_LED = 0b00000000;  // éteindre toutes les LEDs
    }
	return etat_LED;
}

/************************************************************************
;	explication de la fonction:
;		définition de l'étape suivante de l'etat des LEDs pour le chenillard droite
;
;
;	Appel:		état des LEDs actuel
;	Retour:		état des LEDs à l'étape suivante
*************************************************************************/
unsigned char chenillardD(unsigned char etat_LED)
{
   if (etat_LED==0b00000000)		//Lorsque toutes les LEDs sont éteintes
	{
		etat_LED = 0b00100000;		//Allumer la dernière LED
	}
	else if(etat_LED!=0b00000001)
	{
		etat_LED = etat_LED/2;		//Allumer la LED précédante
	}
    else
    {
        etat_LED=0b00000000;    // éteindre toutes les LEDs
    }
	return etat_LED;
}

/************************************************************************
;	explication de la fonction:
;		définition de l'étape suivante de l'etat des LEDs pour la vague gauche
;
;
;	Appel:		état des LEDs actuel
;	Retour:		état des LEDs à l'étape suivante
*************************************************************************/
unsigned char vagueG(unsigned char etat_LED)
{
    if (etat_LED==0b00000000)		//Lorsque toutes les LEDs sont éteintes
	{
		etat_LED = 0b00000001;		//Allumer la première LED
	}
	else if(etat_LED!=0b00111111)
	{
		etat_LED = ((2*etat_LED)+0b00000001);		//Allumer la LED suivante en laissant allumer les autres
	}
    else
    {
        etat_LED = 0b00000000;  // éteindre toutes les LEDs
    }
	return etat_LED;
}

/************************************************************************
;	explication de la fonction:
;		définition de l'étape suivante de l'etat des LEDs pour la vague droite
;
;
;	Appel:		état des LEDs actuel
;	Retour:		état des LEDs à l'étape suivante
*************************************************************************/
unsigned char vagueD(unsigned char etat_LED)
{
   if (etat_LED==0b00000000)		//Lorsque toutes les LEDs sont éteintes
	{
		etat_LED = 0b00100000;		//Allumer la denière LED
	}
	else if(etat_LED!=0b00111111)
	{
		etat_LED = etat_LED/2 + 0b00100000;		//Allumer la LED précédante en laissant allumer les autres
	}
    else
    {
        etat_LED=0b00000000;    // éteindre toutes les LEDs
    }
	return etat_LED;
}


/************************************************************************
;	explication de la fonction:
;		définition de l'étape suivante de l'etat des LEDs pour l'alternance
;
;
;	Appel:		état des LEDs actuel
;	Retour:		état des LEDs à l'étape suivante
*************************************************************************/
unsigned char alternance(unsigned char etat_LED)
{
    if(etat_LED==0b00010101)
        etat_LED=0b00101010; //alterner l'allumage des LEDs
    else
        etat_LED=0b00010101; //alterner l'allumage des LEDs
	return etat_LED;
}

/************************************************************************
;	explication de la fonction:
;		définition de l'étape suivante de l'etat des LEDs pour le clignotement
;
;
;	Appel:		état des LEDs actuel
;	Retour:		état des LEDs à l'étape suivante
*************************************************************************/
unsigned char clignotement(unsigned char etat_LED)
{

    if (etat_LED==0b00000000)		//Si toutes les LEDs sont éteintes
	{
		etat_LED = 0b00111111;		//Allumer toutes les LEDs
	}
	else
	{
		etat_LED = 0b00000000;		//Eteindre toutes les LEDs
	}

	return etat_LED;
}
