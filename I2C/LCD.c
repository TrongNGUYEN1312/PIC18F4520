/* *******************************************
	fichier LCD.C des fonctions pour communiquer avec le LCD
	ATTENTION! les câblages sont définis dans LCD.H
**********************************************/
/*Definition du fichier header contenant les caractéristiques du microcontroleur*/
#include <xc.h>

#include "LCD.h"

#define _XTAL_FREQ 8000000      // Quartz frequency

/* Enable pulse */
void ToggleEpinOfLCD(void)
{
	LCD_E = 1;                  // Give a pulse on E pin
	__delay_us(E_Delay);        // so that LCD can latch the
//    _delay(1);
//    NOP();
	LCD_E = 0;                  // data from data bus
	__delay_us(E_Delay);      
//    _delay(1);
//    NOP();
}

/* Initialization of LCD */
void InitLCD(void)
{ 
	// Firstly make all pins output
	LCD_E  		 		 = 0;   // E  = 0
	LCD_RS  	 		 = 0;   // RS = 0
	LCD_Data_Bus_D4		 = 0;  	// Data bus = 0
	LCD_Data_Bus_D5		 = 0;  	// Data bus = 0
	LCD_Data_Bus_D6		 = 0;  	// Data bus = 0
	LCD_Data_Bus_D7		 = 0;  	// Data bus = 0
	LCD_E_Dir    		 = 0;   // Make Output
	LCD_RS_Dir    	 	 = 0;   // Make Output
	LCD_Data_Bus_Dir_D4  = 0;   // Make Output
	LCD_Data_Bus_Dir_D5  = 0;   // Make Output
	LCD_Data_Bus_Dir_D6  = 0;   // Make Output
	LCD_Data_Bus_Dir_D7  = 0;   // Make Output

  ///////////////// Reset process from datasheet //////////////
    __delay_ms(40);
   
	LCD_Data_Bus &= 0xF0;       // Make Data pins zero
	LCD_Data_Bus |= (0x3);      // Write 0x3 value on data bus
	ToggleEpinOfLCD();          // Give pulse on E pin
    __delay_us(39);
    
	WriteCommandToLCD(0x28);    //function set
    __delay_us(39);
/////////////// Reset Process End ////////////////
	WriteCommandToLCD(0x28);    //function set
    __delay_us(37);
	WriteCommandToLCD(0x0f);    //display ON/OFF control
    __delay_us(37);
	WriteCommandToLCD(0x01);    //clear display
    __delay_ms(2);
	WriteCommandToLCD(0x06);    // entry mode set 
    __delay_us(39);
}

/************************************************************************
;		void WriteCommandToLCD(unsigned char Command)
;	fonction:   écrit/envoie une commande sur le LCD
;	Argument:   la commande (octet)
;	Retour:
*************************************************************************/
void WriteCommandToLCD(unsigned char Command)  
{
	LCD_RS = 0;                     // 
	
	LCD_Data_Bus &= 0xF0;           // 
	LCD_Data_Bus |= ((Command&0xF0)>>4);  // 
	ToggleEpinOfLCD();              // 	
	
	LCD_Data_Bus &= 0xF0;           // 
	LCD_Data_Bus |= (Command&0x0F); // 
	ToggleEpinOfLCD();              // 
}

/************************************************************************
;		void WriteDataToLCD(unsigned char LCDChar)
;	fonction:   écrit/envoie une donnée (un caractère) sur le LCD
;	Argument:   le caractère (code ASCII)
;	Retour:
*************************************************************************/
void WriteDataToLCD(unsigned char LCDChar)  
{
	LCD_RS = 1;                         // 
	
	LCD_Data_Bus &= 0xF0;               // 
	LCD_Data_Bus |= ((LCDChar&0xF0)>>4);    // 
	ToggleEpinOfLCD();                  // 	
	
	LCD_Data_Bus &= 0xF0;               // 
	LCD_Data_Bus |= (LCDChar&0x0F);     // 
	ToggleEpinOfLCD();                  // 
    __delay_us(43);              
}

/************************************************************************
;		void WriteStringToLCD(unsigned char * s)
;	fonction:   affiche une chaîne de caractère stockée dans un tableau
;               sur le LCD
;	Argument:   adresse (pointeur) de la chaine de caractère
;	Retour:
;   exemple: 	unsigned char message[]="Bonjour!";
;				WriteStringToLCD(message);
*************************************************************************/
void WriteStringToLCD(unsigned char *s)
{
	while(*s)
		WriteDataToLCD(*s++);	
}

/************************************************************************
;		WriteStringConstToLCD(unsigned char * s)
;	fonction:   affiche une chaîne de caractère constante passée directement
;               en paramètre sur le LCD
;	Argument:   la chaine de caractère est directement passée en paramètre
;	Retour:
;   exemple:    WriteStringConstToLCD((const rom unsigned char *)"Bonjour!");
*************************************************************************/
void WriteStringConstToLCD(const unsigned char *s)
{
	while(*s)
		WriteDataToLCD(*s++);
}

/* clear the screen of LCD and return cursor at zero position*/
void ClearLCDScreen(void)       
{
	WriteCommandToLCD(0x01);    // Clear the screen
	__delay_ms(2);              // Delay for cursor to return at zero position
}

/************************************************************************
;		void GotoLine1ToLCD(unsigned char p)
;	fonction: positionne le curseur sur la ligne 1 du LCD
;	Argument: la valeur p est la position sur la ligne (0 à 15)
;	Retour:
*************************************************************************/
void GotoLine1ToLCD(unsigned char p)
{
	if(p<16)
		WriteCommandToLCD(0x80+p);
    __delay_us(39);
}

/************************************************************************
;		void GotoLine2ToLCD(unsigned char p)
;	fonction: positionne le curseur sur la ligne 2 du LCD
;	Argument: la valeur p est la position sur la ligne (0 à 15)
;	Retour:
*************************************************************************/
void GotoLine2ToLCD(unsigned char p)
{
	if(p<16)
		WriteCommandToLCD(0x80+0x40+p);
    __delay_us(39);
}

/************************************************************************
;		void WriteBCDToLCD(unsigned char c)
;	fonction: affiche une valeur BCD (0 à 9) sur le LCD
;	Argument: la valeur BCD c  est passée en paramètre
;	Retour:
*************************************************************************/
void WriteBCDToLCD(unsigned char c)
{
	if(c<10)                        
		WriteDataToLCD( c+0x30);
}


	