#include <xc.h>
#include 	"i2c.h"

/************************************************************************
;************************************************************************
;	fonctions I2C pour XC8 MICROCHIP - mode maître uniquement		
;	
;	les brochages sont à définir dans I2C.h
;
;************************************************************************
;************************************************************************/

/************************************************************************
;		void I2C_init(void)
;	fonction:
;		initialise le module MSSP en mode I2C maître
;	Argument:	rien
;	Retour:		rien
;************************************************************************/
void I2C_init(void)
{
		SCL_DIR	= I2C_INPUT;		// équivalent à TRISCbits.TRISC3=1
									// soit RC3=SCL en entrée
									
		SDA_DIR	= I2C_INPUT;		//équivalent à TRISCbits.TRISC4=1
									// soit RC4=SDA en entrée
									//
		SSPCON1 &= 0xF0;			//force SSPCON1 à 0bxxxx0000
		SSPCON1 |= 0b1000;			// Puis à 0bxxxx1000
		// Soit I2C Master Mode clock=FOSC/(4*(SSPADD+1)) d'apres datasheet p172
		//-> MSSP en mode I2C maitre avec clock=FOSC/(4*(SSPADD+1))
		SSPCON1bits.SSPEN = 1;		// valide le module MSSP
		//d'apres datasheet p171

		SSPSTAT = 0b10000000;		// bit7=SMP=1 -> slew rate control disabled 
        //for standard
									//Speed mode (100KHz and 1MHz)
									//bit6=CKE=0-> Disable SMBus specific inputs
									//SMBus est un autre type de liaison proche 
        //de l'I2C
									//autres vits en lecture seule
									
									
		SSPADD = 19;				// regle la frequence de SCL (clock) lors 
        //de la transmission
									//voir ci-dessous le calcul
}

/************************************************************************
;		void I2C_WaitMSSP(void)
;	fonction:
;		attend la fin des différents envois (procédures) sur l'I2C
;		(utilisé pour I2C_Start, I2C_Stop, I2C_SendByte, etc...)
;	Argument:	rien
;	Retour:		rien
;************************************************************************/
void I2C_WaitMSSP(void)
{
	while(PIR1bits.SSPIF==0);		// attend que l'indicateur SSPIF=1
									// soit la fin de l'envoi de la procédure
	PIR1bits.SSPIF = 0;				// RAZ de l'indicateur
}

/************************************************************************
;		void I2C_Start(void)
;	fonction:
;		génère une condition de start sur la liaison I2C en mode maître
;		(front descendant de SDA pendant que SCL est à l'état haut)
;	Argument:	rien
;	Retour:		rien
;************************************************************************/
void I2C_Start(void)
{
	SSPCON2bits.SEN	= 1;	// Mise du start à 1
	I2C_WaitMSSP();			// 
}

/************************************************************************
;		void I2C_Stop(void)
;	fonction:
;		génère une condition de stop sur la liaison I2C en mode maître
;		(front montant de SDA pendant que SCL est à l'état haut)
;	Argument:		rien
;	Retour:		rien
;************************************************************************/
void I2C_Stop(void)
{
	SSPCON2bits.PEN	= 1;	// MISE DU STOP à 1
	I2C_WaitMSSP();			// 
}

/************************************************************************
;		void I2C_ReStart(void)
;	fonction:
;		génère une condition de Restart sur la liaison I2C en mode maître
;	Argument:		rien
;	Retour:		rien
;************************************************************************/
void I2C_ReStart(void)
{
	SSPCON2bits.RSEN	= 1;	// Mise du restart à 1
	I2C_WaitMSSP();				// 
}

/************************************************************************
;		void I2C_Ack(void)
;	fonction:
;			envoi d'un Acknowledge du maitre vers l'esclave sur l'I2C
;	Argument:	rien
;	Retour:		rien
;************************************************************************/
void I2C_Ack(void)
{
	SSPCON2bits.ACKDT	= ACK;	// 
	SSPCON2bits.ACKEN	= 1;	// 
	I2C_WaitMSSP();				// 
}

/************************************************************************
;		void I2C_NAck(void)
;	fonction:
;			envoi d'un Not Acknowledge du maitre vers l'esclave sur l'I2C
;	Argument:	rien
;	Retour:		rien
;************************************************************************/
void I2C_NAck(void)
{
	SSPCON2bits.ACKDT	= NACK;	// 
	SSPCON2bits.ACKEN	= 1;	// 
	I2C_WaitMSSP();				// 
}

/************************************************************************
;		void I2C_WaitAck(void)
;	fonction:
;			attente d'un acknowledge de l'esclave vers le maitre sur l'I2C
;	Argument:	rien
;	Retour:		rien
;************************************************************************/
void I2C_WaitAck(void)
{
	while(SSPCON2bits.ACKSTAT==1);	// tant que le bit "acknowledge status" 
    //est à 1
}

/************************************************************************
;		void I2C_SendByte(unsigned char byte)
;	fonction:
;		envoi d'un octet du maitre vers l'esclave (mode write) de l'I2C
;		et attente et test de l'ACKNOWLEDGE recu par l'esclave
;		(boucle infinie si mauvais ACK de l'esclave)
;	Argument:	l'octet à envoyer est passé en paramètre
;	Retour:		rien
;************************************************************************/
void I2C_SendByte(unsigned char byte)
{
	SSPBUF = byte;			// envoi de l'octet sur I2C
	I2C_WaitMSSP();			// attend la fin de la procédure sur I2C

	I2C_WaitAck();			// attend l'acknowledge de l'esclave
}

/************************************************************************
;		unsigned char I2C_ReadByte(unsigned char ack_or_not)
;	fonction:
;		lecture d'un octet de l'esclave vers le maitre (mode read) de l'I2C
;		et envoi d'un ACK ou d'un NACK
;	Argument:	ack_or_not:	= ACK  envoi d'un ACK vers l'esclave après lecture
;							= NACK envoi d'un NACK vers l'esclave après lecture
;	Retour:	l'octet lu est renvoyé
;************************************************************************/
unsigned char I2C_ReadByte(unsigned char ack_or_not)
{
	unsigned char octet_lu;

	SSPCON2bits.RCEN	= 1;	// 
	I2C_WaitMSSP();				// 
	octet_lu = SSPBUF;			// 
	if(ack_or_not == ACK)		// 
		I2C_Ack();				// 
	if(ack_or_not == NACK)		// 
		I2C_NAck();				// 

	return octet_lu;			// 
}

/************************************************************************
;		void I2C_SendAddress(unsigned char address, unsigned char rw)
;	fonction:
;		envoi du 1er octet du maitre vers l'esclave (mode write) de l'I2C
;		comprenant l'adresse de l'esclave (7 bits) 
;		et le Read/write (1 bit) = I2C_READ = 1 ou = I2C_WRITE = 0
;		avec attente et test de l'ACKNOWLEDGE recu par l'esclave
;		(boucle infinie si mauvais ACK de l'esclave)
;	Argument:	- adress: octet dont les 7 bits de poids forts correspondent
;						à l'adresse I2C de l'esclave
;						le 8ème bit (poids faible) doit être mis à 0
;				- rw:	= I2C_READ (= 1) mode lecture
;						= I2C_WRITE (= 0) mode écriture
;	Retour:		rien
;************************************************************************/
void I2C_SendAddress(unsigned char address, unsigned char rw)
{
	if(rw == I2C_WRITE)						// si demande d'écriture
		I2C_SendByte(address & 0b11111110);	// envoi adresse  + R/W à 0
	if(rw == I2C_READ)						//si demande de lecture 
		I2C_SendByte(address | 0b00000001);	// envoi adresse + R/W à 1
}

/************************************************************************
;		void I2C_PutString(const unsigned char *str, unsigned char length)
;	fonction:
;		envoi d'une série d'octets du maitre vers l'esclave (mode write)
;		de l'I2C
;	Argument:	la série d'octets ou son pointeur 
;				et le nombre d'octet à envoyer sont passés en paramètre
;	Retour:		rien
;************************************************************************/
void I2C_PutString(const unsigned char *str, unsigned char length)
{
	unsigned char i;

	for(i=0;i<length;i++)		// 
	{
		I2C_SendByte(str[i]);	// 
	}
}

/************************************************************************
;		void I2C_GetString(unsigned char *str, unsigned char number)
;	fonction:
;		lit une série d'octets de l'esclave vers le maitre (mode read)
;		de l'I2C ==> ATTENTION! l'I2C doit être en mode lecture 
;	Argument:	le pointeur sur la série d'octets 
;				et le nombre d'octet à lire sont passés en paramètre
;	Retour:		le tableau de la série d'octets contient les octets lus
;************************************************************************/
void I2C_GetString(unsigned char *str, unsigned char number)
{
	unsigned char i;

	for(i=0;i<number-1;i++)				//
	{
		str[i] = I2C_ReadByte(ACK);		// 
	}
	str[i] = I2C_ReadByte(NACK);		// 
}
