/*************************************************************************
	fichier SPI.C 
	des S/P pour communiquer par la liaison SPI
	ATTENTION! 	modifier le câblage du /CS de l'esclave dans spi_init
**************************************************************************/
/* fichiers à inclure */
#include <xc.h>
#include "spi.h"

/************************************************************************
;		void spi_init(void)
;	fonction:
;		initialise la liaison SPI en mode maître
;	Argument:	aucun paramètre
;	Retour: la fonction ne renvoie rien
*************************************************************************/
void spi_init(void)
{
// définition des E/S des broches SPI prédéfinies SCK,SDO,SDI
      TRISCbits.TRISC3=0;              	/* SCK en sortie */
      TRISCbits.TRISC4=1;      		/* SDI en entrée */
      TRISCbits.TRISC5=0;		/* SDO en sortie */

// A MODIFIER SUIVANT CABLAGE
// définir ici la(les) sortie(s) /CS de(s) l'esclave(s)
      ADCON1|=0b00001111;          	/* Port A et E en numérique */
      TRISEbits.TRISE0=0;      		/* RE0 = /CS du MCP3204 en sortie*/
      LATEbits.LATE0=1;              	/* /CS = 1 */

//Configuration de la SPI
	SSPSTAT&=0b00111111;	// bit6=0 => CKE=0 (transmission CLK repos à actif)
				// bit7=0 => SMP=0 (lecture donnée au milieu)

	SSPCON1|=0b00110000;
	SSPCON1&=0b11110000; // bit0 à 3 => MSSP en mode SPI maitre à Fosc/4
			     // bit4=1 car CKP=1 (Clk au repos à 1)
			     // bit5=1 => SSPEN=1 module SSP validé
}

/************************************************************************
;		unsigned char echangeSPI(unsigned char data)
;	fonction:
;		échange d'un octet par la liaison SPI en mode maître
;	Argument:	on passe en paramètre l'octet à envoyer
;	Retour: on récupère l'octet reçu
*************************************************************************/
unsigned char echangeSPI(unsigned char data)
{
	SSPBUF=data; 	// envoi des donnée à l'esclave
	while(!SSPSTATbits.BF);	 // attente que le tranfert soit terminé
	return SSPBUF;	// récupération de la donnée envoyée par l'esclave
}
