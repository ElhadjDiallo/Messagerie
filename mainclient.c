#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <string.h>
#include "couleurs.h"
int main() 
{
	char *message;
	int choix;
	char *machine;
	connexion(machine);
     clrscr();
    do
    {
     menu();
     controleSaisie(&choix);
     viderBuffer(); 
	 switch(choix)
	 {
	 	case 1:
	 	{
	 		clrscr();
	 		inscription();
            break;
	 	}
	 	case 2:
	 	{
             clrscr();
	 		connecter();
	 		break;
	 	}
	 	case 3:
	 	{    clrscr();
	 		 
             printf("Merci \n");
               
             break;         
	 	}
	 	default:
	 	{    
	 		printf("suivez le menu \n");
	 		break;
	 	}

	 	clrscr();

	 }
     
	 
	}while(choix!=3);
	 
   Terminaison();
	

	return 0;
}
