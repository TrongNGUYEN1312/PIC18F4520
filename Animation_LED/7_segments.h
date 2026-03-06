#ifndef H_7_SEGMENTS

#define H_7_SEGMENTS

#include <pic18f4520.h>

void init_7Segments(void);
void conversion_Char2BCD(unsigned char,unsigned char *);
unsigned char ConversionBCD7Segments( unsigned char);
void ConversionDigits7Segments( unsigned char *, unsigned char * );
void GestionAfficheur7Segments( unsigned char *, unsigned char);
void affichage_7Segments(unsigned char);


#endif H_7_SEGMENTS