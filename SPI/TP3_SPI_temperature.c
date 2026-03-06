/*
***********************************************************************
*    Cablages et notes:
*	cablâge entre µC	     et		         LCD:
*		___
 * ___	reliées à	4 bits de données
*		___RB4___	relié à		RS (register select)
*		non     relié à		RW (read/write)
*		___RB5___	relié à		E  (enable)
*	cablâge entre µC		     et		MCP3204:
*		___SCK___ (___RC3__)	relié à		SCK 
*		___SDI__ (___RC4__)	relié à		SDI
*		___SCO___ (___RC5__)	relié à		SDO 
*		___RD___ (___RE0__)	relié à		/CS 
*	ex :  SCK   (RC3 )
*       la tension à convertir sur le MCP3204 est ch1-ch0
*
***********************************************************************/
#define _XTAL_FREQ 8000000              // définition de la fréquence du Quartz

//Definition du fichier header contenant les caractéristiques du microcontroleur
//(et les fonctions delay)*/
#include <xc.h>

/* autres fichiers header associé au projet */
#include "LCD.h"
#include "spi.h"
/*Configuration des bits systemes*/
#pragma config WDT = OFF, OSC = HS, MCLRE = ON, LVP = OFF

// déclaration des fonctions
unsigned int LectureTemperature_ADC_SPI(void);
void ConversionBCDTemperature(unsigned int temp, unsigned char *digits);
void AffichageTemperature(unsigned char *digits_temp);
void DelayMs(unsigned int N);

// définition de la broche de sélection de l'esclave pour la SPI
#define SPI_CS	 

// declaration des variables globales


/**********************************************************************************************
***********************************************************************************************
*		void main(void)
* fonction : Programme principal permettant de récolter, de convertir et d'afficher la température sur le LCD
*
* Argument : aucun
* Retour : aucun
************************************************************************************************
***********************************************************************************************/
void main(void)
{
	/*************** variables locales à main *****************/
	unsigned int Temperature=0;
	unsigned char digits_temp[4]={0,0,0,0};

	/*************** initialisations *****************/
	InitLCD();              // initialisation du LCD
	spi_init();				// initialisation de la SPI
   	WriteStringConstToLCD((const unsigned char *)"TEMPERATURE :");

	/*** boucle infinie **/
	while(1)
	{
		Temperature=LectureTemperature_ADC_SPI();
		ConversionBCDTemperature(Temperature,digits_temp);
      	GotoLine2ToLCD(3);
		AffichageTemperature(digits_temp);
		DelayMs(500);
	}
}

/************************************************************************
;		unsigned int LectureTemperature_ADC_SPI(void)
;	fonction:
;		lecture de la valeur température convertie par l'ADC MCP3202
;		communication par liaison SPI 
;		correction éventuelle par coefficient  
;	Appel:		rien
;	Retour:		la valeur de la température (sur 16 bits limité à 12 bits)
*************************************************************************/
unsigned int LectureTemperature_ADC_SPI(void)
{
	unsigned int result = 0;
	unsigned char resultH = 0, resultL = 0;
                                      
	LATEbits.LATE0=0; //sélection de l'esclave
    // D'après la datasheet il nous faut S/D=0, D2=x, D1=0 et D0=1
    //Car nous sommes en differential Ch1-Ch0
  	resultH=echangeSPI(0b00000100); //envoi du premier octet 
    //On envoie 0b 0 0 0 0 0 START S/D D2
    //l'octet reçu est inutile
  	resultH=echangeSPI(0b01000000); //envoi du deuxième octet
    //On envoie 0b D1 D0 x x x x x x
    //Les 4 derniers bits recupérés nous intéressent (B11 B10 B9 et B8)
 	resultH&=0b00001111;//Masquage des 4 bits de poid fort (inutiles))
    resultL=echangeSPI(0b00000000); //envoi du 3ème octet
    //On envoie un octet au hasard
    //Nous recevons B7 B6 B5 B4 B3 B2 B1 B0
	LATEbits.LATE0=1; //désélection de l'esclave
	result=((unsigned int)resultH<<8)+(unsigned int)resultL; //mise de la temperature bout à bout
    result=(result*1.22); //application du coefficient trouvé afin de passer en degré Celsius

	return result;
}

/************************************************************************
;		void ConversionBCDTemperature(unsigned int temp, unsigned char *digits)
;	fonction:
;		conversion de la valeur convertie 12 bits temp (0 à 4095) en 1/10e de degré
;		en BCD sur 4 digits Centaines,Dizaines,Unités,Dizième (0 à 9)
;	Argument:	- temp est la valeur de la température (avec 12 bits significatifs)
;				- *digits est l'adresse du tableau des 4 digits de la température
;	Retour:		aucun	(Mais le tableau des digits est modifié 
;						avec la valeur BCD dans chaque élément)
*************************************************************************/
void ConversionBCDTemperature(unsigned int temp, unsigned char *digits)
{
        unsigned int j;
	for(j=0;j<4;j++)
		digits[j]=0;// mise à 0 des bits dans le tableau

	digits[0]=(temp)/1000; //Récupération des centaines
	digits[1]=(temp%1000)/100; //Récupération des dizaines 
	digits[2]=(temp%100)/10; //Récupération des unités
	digits[3]=(temp%10); //Récupération des dizièmes
}

/************************************************************************
;		void AffichageTemperature(unsigned char *digits_temp)
;	fonction:
;		affiche la température en 1/10e de degré sur le LCD
;		le message affiché est de la forme: "Temp : XXX,X°C"
;	Argument:	adresse (pointeur) du tableau des 4 digits de la température
;	Retour:		rien	
*************************************************************************/
void AffichageTemperature(unsigned char *digits_temp)
{
    
	if(digits_temp[0]==0)
	{
		WriteDataToLCD(' ');
	}
	else
	{
		WriteBCDToLCD(digits_temp[0]);
	}
	if((digits_temp[0]==0)&&(digits_temp[1]==0))
	{
		WriteDataToLCD(' ');
	}
	else
	{
		WriteBCDToLCD(digits_temp[1]);
	}
	WriteBCDToLCD(digits_temp[2]);
	WriteDataToLCD(',');
	WriteBCDToLCD(digits_temp[3]);
	WriteStringConstToLCD((const unsigned char *)"°C");
    
}

void DelayMs(unsigned int N)
{
    unsigned int i;
    for(i=0;i<N;i++)   __delay_ms(1); 
}