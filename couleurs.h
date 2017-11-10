#ifndef COULEURS

#define COULEURS
// fonction qui pemet de mettre les couleurs à l'ecran et aussi d'effacer la console

#include <stdio.h>


#define clrscr() printf("\033[H\033[2J")
#define couleur(param) printf("\033[%sm",param)

#endif

