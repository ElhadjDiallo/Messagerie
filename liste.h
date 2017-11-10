#include <stdbool.h>
#include "gestionfichier.h"
typedef struct etliste * Liste;

void initialiser_liste(Liste *maliste);
bool vid_liste(Liste maliste);
bool est_premier(Liste maliste,Element element);
bool est_dernier(Liste maliste,Element element);
Element recherche_element(Liste maliste,char chaine[]);
Liste ajoute_element(Liste maliste,Element element);
Liste supprime_element(Liste maliste,Element element);
void imprimer_liste_croissant(Liste maliste,bool mode);
