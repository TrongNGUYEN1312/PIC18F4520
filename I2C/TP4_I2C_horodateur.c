/*
*****************************************************************************
*    Fichiers requis:		xc.h
*				i2c.h 	(i2c.c à inclure dans le projet)
*				lcd.h 	(lcd.c à inclure dans le projet)
*****************************************************************************
*   Câblages et notes:
*       câblage entre ?C et LCD:
*           __RB0-RB3____  reliées à 4 bits de données
*           ___RB4___  relié à RS (register select)
*           non     relié à RW (read/write)
*           ___RB5___  relié à EN (enable)
*       câblage entre ?C et DS1307:
*           ___RC3___ (_SCK____) relié à SCL
*           ___RC4___ (__SDI___) relié à SDA
*       ex : SCL (RC3 )
***********************************************************************/
#define _XTAL_FREQ 8000000              // définition de la fréquence du Quartz

/*Definition du fichier header contenant les caractéristiques du 
 microcontroleur*/
#include <xc.h>

/* autres fichiers header associé au projet */
#include "LCD.h"
#include "i2c.h"

/*Configuration des bits systemes*/
#pragma config WDT = OFF, OSC = HS, MCLRE = ON, LVP = OFF

/********************** déclarations des fonctions *********************/
void Mise_a_Heure_Horodateur(unsigned char *tableau);
void Lecture_Heure_Horodateur(unsigned char *tableau);
void Aff_LCD_Date_Heure(unsigned char *tableau);
void lcd_aff_bcd_2digits(char val);
void lcd_aff_JourSemaine(char JSem);
void DelayMs(unsigned int N);

/******************************************************************************
*******************************************************************************
*		void main(void)
* fonction : Programme principal permettant d'afficher l'heure, le jour, 
 * //l'année...
* sur le LCD via une valeur par défaut récupérer par l'horodateur 
*
* Argument : aucun
* Retour : aucun
*******************************************************************************
*******************************************************************************/
void main(void)
{
	/*************** variables locales à main *****************/
	unsigned char	tableau_mise_a_heure[7];	
	unsigned char	tableau_lecture_heure[7];

	/*************** initialisations *****************/
	I2C_init();                         // initialisation de la liaison I2C
    InitLCD();                         // initialisation du LCD

	tableau_mise_a_heure[0]	= 0x30;		// valeur initiale des secondes
	tableau_mise_a_heure[1]	= 0x18;		// valeur initiale des minutes
	tableau_mise_a_heure[2]	= 0x10;		// valeur initiale des heures
	tableau_mise_a_heure[3]	= 0x05;		// valeur initiale des jours
	tableau_mise_a_heure[4]	= 0x21;		// valeur initiale de la date
	tableau_mise_a_heure[5]	= 0x02;		// valeur initiale des mois
	tableau_mise_a_heure[6]	= 0x20;		// valeur initiale des années

	Mise_a_Heure_Horodateur(tableau_mise_a_heure);
	
	/*** boucle infinie ***/
	while(1)
	{
       Lecture_Heure_Horodateur(tableau_lecture_heure);
       Aff_LCD_Date_Heure(tableau_lecture_heure);
//     Aff_LCD_Date_Heure(tableau_mise_a_heure);

        DelayMs(500);		
	}
}

/************************************************************************
;		S/P Mise_a_Heure_Horodateur
;	fonction:
;		réalise la mise à l'heure de l'horodateur par la liaison I2C
;	Appel:		le pointeur du tableau de mise à l'heure (7 valeurs)
;				est passé en paramètre
;	Retour:		rien
;************************************************************************/
void Mise_a_Heure_Horodateur(unsigned char *tableau)
{
    I2C_Start();						// condition de start
	I2C_SendAddress(0xD0,I2C_WRITE);	// 0xD0=0b11010000 adresse i2c de 
    //l'horodateur, mode écriture
	I2C_SendByte(0x00);					// pointe adresse 00
	I2C_PutString(tableau, 7);			// eciture des données de "tableau" de
    //l'adresse 0 à 6
	I2C_SendByte(0x10);					// ecriture de 0x10 à l'adresse 7
	I2C_Stop();							// condition de stop
}

/************************************************************************
;		S/P Lecture_Heure_Horodateur
;	fonction:
;		réalise la lecture de l'horodateur par la liaison I2C
;	Appel:		le pointeur du tableau de lecture de l'heure (7 valeurs)
;				est passé en paramètre
;	Retour:		le tableau contient les secondes, minutes, heures,
;				jour de la semaine, jour du mois, année
;************************************************************************/
void Lecture_Heure_Horodateur(unsigned char *tableau)
{
	unsigned int i=0;
    I2C_Start();						// condition de start
    I2C_SendAddress(0xD0,I2C_WRITE);	// 0xD0=0b11010000 adresse i2c de 
    //l'horodateur, mode écriture
    I2C_SendByte(0x00);                 // pointe adresse 00
    I2C_ReStart();                      // Condition de restart
    I2C_SendAddress(0xD0,I2C_READ);     // 0xD0=0b11010000 adresse i2c de 
    //l'horodateur, mode lecture
    //Version sans GetString
    /*for(i=0;i<6;i++)
    {
      tableau[i]=I2C_ReadByte(ACK);     //Lecture et écriture de l'acknowledge
    }
     tableau[6]=I2C_ReadByte(NACK);     //Lecture et écriture du not acknowledge
    */
    //Version avec GetString
    I2C_GetString(tableau,7);
    I2C_Stop();                        //Condition de stop
}

/************************************************************************
;		S/P Aff_LCD_Date_Heure
;	fonction:
;		réalise l'affichage de la date et l'heure sur le LCD
;	Appel:		le pointeur du tableau de lecture de l'heure (7 valeurs)
;				est passé en paramètre
;	Retour:		rien
;************************************************************************/
void Aff_LCD_Date_Heure(unsigned char *tableau)
{
	unsigned char	Sec, Min, Heure, JSem, Jour, Mois, Annee;

	Sec		=	tableau[0];		// affectation des secondes
	Min		=	tableau[1];		// affectation des minutes
	Heure	=	tableau[2];		// affectation des Heures
	JSem	=	tableau[3];		// affectation des JSem
	Jour	=	tableau[4];		// affectation des Jours
	Mois	=	tableau[5];		// affectation des Mois
	Annee	=	tableau[6];		// affectation des Annees

    ClearLCDScreen();           // Remise à zero de l'afficheur
    lcd_aff_JourSemaine(JSem); //Affichage de la date (Lundi,Mardi...)
    WriteDataToLCD( '-' );
    lcd_aff_bcd_2digits(Jour);  // Affichage des chiffres des jours 
    WriteDataToLCD( '/' );
    lcd_aff_bcd_2digits(Mois);  // Affichage des chiffres des mois
    WriteDataToLCD( '/' );
    lcd_aff_bcd_2digits(Annee); // Affichage des chiffres des annees
    GotoLine2ToLCD(0);          // Ecriture sur la deuxième ligne du LCD
    lcd_aff_bcd_2digits(Heure); // Affichage des chiffres des heures
    WriteDataToLCD( ':' );
    lcd_aff_bcd_2digits(Min);   // Affichage des chiffres des minutes
    WriteDataToLCD( ':' );
    lcd_aff_bcd_2digits(Sec);   // Affichage des chiffres des secondes
    
}

/************************************************************************
;		S/P lcd_aff_bcd_2digits
;	fonction:
;		affiche une valeur BCD 2 digits (0 à 99) sur le LCD
;	Appel:	la valeur BCD 2 digits c  est passée en paramètre
;	Retour: la fonction ne renvoie rien
*************************************************************************/
void lcd_aff_bcd_2digits(char val)
{
    unsigned char c;
    
    c = (val>>4)&0x0F;          // décallage de 4 bit et masque 
    WriteDataToLCD( c+0x30 );   // Ecriture de la valeur des dizaines  
    c = val&0x0F;               // Affectation des 4 premiers bit
    WriteDataToLCD( c+0x30 );   // Ecriture de la valeur des unités
}

/************************************************************************
;		S/P lcd_aff_JourSemaine
;	fonction:
;		affiche sur le LCD du jour de la semaine (Lundi, mardi,...)
;	Appel:	la valeur BCD 2 digits du jour de la semaine  est passée en 
*     paramètre
;	Retour: la fonction ne renvoie rien
*************************************************************************/
void lcd_aff_JourSemaine(char JSem)
{
    switch(JSem)    // Utilisation de la fonction switch pour afficher les
            //jours de la semaine
    {
        case 1:
          WriteStringToLCD("LUNDI"); 
          //Ecriture sur le LCD du premier jour de la semaine
          break;
        case 2:
          WriteStringToLCD("MARDI"); 
          //Ecriture sur le LCD du deuxième jour de la semaine
          break;
        case 3:
            WriteStringToLCD("MRCREDI"); 
            //Ecriture sur le LCD du troisième jour de la semaine
            break;
        case 4:
            WriteStringToLCD("JEUDI"); 
            //Ecriture sur le LCD du quatrième jour de la semaine
            break;
        case 5:
            WriteStringToLCD("VNDREDI"); 
            //Ecriture sur le LCD du cinquième jour de la semaine
            break;
        case 6:
            WriteStringToLCD("SAMEDI"); 
            //Ecriture sur le LCD du sixième jour de la semaine
            break;
        case 7:
            WriteStringToLCD("DIMANCH"); 
            //Ecriture sur le LCD du dernier jour de la semaine
            break;
        default:
            WriteStringToLCD("PRBLM"); 
            //Ecriture sur le LCD de problème si pas une bonne valeur de JSem
            break;
    }
    
}

void DelayMs(unsigned int N)
{
    unsigned int i;
    for(i=0;i<N;i++)   __delay_ms(1); 
}