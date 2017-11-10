#define MAX_NOM 50
typedef struct
{
   int valeur;
   char nom[MAX_NOM+1];
}Element;

void saisir_element(Element *elem);
//void affecter_element(Element *vers, Element de);
void afficher_element(Element elem);
