#ifndef H_ANIMATIONS

#define H_ANIMATIONS

#include "clavier.h"
#include <pic18f4520.h>

void init_animations(void);
unsigned char gestion_bouton(unsigned char);
unsigned char gestion_clavier(unsigned char);
unsigned char changement_num_animation(unsigned char, unsigned char*);
unsigned char Calcul_LED_animation(unsigned char,unsigned char);
unsigned char chenillardG(unsigned char);
unsigned char chenillardD(unsigned char);
unsigned char vagueG(unsigned char);
unsigned char vagueD(unsigned char);
unsigned char alternance(unsigned char);
unsigned char clignotement(unsigned char);

#endif H_ANIMATIONS