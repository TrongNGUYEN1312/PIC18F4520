/**********************************************************************
*   Programme réalisant:
*       	Ce programme a pour but de faire défiler des animations LEDs en utilisant un microcontrôleur
*	p18f4520. Les animations pouront être changer via un clavier pad et deux boutons poussoirs.
*	La vitesse des annimations est réglée via un potentiomètre et géré grâce à une base de temps de 10ms.
*	Enfin le nombre courant de décalage de LEDs sera affiché sur des afficheurs 7 segments.
*	
***********************************************************************
*    Nom de fichier:     main.c
*    Date:               12/01/21
*    Version:            FINALE
*    Auteurs:            Nicolas LAVAL et Oyhan BRUNZIN
*    Etablissement:      INSA CENTRE VAL DE LOIRE
*    Année d'étude:      3ème année GSI
*    Groupe TP:          3TP2
***********************************************************************
*    Fichiers requis:   pic18f4520.h 	= definition des registres
*						animations.h	= definition des fonctions gérant les animations
*						timer.h			= definition des fonctions gérant le timer
*						ADC.h			= definition des fonctions gérant l'ADC
*						clavier.h		= definition des fonctions gérant le clavier matriciel
*						7_segments.h	= definition des fonctions gérant l'affichage
*								sur les 3 afficheurs 7 segments DIS1, DIS2 et DIS3
***********************************************************************
*    Cablages et notes:			décrire les connexions des E/S au système
*       
*		Entrées:
*			Les lignes du clavier reliées à RC4 jusqu'à RC7
*			Les deux boutons poussoirs  reliés à RE0 et RE1
*       		Le potentiomètre relié à RA0
*		Sorties:
*			Les colonnes du clavier reliées à RC0 jusqu'à RC3
*			Les LEDs reliées à RB0 jusqu'à RB5
*       		Les segments de a jusqu'à g reliés à RD0 jusqu'à RD7
*			Transistors de selection reliés à RA1 (Unité), RA2 (Dizaine) et RA3 (Centaine)
*		
***********************************************************************/

/*Configuration des bits systemes*/
#pragma config WDT = OFF, OSC = HS, MCLRE = ON, LVP = OFF
/*Definition du fichier header contenant les caractéristiques du microcontroleur*/
#include  <pic18f4520.h>
/* autres fichiers nécessaires */
#include "animations.h"
#include "timer.h"
#include "ADC.h"
#include "clavier.h"
#include "7_segments.h"

/******* PROGRAMME PRINCIPAL *******/
void main (void)
{
	//initialisation des variables
	unsigned char LED=0;
	unsigned int i;
	unsigned char num_animation=1;
	unsigned char num_animation_precedent=1;
	unsigned char seg=0;

	/* initialisation des E/S */
  	init_animations();
	init_timer();
   	init_clavier();
   	init_ADC();
	init_7Segments();

	while(1)
	{
		if(num_animation==num_animation_precedent)
		{
			seg++;
		}
		else
		{
			seg=0;
			num_animation_precedent=num_animation;
		}
		
       		for(i=0;i<=acquisition_ADC();i++) //definition du temps entre charque changement d'état donné par l'entrée analogique
       		{ 
           	   WaitForTimer0_10ms(); //base de temps de 10 ms
           	   affichage_7Segments(seg); //affichage des segments
          	   num_animation=changement_num_animation(num_animation, &LED); //definition de l'animation en cours
      		}
       		LED=Calcul_LED_animation(num_animation,LED); //attribution du nouvel état des LED
       		LATB=LED; //Affichage du nouvel état
		
	}
}
