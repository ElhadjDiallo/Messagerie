#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "liste.h"
typedef struct  Etcel 
{
    Element elem;
    struct Etcel *precedent;
    struct Etcel *suivant;
}Cel;

struct etliste 
{
   Cel *debut;
   Cel *fin;
};
   
void initialiser_liste(Liste *maliste)
{
   (*maliste)=(Liste ) malloc(sizeof(struct etliste));
   (*maliste)->debut=NULL;
   (*maliste)->fin=NULL;
}

bool vid_liste(Liste maliste)
{
    return (maliste->debut==NULL);
}

bool est_premier(Liste maliste,Element element)
{
    if(vid_liste(maliste))
       return false;
    return(maliste->debut->elem.valeur==element.valeur && !strcmp(maliste->debut->elem.nom,element.nom));
}

bool est_dernier(Liste maliste,Element element)
{
    if(vid_liste(maliste))
       return false;
    return(maliste->fin->elem.valeur==element.valeur && !strcmp(maliste->fin->elem.nom,element.nom));
}
 
Element recherche_element(Liste maliste,char chaine[])
{
   Element element;
   Cel *parcours;
   int trouve=0;
   element.valeur=-1;
   parcours=maliste->debut;
   while(parcours!=NULL && !trouve)
   {
      if(!strcmp(parcours->elem.nom,chaine))
      {
          strcpy(element.nom,parcours->elem.nom);
          element.valeur=parcours->elem.valeur;
          trouve=1;
      }
      else parcours=parcours->suivant;
   }    
   return element;
}

void trouverPlace(Liste maliste,Cel **avant,Cel **apres,Element element)
{
   Cel *cour=maliste->debut;
   *avant=NULL;
   *apres=cour;
   while(cour!=NULL)
   {
	if(cour->elem.valeur<element.valeur)
 	{
	    *avant=cour;
	    cour=cour->suivant;
	    *apres=cour;
	}
	else cour=NULL;
   }
}

void trouverElement(Liste maliste,Cel **avant,Cel **apres,Element element)
{
   Cel *cour=maliste->debut;
   *avant=NULL;
   *apres=cour;
   while(cour!=NULL)
   {
	if(strcmp(element.nom,cour->elem.nom))
 	{
	    *avant=cour;
	    cour=cour->suivant;
	    *apres=cour;
	}
	else cour=NULL;
   }
}

Liste ajoute_element(Liste maliste,Element element)
{
   Cel *aux,*avant,*apres;
   trouverPlace(maliste,&avant,&apres,element);
   aux=(Cel *) malloc(sizeof(Cel));
   assert(aux!=NULL);
   aux->elem.valeur=element.valeur;
   strcpy(aux->elem.nom,element.nom);
   aux->precedent=avant;
   aux->suivant=apres;
   if(avant==NULL)
	maliste->debut=aux;
   else aux->precedent->suivant=aux;
   if(apres==NULL)
	maliste->fin=aux;
   else aux->suivant->precedent=aux;
   return maliste;
}

Liste supprime_element(Liste maliste,Element element)
{
   Cel *avant,*apres;
   trouverElement(maliste,&avant,&apres,element);
   if(apres==NULL || apres->elem.valeur>element.valeur)
	return maliste;
   if(avant==NULL)
	maliste->debut=apres->suivant;
   else avant->suivant=apres->suivant;
   if(apres->suivant==NULL)
	maliste->fin=avant;
   else apres->suivant->precedent=avant;
   free(apres);
   return maliste;
}

void imprimer_liste_croissant(Liste maliste,bool mode)
{
    Cel *parcours;
    if(mode)
	parcours=maliste->debut;
    else parcours=maliste->fin;
    while(parcours!=NULL){
	afficher_element(parcours->elem);
        if(mode)
	    parcours=parcours->suivant;
	else  parcours=parcours->precedent;
    }
}
