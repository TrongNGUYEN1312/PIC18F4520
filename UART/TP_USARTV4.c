
***************************************************************************************/
/*Definition du fichier header contenant les caractéristiques du microcontroleur*/
#include <pic18f4520.h>
/*Configuration des bits systemes*/
#pragma config WDT = OFF, OSC = HS, MCLRE = ON, LVP = OFF
#define Xon 0b00010001
#define Xoff 0b00010011

/********************** déclarations prototypes des fonctions *********************/
void init_USART(void);
void SendChar_USART(unsigned char byte);
unsigned char CalcParityBitEven(unsigned char Byte);
void ParityEven8(unsigned char Byte);
void ParityOdd8(unsigned char Byte);
void SendString_USART(unsigned char * text);
void SendStringConst_USART(const unsigned char * text);
void TempoUneMilliSeconde(void);
void TempoNMilliSeconde(unsigned int N);


/**********************************************************************************************
***********************************************************************************************
*		void main(void)
* fonction : Programme principal permettant d'initialiser les registres nécessaire (Timer et USART) et d'envoyer un caractère vers l'USART
*
* Argument : aucun
* Retour : aucun
************************************************************************************************
***********************************************************************************************/
void main(void)
{
	/*************** variables locales à main *****************/


	/*************** initialisations *****************/

	T0CON = 0b11010010;		// initialisation TIMER0 pour temporisation 
							
	init_USART();			// initialisation de l'USART
    
    const unsigned char text1[]="Bonjour!\n";
    const unsigned char text2[]="Bienvenue a l'INSA!\n";
    //SendString_USART(text1);
    //SendString_USART(text2); //Envoie du caractère 
    
	/*** boucle infinie ***/
	while(1)
	{
        SendStringConst_USART((const unsigned char *)"Bonjour!\n");
        SendStringConst_USART((const unsigned char *)"Bienvenue a l'INSA!\n");
        TempoNMilliSeconde(5); //Temporisation de 5ms
	}
}

/************************************************************************
;		void init_USART(void)
;	fonction:
;		initialise l'USART
;	Argument:
;	Retour:
*************************************************************************/
void init_USART(void)
{
	TRISCbits.TRISC7=1;  //RC7=broche RX en entrée
	TRISCbits.TRISC6=1;  //RC6=broche TX en entrée

	SPBRG=12;	     //vitesse de transmission à 9600Bds (9600 bits/seconde)

	BAUDCONbits.BRG16=0; //BRG compte en mode 8 bits

/********mode 8 bits sans parité********/
	 TXSTA=0b00100000;    //transmission de 8-bits, transmission autorisée
			     //mode asynchrone, mode basse vitesse
                            //bit6=0 : choisir la transmission de 8 bits
                            //bit5=1 : activer la transmission
                            //bit4=0 : mode asynchrone
                            //bit3=0 : quand la transmission est complete
                            //bit2=0 : car on est en low speed (n<225)
/********mode 8 bits avec parité********/
	//TXSTA=0b01100000;    //transmission de 9-bits, transmission autorisée
			     //mode asynchrone, mode basse vitesse    

	RCSTA=0b10010000;    //liaison en serie autorisée, reception non autorisée
                        //bit7=1 : car la porte en serie est activee
                        //bit6=0 : car on receptionne 8 bit
                        //bit4=1 : pour activer le recepteur
}
/************************************************************************
;		void SendChar_USART(unsigned char byte)
;	fonction:
;		envoie un caractère sur la ligne de transmission de l'USART
;	Argument: le code ASCII du caractère est passé en paramètre
;	Retour:
*************************************************************************/
void SendChar_USART(unsigned char byte) 
{
    //ParityEven8(byte);		// renvoie la parité pour un format 8 bits + parité paire
	//ParityOdd8(byte);		// renvoie la parité pour un format 8 bits + parité impaire
	if(PIR1bits.RCIF)   //L'EUSART à reçu une information
	{
		if(RCREG==Xoff) //Si l'information est un Xoff
		{
			while(RCREG!=Xon); //Nous bloquons l'envoi jusqu'à la récéption d'un Xon
		}
	}
    
    
    
	while(!PIR1bits.TXIF); //tant que le registre de transmission n'est pas vide
 
	TXREG= byte; //envoie d'un caractère dans TXREG

}
/************************************************************************
;		void SendString_USART(unsigned char * text)
;	fonction:
;		envoie une chaine de caractère sur la ligne de transmission de l'USART
;	Argument: adresse (pointeur) de la chaine de caractère 
;	Retour:
;       exemple: 	unsigned char text[]="Bienvenue à l'INSA!\n";
*			SendString_USART(text);
*************************************************************************/
void SendString_USART(unsigned char * text) 
{
    while(*text!='\0') //Tant que la chaîne de caractère n'est pas fini
    {
        SendChar_USART(*text); //envoie d'un caractère dans TXREG
        text++; //déplacement au caractère suivant
    }
}
/************************************************************************
;		void SendStringConst_USART(const unsigned char * text)
;	fonction:
;		envoie une chaine de caractère constante passée directement en paramètre
;		sur la ligne de transmission de l'USART
;	Argument: la chaine de caractère est directement passée en paramètre
;	Retour:
;   exemple: 
;		SendStringConst_USART((const char *)"Bienvenue à l'INSA!\n");
*************************************************************************/
void SendStringConst_USART(const unsigned char * text)
{
    while(*text!='\0') //Tant que la chaîne de caractère n'est pas fini
    {
        SendChar_USART(*text); //envoie d'un caractère dans TXREG
        text++; //déplacement au caractère suivant
        
    }
}
/************************************************************************
;		unsigned char CalcParityBitEven(unsigned char Byte)
;	fonction:
;		calcule le bit de parité d'un caractère (8bits)
;	Argument: le code ASCII du caractère est passé en paramètre
;	Retour: un octet donc le bit 0 est le bit de parité est renvoyé
*************************************************************************/
unsigned char CalcParityBitEven(unsigned char Byte)
{
	unsigned char ParityByte, Byte2;	// octets pour le calcul de la parité

	Byte2 = Byte;						// sauvegarde de l'octet
	ParityByte = Byte;		
	ParityByte = ParityByte>>1;			// décalage à droite
	ParityByte = ParityByte ^ Byte2;	// ou exclusif, compare tous les bits avec leur voisin
	Byte2 = ParityByte;
	ParityByte = ParityByte>>2;			// décalage à droite * 2
	ParityByte = ParityByte^Byte2;  	// ou exclusif, compare chaque 2nd bit et sauve
	Byte2 = ParityByte>>4;				// décalage à droite * 4
	ParityByte = ParityByte ^ Byte2;	// ou exclusif, compare chaque 4ème bit et sauve

	return ParityByte;
}
/************************************************************************
;		void ParityEven8(unsigned char Byte)
;	fonction:
;	 	placement du bit de parité paire dans le 9ème bit 
;		ce qui correspond à un format 8 bits parité paire
;	Argument: le code ASCII du caractère est passé en paramètre
;	Retour: 
*************************************************************************/
void ParityEven8(unsigned char Byte)
{
	unsigned char ParityByte;				// octets pour le calcul de la parité

	ParityByte = CalcParityBitEven(Byte);	/* calcul du bit de parité = bit 0
												du résultat dans octet */
	TXSTAbits.TX9D = ParityByte; 			/* attention! troncature à 1 bit
												placement du bit de parité paire
												(bit 0) dans le 9ème bit à envoyer */
}
/************************************************************************
;		void ParityOdd8(unsigned char Byte)
;	fonction:
;	 	placement du bit de parité impaire dans le 9ème bit 
;		ce qui correspond à un format 8 bits parité impaire
;	Argument: le code ASCII du caractère est passé en paramètre
;	Retour: 
*************************************************************************/
void ParityOdd8(unsigned char Byte)
{
	unsigned char ParityByte;				// octets pour le calcul de la parité

	ParityByte = CalcParityBitEven(Byte);	/* calcul du bit de parité = bit 0
												du résultat dans octet */
	TXSTAbits.TX9D = ~ParityByte; 			/* attention! troncature à 1 bit
												placement du complément du bit de parité paire 
												(bit 0) dans le 9ème bit à envoyer */
}
/************************************************************************
;		void TempoUneMilliSeconde(void)
;	fonction:
;		réalise une temporisation de 1 ms
;	Argument: 
;	Retour:
*************************************************************************/
void TempoUneMilliSeconde(void) 
{
		TMR0L = 0;					// RAZ Timer 0
		while(TMR0L<250);			// attend 250 coups = 1 ms
}
/************************************************************************
;		void TempoNMilliSeconde(unsigned int N)
;	fonction:
;		réalise une temporisation de N ms
;	Argument: N
;	Retour:
*************************************************************************/
void TempoNMilliSeconde(unsigned int N) 
{
		unsigned int i;

		for(i=0;i<N;i++)	TempoUneMilliSeconde();
}
