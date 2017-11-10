#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"
#include <string.h>

int main() {
	char *message = NULL;
    int i=0;
	Initialisation();

	while(1) {
		int fini = 0;
	
		AttenteClient();
	
		while(!fini) {
		
			message = Reception();
			
			if(message != NULL) 
			{ 
				if(!strcmp(message,"First_connexion\n"))
				{
				       

                    free(message);
					inscriptionBasedeDonnee();                    

				}
				else if(!strcmp(message,"CONNECT_C\n"))
			    {

			    	gestionconnexion();
			    	
			    }
			    else 
			        gestionDesErreurs();	

			}
			
			else 
			{

				fini = 1;
			}
		}

		

		TerminaisonClient();
	}

	return 0;
}
