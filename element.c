#include <stdio.h>
#include <string.h>
#include "element.h"
/******************************************
ces fonctions permettent l'insertion des 
elements dans la liste doublement chainée 
utilisée 
******************************************/


void saisir_element(Element *elem)
{
    printf("Donnez le nom:");
    fgets(elem->nom,MAX_NOM,stdin);
    elem->nom[strlen(elem->nom)-1]='\0';
    printf("Donnez la valeur:");
    scanf("%d%*c",&elem->valeur);
}

void afficher_element(Element elem)
{
    printf("%s, %d\n",elem.nom,elem.valeur);
}

